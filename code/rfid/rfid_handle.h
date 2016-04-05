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

#ifndef _RFID_HANDLE_H_
#define _RFID_HANDLE_H_

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
#include "protocol/protocol_rfid.h"
#include "session/handle_basic.h"

using namespace std;

class RfidHandle : public session::HandleBasic
{
public:
	int mod;
	common::Redis* rc_upload;
public:
	string getRealQueue(const string& dev_id, const string& queue_name);
	session::HANDLE_RTN flashPush(const protocol::PacketRFID* in);
public:
	session::HANDLE_RTN ridSOF(const protocol::PacketRFID* in);
};

#endif