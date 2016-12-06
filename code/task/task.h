/*
//
//	Last Modify Date: 2016-09-27
//	Author: zengpw
//	History:
//					2015-09-02 zengpw created
//	File Property: private
//	Future:
//					I. 消息预处理
//
//
*/

#ifndef _TASK_H_
#define _TASK_H_

#include "common/thread.h"
#include "common/process.h"
#include "common/net.h"
#include "common/log.h"
#include "session/heartbeat.h"
#include "session/session.h"
#include "session/redis_queue.h"

#ifdef JZT
#include "task_jzt_handle.h"
#else
#include "task_handle.h"
#endif

using namespace std;

int normal(int argc, char* argv[]);
int jzt(int argc, char* argv[]);

#endif