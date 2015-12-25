/*
//
//	Last Modify Date: 2015-12-18
//	Author: zengpw
//	History:
//					2015-08-12 zengpw created
//	File Property: public
//	Future:
//					I. 部分工具
//
//
*/

#ifndef _COMMON_UTIL_H_
#define _COMMON_UTIL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <set>
#include <time.h>
#include <iostream>
#include <string>
#include <limits>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "log.h"

using namespace std;

namespace common
{
	//字符串拆分
	typedef set<string> StringSet;
	void split(const string& in, const string delimiter, StringSet& out);

	// 16进制打印
	char hex2char(int hex);
	string printByHex(const char* message, const int length);
	string printByHex(const unsigned char* message, const int length);

	//进制转换
	string int2binary(const int i, const int reverse = 0);
	string uchar2binary(const unsigned char i);
	int binary2int(const string& str);

	//生成当前时间
	string buildDateTime();

	//生成当前时间（秒数）
	long long buildDateTimeSecond();
};// namespace common

#endif
