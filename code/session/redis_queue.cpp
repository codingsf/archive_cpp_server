/*
//
//	Last Modify Date: 2016-03-28
//	Author: zengpw
//	History:
//					2015-10-10 zengpw created
//	File Property: public
//	Future:
//					I. 封装从 redis 队列存取数据事件
//
//
*/

#include "redis_queue.h"

namespace session
{
	RedisQueue::RedisQueue(const string ip, const unsigned int port)
	{
		rc = NULL;
		this->ip = ip;
		this->port = port;
	}

	RedisQueue::RedisQueue(const string ip, const unsigned int port, HandleBasic* hb)
	{
		rc = NULL;
		this->ip = ip;
		this->port = port;
		this->hb = hb;
	}

	common::Redis* RedisQueue::open()
	{
		rc = new common::Redis(ip, port);
		if(rc->redis_conn() != 0)
		{
			LOG_DEBUG("RedisQueue : open() error!");
			delete rc;
			rc = NULL;
		}
		return rc;
	}

	int RedisQueue::run(const string& queue)
	{
		string queue_redo = queue + "_redo";
		string queue_error = queue + "_error";

		if(open() == NULL)
			return 0;

		// 1st, 处理上次的错误数据
		handle(queue_redo, queue_error, 0);

		// 2nd, 处理正常数据
		return handle(queue, queue_redo, 1);
	}

	int RedisQueue::handle(const string& queue, const string& queue_redo, const int repeat)
	{
		LOG_DEBUG("get data from " << queue << "!");

		for(;;)
		{
			memset(buffer, 0, sizeof(buffer));
			len = 0;

			int rtn = rc->redis_rpoplpush(queue.c_str(), queue_redo.c_str(), buffer, &len);

			if(rtn == -1)
			{
				LOG_DEBUG("RedisQueue : get data from " << queue << " error!");
				return -1;
			}

			if(rtn == 1 && repeat == 0)
			{
				break;
			}

			if(rtn == 1 && repeat == 1)
			{
				usleep(5*1000*1000);
			}

			if(rtn == 0)
			{
				protocol::PacketIn *in = new protocol::PacketIn;
				protocol::PacketOut *out = new protocol::PacketOut;

				int rt = hb->dispatch_origin_msg(buffer, len, in, out);

				if(rt == HANDLE_RTN_CONTINUE)
				{
					LOG_DEBUG("handle redis queue " << queue << " error!");

					delete in;
					delete out;
					continue;
				}

				if(rt == HANDLE_RTN_CLOSE)
				{
					string queue_close = queue + "_close";

					//插入 close队列
					if(rc->redis_lpush(queue_close.c_str(), buffer, len) != 0)
					{
						LOG_DEBUG("redis push occur serious error, please check redis!");
						return 0;
					}

					//从 redo删除
					rc->redis_lrem(queue_redo.c_str(), buffer, len);

					delete in;
					delete out;
					continue;
				}

				if(rt == HANDLE_RTN_REMOVE)
				{
					rc->redis_lrem(queue_redo.c_str(), buffer, len);

					delete in;
					delete out;
					continue;
				}

				if(rt == HANDLE_RTN_STOP)
				{
					delete in;
					delete out;
					return 0;
				}
			}
		}

		return 0;
	}
};// namespace session