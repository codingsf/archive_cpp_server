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

#ifndef _TASK_JZT_HANDLE_H_
#define _TASK_JZT_HANDLE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/redis.h"
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_tlv.h"
#include "session/handle_basic.h"
#include "session/db_public.h"

using namespace std;

class TaskHandle : public session::HandleBasic
{
public:
	int mode;
	int key;
	int mod_x;
	int mod_y;
	int mod_z;
	common::Redis* rc_upload;
	common::Redis* rc_sending;
public:
	TaskHandle():HandleBasic(){}
	TaskHandle(string db_conn_info):HandleBasic(db_conn_info){}
public:
	string getRealQueue(const string& dev_id, const string& queue_name, const string& queue_property);
	void shortResponse(protocol::PacketOut* out, int success);
	session::HANDLE_RTN push(const protocol::PacketIn* in, protocol::PacketOut* out, const string& queue_property);
	session::HANDLE_RTN flashPush(const protocol::PacketIn* in, protocol::PacketOut* out, const string& queue_property);
	void rename(string& key);
public:
	session::HANDLE_RTN reject(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out);
public:
	session::HANDLE_RTN cidJztDev(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidJztLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidJztTh2(const protocol::PacketIn* in, protocol::PacketOut* out);
public:
	session::HANDLE_RTN sending(const protocol::PacketIn* in, vector<protocol::PacketOut> &vp);
	session::HANDLE_RTN sendParameter(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN sendVersion(const protocol::PacketIn* in, protocol::PacketOut* out);
};

#endif