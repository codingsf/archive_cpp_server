/*
//
//	Last Modify Date: 2016-03-11
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
#include "common/util.h"
#include "session/db_public.h"
#include "session/http_interface.h"
#include "tlv_handle_data.h"

using namespace std;

class DbInterface : public session::DbPublic
{
public:
	TLV_HANDLE_RTN firm_update(common::DB* db, TLV_HANDLE_DEV_FIRM& firm_handle);
	TLV_HANDLE_RTN gps_update(common::DB* db, TLV_HANDLE_GPS& gps_handle);
	TLV_HANDLE_RTN lbs_update(common::DB* db, TLV_HANDLE_LBS& lbs_handle);
	TLV_HANDLE_RTN warning_type_update(common::DB* db, const string& order_id, const string& ref_date, const int& position);
	TLV_HANDLE_RTN th_update(common::DB* db, TLV_HANDLE_TH& th_handle);
	TLV_HANDLE_RTN unband_update(common::DB* db, TLV_HANDLE_UNBAND_RTN& unband_handle);
	TLV_HANDLE_RTN dev_unband_update(common::DB* db, TLV_HANDLE_DEV_UNBAND_RTN& dev_unband_handle);
	TLV_HANDLE_RTN dev_msg_update(common::DB* db, TLV_HANDLE_DEV_MSG& dev_msg_handle);
	TLV_HANDLE_RTN parameter_update(common::DB* db, TLV_HANDLE_PARAMETER_RTN& parameter_handle);
	TLV_HANDLE_RTN version_update(common::DB* db, TLV_HANDLE_VERSION_RTN& ver_handle);
	TLV_HANDLE_RTN manage_lbs_update(common::DB* db, TLV_HANDLE_MANAGE_LBS& m_lbs_handle);
	TLV_HANDLE_RTN th2_update(common::DB* db, TLV_HANDLE_TH2& th_handle);
	TLV_HANDLE_RTN exposure_update(common::DB* db, TLV_HANDLE_EXPOSURE& exposure_handle);
	TLV_HANDLE_RTN shock_update(common::DB* db, TLV_HANDLE_SHOCK& shock_handle);
	TLV_HANDLE_RTN tilt_update(common::DB* db, TLV_HANDLE_TILT& tilt_handle);
	TLV_HANDLE_RTN baro_update(common::DB* db, TLV_HANDLE_BARO& baro_handle);
};

#endif