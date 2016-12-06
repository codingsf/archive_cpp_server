/*
//
//	Last Modify Date: 2016-09-27
//	Author: zengpw
//	History:
//					2015-10-09 zengpw created
//	File Property: private
//	Future:
//					I. 消息分析和入库
//
//
*/

#ifndef _UPLOAD_H_
#define _UPLOAD_H_

#include "common/py_call.h"
#include "common/thread.h"
#include "common/process.h"
#include "common/net.h"
#include "common/log.h"
#include "session/session.h"
#include "session/redis_queue.h"

#ifdef JZT
#include "upload_jzt_handle.h"
#else
#include "upload_handle.h"
#endif

using namespace std;

int normal(int argc, char* argv[]);
int jzt(int argc, char* argv[]);

#endif