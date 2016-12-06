/*
//
//	Last Modify Date: 2016-09-29
//	Author: zengpw
//	History:
//					2015-09-02 zengpw created
//	File Property: private
//	Future:
//					I. 第一轮消息处理
//
//
*/

#include "task_jzt_handle.h"

string TaskHandle::getRealQueue(const string& dev_id, const string& queue_name, const string& queue_property)
{
	if(this->key == 0)
	{
		return queue_name;
	}

	if(queue_property == "H")
	{
		if(this->mod_x == 1)
		{
			return queue_name + "_high";
		}

		string str = queue_name + "_high_";
		str += common::getMod(dev_id, this->mod_x);

		return str;
	}

	if(queue_property == "M")
	{
		if(this->mod_y == 1)
		{
			return queue_name + "_mid";
		}

		string str = queue_name + "_mid_";
		str += common::getMod(dev_id, this->mod_y);

		return str;
	}

	if(queue_property == "L")
	{
		if(this->mod_z == 1)
		{
			return queue_name + "_low";
		}

		string str = queue_name + "_low_";
		str += common::getMod(dev_id, this->mod_z);

		return str;
	}

	return queue_name;
}

void TaskHandle::shortResponse(protocol::PacketOut* out, int success)
{
	TLV_RTN rtn = success;
	out->add(TID_RTN, sizeof(TLV_RTN), (char*)&rtn);
	out->build();
}

session::HANDLE_RTN TaskHandle::push(const protocol::PacketIn* in, protocol::PacketOut* out, const string& queue_property)
{
	string queue = getRealQueue(in->getDevId(), "msg_upload", queue_property);

	if(rc_upload->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) == 0)
	{
		shortResponse(out, 1);
		return session::HANDLE_RTN_SEND;
	}
	else
	{
		LOG_DEBUG("redis push occur serious error, please check redis!");
		return session::HANDLE_RTN_STOP;
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::flashPush(const protocol::PacketIn* in, protocol::PacketOut* out, const string& queue_property)
{
	string queue = getRealQueue(in->getDevId(), "msg_upload", queue_property);

	if(rc_upload->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) != 0)
	{
		LOG_DEBUG("redis push occur serious error, please check redis!");
		return session::HANDLE_RTN_STOP;
	}

	return session::HANDLE_RTN_CONTINUE;
}

void TaskHandle::rename(string& key)
{
	string end = common::buildDateTime();
	string new_key = key + "_";
	new_key += end;

	if(rc_sending->redis_rename(key.c_str(), new_key.c_str()) == 0)
	{
		//修改 sts ，修改 end time
		// sts =0 初始状态； =1 下发成功； =2 未下发，被新的命令覆盖
		rc_sending->redis_hmset4update(new_key.c_str(), end.c_str(), "1");
	}
}

session::HANDLE_RTN TaskHandle::reject(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	LOG_DEBUG("--------------- reject message, command id = 0x" << hex << setiosflags(ios::uppercase) << setfill('0') << setw(4) << in->getCID() << " ---------------");
	out->copy(in);
	shortResponse(out, 0);

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	if(this->mode == 0)
	{
		return push(in, out, "L");
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	if(this->mode == 0)
	{
		string key = in->getDevId() + "_6";
		rename(key);
		return flashPush(in, out, "H");
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	if(this->mode == 0)
	{
		string key = in->getDevId() + "_8";
		rename(key);

		return flashPush(in, out, "H");
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidJztDev(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	if(this->mode == 1)
	{
		return push(in, out, "H");
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidJztLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	if(this->mode == 1)
	{
		return push(in, out, "L");
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidJztTh2(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	if(this->mode == 1)
	{
		return push(in, out, "M");
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::sending(const protocol::PacketIn* in, vector<protocol::PacketOut> &vp)
{
	if(this->mode == 0)
	{
		string keys = in->getDevId() + "_*";
		vector<string> vs;

		if(rc_sending->redis_keys(keys, vs) == 0)
		{
			for(unsigned int i = 0; i < vs.size(); i++)
			{
				protocol::PacketOut out;
				out.copy(in);

				LOG_DEBUG("matching command = " << vs[i]);

				//运行参数下发
				if(vs[i] == in->getDevId() + "_6")
				{
					if(sendParameter(in, &out) == session::HANDLE_RTN_SEND)
					{
						vp.push_back(out);
					}
				}

				//版本升级
				if(vs[i] == in->getDevId() + "_8")
				{
					if(sendVersion(in, &out) == session::HANDLE_RTN_SEND)
					{
						vp.push_back(out);
					}
				}
			}
		}

		return session::HANDLE_RTN_SEND;
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::sendParameter(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_PARAMETER);

	string protocol_ver;
	dp.public_get_protocol_ver(db, in->getDevId(), protocol_ver);

	if(protocol_ver == "11" || protocol_ver == "12")
	{
		TLV_PARAMETER_V11 para;

		if(!dp.public_get_parameter_sending_ver11(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V11), (char*)&para);
	}
	else if(protocol_ver == "13")
	{
		TLV_PARAMETER_V13 para;

		if(!dp.public_get_parameter_sending_ver13(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V13), (char*)&para);
	}
	else if(protocol_ver == "14")
	{
		TLV_PARAMETER_V14 para;

		if(!dp.public_get_parameter_sending_ver14(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V14), (char*)&para);
	}
	else
	{
		TLV_PARAMETER para;

		if(!dp.public_get_parameter_sending(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER), (char*)&para);
	}

	out->build();

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::sendVersion(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_PARAMETER_VERSION);

	TLV_PARAMETER_VERSION ver;

	if(!dp.public_get_version(db, in->getDevId(), ver))
		return session::HANDLE_RTN_CONTINUE;
	else
		out->add(TID_PARAMETER_VERSION, sizeof(TLV_PARAMETER_VERSION), (char*)&ver);

	out->build();

	return session::HANDLE_RTN_SEND;
}




