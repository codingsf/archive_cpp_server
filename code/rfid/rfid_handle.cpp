/*
//
//	Last Modify Date: 2016-02-18
//	Author: zengpw
//	History:
//					2016-02-18 zengpw created
//	File Property: private
//	Future:
//					I. RFID 第一轮消息处理
//
//
*/

#include "rfid_handle.h"

string RfidHandle::getRealQueue(const string& dev_id, const string& queue_name)
{
	if(this->mod == 1)
	{
		return queue_name;
	}
	else
	{
		string str = queue_name + "_";
		str += common::getMod(dev_id, this->mod);

		return str;
	}
}

session::HANDLE_RTN RfidHandle::flashPush(const protocol::PacketRFID* in)
{
	string queue = getRealQueue("1", "rfid_upload");

	if(rc_upload->redis_lpush(queue.c_str(), in->getMsg(), in->getMsgLen()) != 0)
	{
		LOG_DEBUG("redis push occur serious error, please check redis!");
		return session::HANDLE_RTN_STOP;
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN RfidHandle::ridSOF(const protocol::PacketRFID* in)
{
	return flashPush(in);
}