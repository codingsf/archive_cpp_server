/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息分析和入库
//
//
*/

#ifndef _RFID_UPLOAD_HANDLE_H_
#define _RFID_UPLOAD_HANDLE_H_

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
#include "common/db.h"
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_rfid.h"
#include "session/handle_basic.h"
#include "tlv_rfid_handle.h"
#include "rfid_db_interface.h"

using namespace std;

class RfidUploadHandle : public session::HandleBasic
{
public:
	explicit RfidUploadHandle(string db_conn_info):HandleBasic(db_conn_info){}
public:
	session::HANDLE_RTN ridSOF(const protocol::PacketRFID* in);
};

#endif