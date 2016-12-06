/*
//
//	Last Modify Date: 2016-08-26
//	Author: zengpw
//	History:
//					2015-08-26 zengpw created
//	File Property: public
//	Future:
//					I. 负载均衡消息处理
//
//
*/

#include "balance_handle.h"

string BalanceHandle::getRealQueue(const string& dev_id, const string& queue_name, const string& queue_property)
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

session::HANDLE_RTN BalanceHandle::reject(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	LOG_DEBUG("--------------- reject message, command id = 0x" << hex << setiosflags(ios::uppercase) << setfill('0') << setw(4) << in->getCID() << " ---------------");
	out->copy(in);

	TLV_RTN rtn = 0;
	out->add(TID_RTN, sizeof(TLV_RTN), (char*)&rtn);
	out->build();

	return session::HANDLE_RTN_SEND;
}

session::HANDLE_RTN BalanceHandle::cidLogin(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	session::DbPublic dp;

	out->setCID(CID_LOGIN);

	//时间
	DateTimeSec dts = buildDateTimeSec();
	out->add(TID_TIME, sizeof(DateTimeSec), (char*)&dts);

	//运行参数
	string protocol_ver;
	dp.public_get_protocol_ver(db, in->getDevId(), protocol_ver);

	if(protocol_ver == "11" || protocol_ver == "12")
	{
		TLV_PARAMETER_V11 para;

		if(dp.public_get_parameter_request_ver11(db, in->getDevId(), para))
		{
			session::node n = session::HeartBeat::instance()->pop();
			LOG_DEBUG("fast server : ip = " << n.ip << ", port = " << n.port << ", load = " << n.load);

			stringstream ss;

			memset(para.addr, 0, sizeof(para.addr));
			ss << n.ip;
			ss >> para.addr;
			ss.str("");
			ss.clear();

			memset(para.port, 0, sizeof(para.port));
			ss << n.port;
			ss >> para.port;
			ss.str("");
			ss.clear();

			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V11), (char*)&para);
			out->build();

			//只要数据库能够连接，不论 redis 是否能够连接，均发送服务器信息
			string queue = getRealQueue(in->getDevId(), "msg_upload", "H");

			if(rc->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) != 0)
			{
				LOG_DEBUG("redis push occur serious error, please check redis!");
			}
		}
		else
		{
			return session::HANDLE_RTN_CONTINUE;
		}

		LOG_DEBUG("send server address to terminal " << in->getDevId() << ", ip = " << para.addr << ", port = " << para.port);
	}
	else if(protocol_ver == "13")
	{
		TLV_PARAMETER_V13 para;

		if(dp.public_get_parameter_request_ver13(db, in->getDevId(), para))
		{
			session::node n = session::HeartBeat::instance()->pop();
			LOG_DEBUG("fast server : ip = " << n.ip << ", port = " << n.port << ", load = " << n.load);

			stringstream ss;

			memset(para.addr, 0, sizeof(para.addr));
			ss << n.ip;
			ss >> para.addr;
			ss.str("");
			ss.clear();

			memset(para.port, 0, sizeof(para.port));
			ss << n.port;
			ss >> para.port;
			ss.str("");
			ss.clear();

			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V13), (char*)&para);
			out->build();

			//只要数据库能够连接，不论 redis 是否能够连接，均发送服务器信息
			string queue = getRealQueue(in->getDevId(), "msg_upload", "H");

			if(rc->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) != 0)
			{
				LOG_DEBUG("redis push occur serious error, please check redis!");
			}
		}
		else
		{
			return session::HANDLE_RTN_CONTINUE;
		}

		LOG_DEBUG("send server address to terminal " << in->getDevId() << ", ip = " << para.addr << ", port = " << para.port);
	}
	else if(protocol_ver == "14")
	{
		TLV_PARAMETER_V14 para;

		if(dp.public_get_parameter_request_ver14(db, in->getDevId(), para))
		{
			session::node n = session::HeartBeat::instance()->pop();
			LOG_DEBUG("fast server : ip = " << n.ip << ", port = " << n.port << ", load = " << n.load);

			stringstream ss;

			memset(para.addr, 0, sizeof(para.addr));
			ss << n.ip;
			ss >> para.addr;
			ss.str("");
			ss.clear();

			memset(para.port, 0, sizeof(para.port));
			ss << n.port;
			ss >> para.port;
			ss.str("");
			ss.clear();

			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER_V14), (char*)&para);
			out->build();

			//只要数据库能够连接，不论 redis 是否能够连接，均发送服务器信息
			string queue = getRealQueue(in->getDevId(), "msg_upload", "H");

			if(rc->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) != 0)
			{
				LOG_DEBUG("redis push occur serious error, please check redis!");
			}
		}
		else
		{
			return session::HANDLE_RTN_CONTINUE;
		}

		LOG_DEBUG("send server address to terminal " << in->getDevId() << ", ip = " << para.addr << ", port = " << para.port);
	}
	else
	{
		TLV_PARAMETER para;

		if(dp.public_get_parameter_request(db, in->getDevId(), para))
		{
			session::node n = session::HeartBeat::instance()->pop();
			LOG_DEBUG("fast server : ip = " << n.ip << ", port = " << n.port << ", load = " << n.load);

			stringstream ss;

			memset(para.addr, 0, sizeof(para.addr));
			ss << n.ip;
			ss >> para.addr;
			ss.str("");
			ss.clear();

			memset(para.port, 0, sizeof(para.port));
			ss << n.port;
			ss >> para.port;
			ss.str("");
			ss.clear();

			out->add(TID_PARAMETER, sizeof(TLV_PARAMETER), (char*)&para);
			out->build();

			//只要数据库能够连接，不论 redis 是否能够连接，均发送服务器信息
			string queue = getRealQueue(in->getDevId(), "msg_upload", "H");

			if(rc->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) != 0)
			{
				LOG_DEBUG("redis push occur serious error, please check redis!");
			}
		}
		else
		{
			return session::HANDLE_RTN_CONTINUE;
		}

		LOG_DEBUG("send server address to terminal " << in->getDevId() << ", ip = " << para.addr << ", port = " << para.port);
	}

	return session::HANDLE_RTN_SEND;
}
