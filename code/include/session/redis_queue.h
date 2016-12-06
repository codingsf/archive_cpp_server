/*
//
//	Last Modify Date: 2016-02-23
//	Author: zengpw
//	History:
//					2015-10-10 zengpw created
//	File Property: public
//	Future:
//					I. 封装从 redis 队列存取数据事件
//
//
*/

#ifndef _REDIS_QUEUE_H_
#define _REDIS_QUEUE_H_

#include "common/redis.h"
#include "common/log.h"
#include "handle_basic.h"

using namespace std;

namespace session
{
	class RedisQueue
	{
	public:
		//读取数据缓冲区设置为最大 10KB
		char buffer[10240];
		int len;
	public:
		string ip;
		unsigned int port;
	public:
		common::Redis* rc;
		HandleBasic* hb;
	public:
		//创建一个 redis 连接
		RedisQueue(const string ip, const unsigned int port);
		//创建一个 redis 连接，同时开启一个读取数据循环
		RedisQueue(const string ip, const unsigned int port, HandleBasic* hb);
	public:
		//开启 redis 连接
		common::Redis* open();
		//开启 redis 连接，并开启读取数据循环
		int run(const string& queue);
		//处理数据循环
		// repeat: =0 无数据时返回，=1 无数据时等待
		virtual int handle(const string& queue, const string& queue_redo, const int repeat);
	};
};// namespace session

#endif