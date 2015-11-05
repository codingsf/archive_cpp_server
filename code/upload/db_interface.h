/*
//
//	Last Modify Date: 2015-11-03
//	Author: zengpw
//	History:
//					2015-10-21 zengpw created
//	File Property: private
//	Future:
//					I. 数据库操作
//
//
*/

#ifndef _DB_INTERFACE_H_
#define _DB_INTERFACE_H_

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
#include "tlv_handle_data.h"

using namespace std;

class DbInterface : public session::DbPublic
{
public:
	TLV_HANDLE_RTN gps_update(common::DB* db, TLV_HANDLE_GPS& gps_handle);
	TLV_HANDLE_RTN lbs_update(common::DB* db, TLV_HANDLE_LBS& lbs_handle);
	TLV_HANDLE_RTN th_update(common::DB* db, TLV_HANDLE_TH& th_handle);
	TLV_HANDLE_RTN unband_update(common::DB* db, TLV_HANDLE_UNBAND_RTN& unband_handle);
	TLV_HANDLE_RTN dev_unband_update(common::DB* db, TLV_HANDLE_DEV_UNBAND_RTN& dev_unband_handle);
};

#endif