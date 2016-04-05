/*
//
//	Last Modify Date: 2016-03-18
//	Author: zengpw
//	History:
//					2015-10-22 zengpw created
//	File Property: private
//	Future:
//					I. http 操作
//
//
*/

#ifndef _HTTP_INTERFACE_H_
#define _HTTP_INTERFACE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/py_call.h"
#include "common/log.h"
#include "protocol/protocol_tlv.h"

using namespace std;

namespace session
{
	class HttpInterface
	{
	public:
		common::PyCall python_call;
	public:
		bool lbs2gps(TLV_LBS& lbs, string& latitude, string& longitude, string& url);
		bool lbs2gps(TLV_MANAGE_LBS& m_lbs, string& latitude, string& longitude, string& url);
		bool lbs2gpsAmap(vector<LbsData> &vLbs, string& latitude, string& longitude, string& url);
		bool gps2addrAmap(const string& latitude, const string& longitude, string& addr);
		bool notifyJava(const string& java_addr, const string& dev_id, const string& type, const string& value, const string& other);
		bool notifyJavaCreateOrder(const string& java_addr, const string& dev_id);
		bool notifyJavaQueryFence(const string& java_addr, const string& dev_id, const string& latitude, const string& longitude, int& rtn);
	};
};// namespace session
#endif