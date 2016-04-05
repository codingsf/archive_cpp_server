/*
//
//	Last Modify Date: 2016-01-19
//	Author: zengpw
//	History:
//					2016-01-11 zengpw created
//	File Property: private
//	Future:
//					I. 下发 redis 操作
//
//
*/

#ifndef _REDIS_PUBLIC_H_
#define _REDIS_PUBLIC_H_

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
#include "common/redis.h"
#include "common/log.h"

using namespace std;

namespace session
{
	class RedisPublic
	{
	public:
		int public_insert_command(common::Redis* rc, const string& dev_id, const int type, const string& live_time, string& hint);
		int public_remove_command(common::Redis* rc, const string& dev_id, const int type, const string& live_time, string& hint);
		int public_del_command(common::Redis* rc, const string& dev_id, const int type, string& hint);
	};
};// namespace session

#endif