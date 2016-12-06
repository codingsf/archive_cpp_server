/*
//
//	Last Modify Date: 2016-08-26
//	Author: zengpw
//	History:
//					2015-11-04 zengpw created
//	File Property: private
//	Future:
//					I. 数据库操作
//
//
*/

#ifndef _DB_PUBLIC_H_
#define _DB_PUBLIC_H_

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
#include "protocol/protocol_tlv.h"

using namespace std;

namespace session
{
	typedef struct TLV_HANDLE_FENCE
	{
		string dev_id;
		string order_id;
		string latitude;
		string longitude;
		string datetime;
	}TLV_HANDLE_FENCE;

	class DbPublic
	{
	public:
		bool public_get_order_id(common::DB* db, const string& dev_id, string& order_id);
		bool public_get_protocol_ver(common::DB* db, const string& dev_id, string& protocol_ver);
		bool public_get_parameter_request(common::DB* db, const string& dev_id, TLV_PARAMETER& para);
		bool public_get_parameter_request_ver11(common::DB* db, const string& dev_id, TLV_PARAMETER_V11& para);
		bool public_get_parameter_request_ver13(common::DB* db, const string& dev_id, TLV_PARAMETER_V13& para);
		bool public_get_parameter_request_ver14(common::DB* db, const string& dev_id, TLV_PARAMETER_V14& para);
		bool public_get_parameter_sending(common::DB* db, const string& dev_id, TLV_PARAMETER& para);
		bool public_get_parameter_sending_ver11(common::DB* db, const string& dev_id, TLV_PARAMETER_V11& para);
		bool public_get_parameter_sending_ver13(common::DB* db, const string& dev_id, TLV_PARAMETER_V13& para);
		bool public_get_parameter_sending_ver14(common::DB* db, const string& dev_id, TLV_PARAMETER_V14& para);
		bool public_get_version(common::DB* db, const string& dev_id, TLV_PARAMETER_VERSION& ver);
		bool public_get_order_type(common::DB* db, const string& dev_id, int& order_type);
		bool timing_get_dev_id(common::DB* db, vector<TLV_HANDLE_FENCE> &vecThf);
		bool timing_get_location(common::DB* db, TLV_HANDLE_FENCE& thf);
		bool public_get_start(common::DB* db, const string& dev_id, TLV_TIME_6& time_6, TLV_EXPRESS_ID& express_id);
		bool public_get_end(common::DB* db, const string& dev_id, TLV_TIME_6& para);
	};
};// namespace session

#endif