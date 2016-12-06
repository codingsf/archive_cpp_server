/*
//
//	Last Modify Date: 2016-07-28
//	Author: zengpw
//	History:
//					2016-01-11 zengpw created
//	File Property: private
//	Future:
//					I. 下发 redis 操作
//
//
*/

#include "redis_public.h"

namespace session
{
	int RedisPublic::public_insert_command(common::Redis* rc, const string& dev_id, const int type, const string& live_time, string& hint)
	{
		LOG_DEBUG("=========== public_insert_command: dev_id = " << dev_id << ", type = " << type);

		// 解绑 3
		// 运行参数下发 6
		// 版本升级 8
		// 初始化 9
		// 电子围栏 - 进入 11
		// 电子围栏 - 离开 12
		if(type == 3 || type == 6 || type == 8 || type == 9 || type == 11 || type == 12 || type == 23 || type == 26)
		{
			string key = dev_id + "_";

			string id;
			stringstream ss;
			ss << type;
			id = ss.str();
			ss.str("");
			ss.clear();

			key += id;

			int rtn = public_remove_command(rc, dev_id, type, "2592000", hint);

			if(rtn != 0)
			{
				LOG_DEBUG("insert redis occur error, hint = " << hint);
			}

			LOG_DEBUG("----------- insert key " << key);

			if(rc->redis_select(0) != 0)
			{
				hint = "redis select error!";
				return -2;
			}

			//调用插入函数
			string begin = common::buildDateTime();

			if(rc->redis_hmset4sending(key.c_str(), begin.c_str(), "0", "0") != 0)
			{
				hint = "redis insert error!";
				return -3;
			}
			else
			{
				rc->redis_expire(key.c_str(), live_time.c_str());
				hint = "success!";
				return 0;
			}
		}
		else
		{
			hint = "command type error!";
			return -1;
		}

		return 0;
	}


	int RedisPublic::public_remove_command(common::Redis* rc, const string& dev_id, const int type, const string& live_time, string& hint)
	{
		LOG_DEBUG("=========== public_remove_command: dev_id = " << dev_id << ", type = " << type);

		string key = dev_id + "_";

		string id;
		stringstream ss;
		ss << type;
		id = ss.str();
		ss.str("");
		ss.clear();

		key += id;

		if(rc->redis_select(0) != 0)
		{
			hint = "redis select error!";
			return -2;
		}

		//处理未下发命令
		int rtn = rc->redis_exists(key.c_str());

		if(rtn == 1)
		{
			if(rc->redis_move(key.c_str(), 1) != 0)
			{
				hint = "redis move error!";
				return -4;
			}
			else
			{
				if(rc->redis_select(1) != 0)
				{
					hint = "redis select error!";
					return -2;
				}

				//修改 key name
				string new_key = key + "_";
				new_key += common::buildDateTime();

				if(rc->redis_rename(key.c_str(), new_key.c_str()) == 0)
				{
					//修改 sts ，修改 end time
					// sts =0 初始状态； =1 下发成功； =2 未下发，被新的命令覆盖
					string end = common::buildDateTime();
					if(rc->redis_hmset4update(new_key.c_str(), end.c_str(), "2") != 0)
					{
						hint = "redis update error!";
						return -6;
					}

					//设置失效时间
					rc->redis_expire(new_key, live_time.c_str());
				}
				else
				{
					hint = "redis rename error!";
					return -5;
				}

				if(rc->redis_select(0) != 0)
				{
					hint = "redis select error!";
					return -2;
				}
			}
		}

		//处理已下发命令
		string new_key = key + "_*";
		vector<string> vs;

		if(rc->redis_keys(new_key, vs) == 0)
		{
			for(unsigned int i = 0; i < vs.size(); i++)
			{
				if(rc->redis_move(vs[i].c_str(), 1) != 0)
				{
					hint = "redis move error!";
					return -4;
				}
				else
				{
					if(rc->redis_select(1) != 0)
					{
						hint = "redis select error!";
						return -2;
					}

					//设置失效时间
					rc->redis_expire(vs[i], live_time.c_str());

					if(rc->redis_select(0) != 0)
					{
						hint = "redis select error!";
						return -2;
					}
				}
			}
		}

		return 0;
	}


	int RedisPublic::public_del_command(common::Redis* rc, const string& dev_id, const int type, string& hint)
	{
		LOG_DEBUG("=========== public_del_command: dev_id = " << dev_id << ", type = " << type);

		string key = dev_id + "_";

		string id;
		stringstream ss;
		ss << type;
		id = ss.str();
		ss.str("");
		ss.clear();

		key += id;

		if(rc->redis_select(0) != 0)
		{
			hint = "redis select error!";
			return -2;
		}

		//处理未下发命令
		int rtn = rc->redis_exists(key.c_str());

		if(rtn == 1)
		{
			rc->redis_del(key.c_str());
		}

		//处理已下发命令
		string new_key = key + "_*";
		vector<string> vs;

		if(rc->redis_keys(new_key, vs) == 0)
		{
			for(unsigned int i = 0; i < vs.size(); i++)
			{
				rc->redis_del(vs[i].c_str());
			}
		}

		return 0;
	}
};// namespace session