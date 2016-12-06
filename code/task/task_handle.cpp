/*
//
//	Last Modify Date: 2016-09-26
//	Author: zengpw
//	History:
//					2015-09-02 zengpw created
//	File Property: private
//	Future:
//					I. 第一轮消息处理
//
//
*/

#include "task_handle.h"

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

session::HANDLE_RTN TaskHandle::cidDataGps(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDataLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "L");
}

session::HANDLE_RTN TaskHandle::cidDataTh(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDataTh2(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	string key = in->getDevId() + "_3";
	rename(key);

	//负载 - 1
	LOAD_DECR(1);

	return flashPush(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDevUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	//负载 - 1
	LOAD_DECR(1);

	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDevband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_DEV_BAND);

	//时间
	DateTimeSec dts = buildDateTimeSec();
	out->add(TID_TIME, sizeof(DateTimeSec), (char*)&dts);

	//订单号
	string order_id;
	TLV_ORDER_ID order;

	if(!dp.public_get_order_id(db, in->getDevId(), order_id))
	{
		//返回空订单号
		memset(order, 0, sizeof(order));
		out->add(TID_ORDER_ID, sizeof(TLV_ORDER_ID), (char*)&order);

		//目的是为了存入 redis 做后续处理，并不是为了处理 out
		flashPush(in, out, "H");
	}
	else
	{
		memset(order, 0, sizeof(order));
		memcpy(order, order_id.c_str(), sizeof(order));
		out->add(TID_ORDER_ID, sizeof(TLV_ORDER_ID), (char*)&order);
	}

	//运行参数
	string protocol_ver;
	dp.public_get_protocol_ver(db, in->getDevId(), protocol_ver);

	if(protocol_ver == "11" || protocol_ver == "12")
	{
		TLV_PARAMETER_V11 para;

		if(!dp.public_get_parameter_request_ver11(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V11), (char*)&para);
	}
	else if(protocol_ver == "13")
	{
		TLV_PARAMETER_V13 para;

		if(!dp.public_get_parameter_request_ver13(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V13), (char*)&para);
	}
	else if(protocol_ver == "14")
	{
		TLV_PARAMETER_V14 para;

		if(!dp.public_get_parameter_request_ver14(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V14), (char*)&para);
	}
	else
	{
		TLV_PARAMETER para;

		if(!dp.public_get_parameter_request(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER), (char*)&para);
	}

	out->build();

	//负载 + 1
	LOAD_INCR(1);

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::cidDevbandTime(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_DEV_BAND_TIME);

	//时间
	DateTimeSec dts = buildDateTimeSec();
	out->add(TID_TIME, sizeof(DateTimeSec), (char*)&dts);

	//订单号
	string order_id;
	TLV_ORDER_ID order;

	if(!dp.public_get_order_id(db, in->getDevId(), order_id))
	{
		//返回空订单号
		memset(order, 0, sizeof(order));
		out->add(TID_ORDER_ID, sizeof(TLV_ORDER_ID), (char*)&order);

		//目的是为了存入 redis 做后续处理，并不是为了处理 out
		flashPush(in, out, "H");
	}
	else
	{
		memset(order, 0, sizeof(order));
		memcpy(order, order_id.c_str(), sizeof(order));
		out->add(TID_ORDER_ID, sizeof(TLV_ORDER_ID), (char*)&order);
	}

	//运行参数
	string protocol_ver;
	dp.public_get_protocol_ver(db, in->getDevId(), protocol_ver);

	if(protocol_ver == "11" || protocol_ver == "12")
	{
		TLV_PARAMETER_V11 para;

		if(!dp.public_get_parameter_request_ver11(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V11), (char*)&para);
	}
	else if(protocol_ver == "13")
	{
		TLV_PARAMETER_V13 para;

		if(!dp.public_get_parameter_request_ver13(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V13), (char*)&para);
	}
	else if(protocol_ver == "14")
	{
		TLV_PARAMETER_V14 para;

		if(!dp.public_get_parameter_request_ver14(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V14), (char*)&para);
	}
	else
	{
		TLV_PARAMETER para;

		if(!dp.public_get_parameter_request(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER), (char*)&para);
	}

	out->build();

	//负载 + 1
	LOAD_INCR(1);

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::cidDevMsg(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return flashPush(in, out, "H");
}

session::HANDLE_RTN TaskHandle::cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	string key = in->getDevId() + "_6";
	rename(key);

	return flashPush(in, out, "H");
}

session::HANDLE_RTN TaskHandle::cidParameterRequest(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_PARAMETER_REQUEST);

	//运行参数
	string protocol_ver;
	dp.public_get_protocol_ver(db, in->getDevId(), protocol_ver);

	if(protocol_ver == "11" || protocol_ver == "12")
	{
		TLV_PARAMETER_V11 para;

		if(!dp.public_get_parameter_request_ver11(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V11), (char*)&para);
	}
	else if(protocol_ver == "13")
	{
		TLV_PARAMETER_V13 para;

		if(!dp.public_get_parameter_request_ver13(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V13), (char*)&para);
	}
	else if(protocol_ver == "14")
	{
		TLV_PARAMETER_V14 para;

		if(!dp.public_get_parameter_request_ver14(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V14), (char*)&para);
	}
	else
	{
		TLV_PARAMETER para;

		if(!dp.public_get_parameter_request(db, in->getDevId(), para))
			return session::HANDLE_RTN_CONTINUE;
		else
			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER), (char*)&para);
	}

	out->build();

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	string key = in->getDevId() + "_8";
	rename(key);

	return flashPush(in, out, "H");
}

session::HANDLE_RTN TaskHandle::cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "L");
}

session::HANDLE_RTN TaskHandle::cidEFence(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	LOG_DEBUG("dev_id = " << in->getDevId() << " e_fence return!");

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidDataExposure(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDataShock(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDataTilt(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidDataBaro(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::cidTimeStart(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidTimeStart processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_RTN)
		{
			TLV_RTN data;

			if(sizeof(data) != iter->len)
			{
				LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = cidTimeStart, version error!");
				continue;
			}

			memset(&data, 0, sizeof(data));
			memcpy(&data, iter->value, iter->len);

			if((int)data == 1)
			{
				string key = in->getDevId() + "_23";
				rename(key);
			}
		}
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidTimeEnd(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidTimeEnd processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_RTN)
		{
			TLV_RTN data;

			if(sizeof(data) != iter->len)
			{
				LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = cidTimeEnd, version error!");
				continue;
			}

			memset(&data, 0, sizeof(data));
			memcpy(&data, iter->value, iter->len);

			if((int)data == 1)
			{
				string key = in->getDevId() + "_26";
				rename(key);
			}
		}
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::cidDevType(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return flashPush(in, out, "H");
}

session::HANDLE_RTN TaskHandle::cidSrvStatus(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	out->setCID(CID_SRV_STATUS);

	//运行参数
	TLV_SRV_STATUS para;
	memset(&para, 0, sizeof(para));

	char buf[64];
	stringstream ss;

	memset(buf, 0, sizeof(buf));
	ss << string("this is a c_struct array");
	ss >> para.array_test;
	ss.str("");
	ss.clear();

	para.short_test = 100;
	para.u_int_test = 1000000;
	para.char_test = 'a';

	out->add(TID_SRV_STATUS, sizeof(TLV_SRV_STATUS), (char*)&para);
	out->build();

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::cidAutoUploadR(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return push(in, out, "M");
}

session::HANDLE_RTN TaskHandle::sending(const protocol::PacketIn* in, vector<protocol::PacketOut> &vp)
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

			//解绑
			if(vs[i] == in->getDevId() + "_3")
			{
				if(sendUnband(in, &out) == session::HANDLE_RTN_SEND)
				{
					LOG_DEBUG("build message : redis key = " << vs[i]);
					vp.push_back(out);
				}
			}

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

			//初始化
			if(vs[i] == in->getDevId() + "_9")
			{
				if(sendInit(in, &out) == session::HANDLE_RTN_SEND)
				{
					vp.push_back(out);
				}
			}

			//电子围栏 - 进入
			if(vs[i] == in->getDevId() + "_11")
			{
				if(sendEFenceEntry(in, &out) == session::HANDLE_RTN_SEND)
				{
					vp.push_back(out);
					rename(vs[i]);
				}
			}

			//电子围栏 - 离开
			if(vs[i] == in->getDevId() + "_12")
			{
				if(sendEFenceExit(in, &out) == session::HANDLE_RTN_SEND)
				{
					vp.push_back(out);
					rename(vs[i]);
				}
			}

			//起运时间下发
			if(vs[i] == in->getDevId() + "_23")
			{
				if(sendStartTime(in, &out) == session::HANDLE_RTN_SEND)
				{
					vp.push_back(out);
				}
			}

			//签收时间下发
			if(vs[i] == in->getDevId() + "_26")
			{
				if(sendEndTime(in, &out) == session::HANDLE_RTN_SEND)
				{
					vp.push_back(out);
				}
			}
		}
	}

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::sendUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	out->setCID(CID_UNBAND);
	DateTimeSec dts = buildDateTimeSec();
	out->add(TID_TIME, sizeof(DateTimeSec), (char*)&dts);
	out->build();

	return session::HANDLE_RTN_SEND;
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

session::HANDLE_RTN TaskHandle::sendInit(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN TaskHandle::sendEFenceEntry(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	out->setCID(CID_E_FENCE);
	shortResponse(out, 1);
	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::sendEFenceExit(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	out->setCID(CID_E_FENCE);
	shortResponse(out, 0);
	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::sendStartTime(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_TIME_START);

	TLV_TIME_6 dts;
	TLV_EXPRESS_ID express_id;

	if(!dp.public_get_start(db, in->getDevId(), dts, express_id))
	{
		return session::HANDLE_RTN_CONTINUE;
	}
	else
	{
		out->add(TID_TIME_6, sizeof(TLV_TIME_6), (char*)&dts);
		out->add(TID_EXPRESS_ID, sizeof(TLV_EXPRESS_ID), (char*)&express_id);
	}

	out->build();

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN TaskHandle::sendEndTime(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_TIME_END);

	TLV_TIME_6 dts;

	if(!dp.public_get_end(db, in->getDevId(), dts))
		return session::HANDLE_RTN_CONTINUE;
	else
		out->add(TID_TIME_6, sizeof(TLV_TIME_6), (char*)&dts);

	out->build();

	return session::HANDLE_RTN_SEND;
}



