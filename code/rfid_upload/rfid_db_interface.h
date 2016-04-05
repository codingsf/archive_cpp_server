/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息入库
//
//
*/

#ifndef _RFID_DB_INTERFACE_H_
#define _RFID_DB_INTERFACE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/db.h"
#include "common/log.h"
#include "session/db_public.h"
#include "session/http_interface.h"
#include "tlv_rfid_handle_data.h"

using namespace std;

class RfidDbInterface : public session::DbPublic
{
public:
	TLV_HANDLE_RTN rs232_update(common::DB* db, TLV_RFID_HANDLE_RS232& rs232_handle);
};

#endif