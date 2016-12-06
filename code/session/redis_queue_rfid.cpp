/*
//
//	Last Modify Date: 2016-02-17
//	Author: zengpw
//	History:
//					2016-02-17 zengpw created
//	File Property: public
//	Future:
//					I. RedisQueue子类，用于处理 RFID报文事件循环
//
//
*/

#include "redis_queue_rfid.h"

namespace session
{
	int RedisQueueRFID::handle(const string& queue, const string& queue_redo, const int repeat)
	{
		LOG_DEBUG("get data from " << queue << "!");

		for(;;)
		{
			memset(buffer, 0, sizeof(buffer));
			len = 0;

			int rtn = rc->redis_rpoplpush(queue.c_str(), queue_redo.c_str(), buffer, &len);

			if(rtn == -1)
			{
				LOG_DEBUG("RedisQueueRFID : get data from " << queue << " error!");
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
				protocol::PacketRFID *in = new protocol::PacketRFID;

				int rt = hb->dispatch_rfid_origin_msg(buffer, len, in);

				if(rt == HANDLE_RTN_CONTINUE)
				{
					LOG_DEBUG("handle redis queue " << queue << " error!");

					delete in;
					continue;
				}

				if(rt == HANDLE_RTN_REMOVE)
				{
					rc->redis_lrem(queue_redo.c_str(), buffer, len);

					delete in;
					continue;
				}

				if(rt == HANDLE_RTN_STOP)
				{
					delete in;
					return 0;
				}
			}
		}

		return 0;
	}
};// namespace session