/*
//
//	Last Modify Date: 2016-01-19
//	Author: zengpw
//	History:
//					2016-01-12 zengpw created
//	File Property: private
//	Future:
//					I. 处理一些存在时间要求的小任务
//
//
*/

#include "timing_impl.h"

TimingImpl::TimingImpl(string db_conn_info, string java_addr)
{
	db = new common::DB(db_conn_info);

	if(db->getConn() == NULL)
	{
		delete db;
		db = NULL;
		LOG_DEBUG("connect database error : " << db_conn_info);
	}

	this->java_addr = java_addr;
}


void TimingImpl::run()
{
	session::DbPublic dp;
	session::RedisPublic rp;

	for(;;)
	{
		LOG_DEBUG("================ search device from database ================");

		vector<session::TLV_HANDLE_FENCE> vecThf;
		vector<session::TLV_HANDLE_FENCE>::iterator iter;

		if(dp.timing_get_dev_id(db, vecThf))
		{
			LOG_DEBUG("vecThf->size = " << vecThf.size());

			for(iter = vecThf.begin(); iter != vecThf.end(); ++iter)
			{
				LOG_DEBUG("dev_id = " << iter->dev_id << ", order_id = " << iter->order_id);

				if(dp.timing_get_location(db, *iter))
				{
					LOG_DEBUG("dev_id = " << iter->dev_id << \
							", order_id = " << iter->order_id << \
							", latitude = " << iter->latitude << \
							", longitude = " << iter->longitude << \
							", datetime = " << iter->datetime);

					int rtn;

					session::HttpInterface hi;
					if(hi.notifyJavaQueryFence(java_addr, iter->dev_id, iter->latitude, iter->longitude, rtn))
					{
						string hint = "no hint";

						if(rp.public_del_command(rc, iter->dev_id, 11, hint) != 0)
						{
							// LOG_DEBUG 是有2行代码的宏，所以一定要写在花括号里
							LOG_DEBUG("delete redis error, hint = " << hint);
						}

						if(rp.public_del_command(rc, iter->dev_id, 12, hint) != 0)
						{
							LOG_DEBUG("delete redis error, hint = " << hint);
						}

						if(rtn == 1)
						{
							if(rp.public_insert_command(rc, iter->dev_id, 11, "3600", hint) != 0)
							{
								LOG_DEBUG("insert redis error, hint = " << hint);
							}
						}

						if(rtn == 0)
						{
							if(rp.public_insert_command(rc, iter->dev_id, 12, "3600", hint) != 0)
							{
								LOG_DEBUG("insert redis error, hint = " << hint);
							}
						}
					}
				}
			}

			LOG_DEBUG("sleep 4 minute!");
			sleep(60*4);
		}
		else
		{
			LOG_DEBUG("sleep 1 minute!");
			sleep(60*1);
		}
	}
}