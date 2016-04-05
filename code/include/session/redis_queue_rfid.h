/*
//
//	Last Modify Date: 2016-02-18
//	Author: zengpw
//	History:
//					2016-02-17 zengpw created
//	File Property: public
//	Future:
//					I. RedisQueue子类，用于处理 RFID报文事件循环
//
//
*/

#ifndef _REDIS_QUEUE_RFID_H_
#define _REDIS_QUEUE_RFID_H_

#include "common/redis.h"
#include "common/log.h"
#include "handle_basic.h"
#include "redis_queue.h"

using namespace std;

namespace session
{
	class RedisQueueRFID : public RedisQueue
	{
	public:
		//创建一个 redis 连接
		RedisQueueRFID(const string ip, const unsigned int port):RedisQueue(ip, port){}
		//创建一个 redis 连接，同时开启一个读取数据循环
		RedisQueueRFID(const string ip, const unsigned int port, HandleBasic* hb):RedisQueue(ip, port, hb){}
	public:
		//处理数据循环
		// repeat: =0 无数据时返回，=1 无数据时等待
		int handle(const string& queue, const string& queue_redo, const int repeat);
	};
};// namespace session

#endif