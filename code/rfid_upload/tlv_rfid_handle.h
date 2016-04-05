/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息处理
//
//
*/

#ifndef _TLV_RFID_HANDLE_H_
#define _TLV_RFID_HANDLE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/redis.h"
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_rfid.h"
#include "session/http_interface.h"
#include "tlv_rfid_handle_data.h"

using namespace std;

class TlvRfidHandle
{
public:
	TLV_HANDLE_RTN tlv_rfid_rs232_handle(TLV_RFID_RS232& rs232, TLV_RFID_HANDLE_RS232& rs232_handle);
};

#endif