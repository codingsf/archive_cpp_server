/*
//
//	Last Modify Date: 2016-01-26
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
		bool public_get_parameter_request(common::DB* db, const string& dev_id, TLV_PARAMETER& para);
		bool public_get_parameter_sending(common::DB* db, const string& dev_id, TLV_PARAMETER& para);
		bool public_get_version(common::DB* db, const string& dev_id, TLV_PARAMETER_VERSION& ver);
		bool public_get_order_type(common::DB* db, const string& dev_id, int& order_type);
		bool timing_get_dev_id(common::DB* db, vector<TLV_HANDLE_FENCE> &vecThf);
		bool timing_get_location(common::DB* db, TLV_HANDLE_FENCE& thf);
	};
};// namespace session

#endif