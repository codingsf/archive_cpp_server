/*
//
//	Last Modify Date: 2016-01-13
//	Author: zengpw
//	History:
//					2016-01-12 zengpw created
//	File Property: private
//	Future:
//					I. 处理一些存在时间要求的小任务
//
//
*/

#ifndef _TIMING_IMPL_H_
#define _TIMING_IMPL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/py_call.h"
#include "common/thread.h"
#include "common/process.h"
#include "common/log.h"
#include "session/db_public.h"
#include "session/redis_public.h"
#include "session/http_interface.h"
#include "session/session.h"
#include "session/redis_queue.h"

using namespace std;

class TimingImpl
{
public:
	common::DB* db;
	string java_addr;
	common::Redis* rc;
public:
	explicit TimingImpl(string db_conn_info, string java_addr);
public:
	void run();
};

#endif