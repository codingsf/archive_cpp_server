/*
//
//	Last Modify Date: 2016-10-28
//	Author: zengpw
//	History:
//					2016-09-27 zengpw created
//	File Property: private
//	Future:
//					I. 九州通消息分析和入库
//
//
*/

#ifndef _UPLOAD_HANDLE_H_
#define _UPLOAD_HANDLE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "common/redis.h"
#include "common/db.h"
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_tlv.h"
#include "session/handle_basic.h"
#include "tlv_handle.h"
#include "db_interface.h"

using namespace rapidjson;
using namespace std;

class UploadHandle : public session::HandleBasic
{
public:
	int mode;
	common::Redis* rc_jzt;
public:
	UploadHandle():HandleBasic(){}
	UploadHandle(string db_conn_info):HandleBasic(db_conn_info){}
public:
	session::HANDLE_RTN jztPush(const char* key, const char* value, const int length);
public:
	session::HANDLE_RTN cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out);
public:
	session::HANDLE_RTN cidJztDev(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidJztLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidJztTh2(const protocol::PacketIn* in, protocol::PacketOut* out);
};

#endif