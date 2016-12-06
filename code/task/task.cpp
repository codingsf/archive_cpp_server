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

#include "task.h"

int main(int argc, char* argv[])
{
#ifdef JZT
	return jzt(argc, argv);
#else
	return normal(argc, argv);
#endif
}

int normal(int argc, char* argv[])
{
	common::Config cfg(argc, argv, "r:s:d:i:a:h:p:l:k:x:y:z:");

	if(!cfg.exist('r') || !cfg.exist('s') || !cfg.exist('d') || !cfg.exist('i') || !cfg.exist('a') || !cfg.exist('h') || !cfg.exist('p') || !cfg.exist('l') || !cfg.exist('k') || !cfg.exist('x') || !cfg.exist('y') || !cfg.exist('z'))
	{
		printf("formal  : task -address 220.248.44.218 -heartbeat 21000 -interval 30(second) -port 22001 -redis upload 6379 -queue key 1 -queue x 1 -queue y 1 -queue z 1 -redis sending 6380 -database iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -log path ../log/task.log \n");
		printf("example : task -a 220.248.44.218 -h 21000 -i 30 -p 22001 -r 6379 -k 1 -x 1 -y 1 -z 1 -s 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l ../log/task.log \n");
		return 0;
	}

	if(cfg.getInt('x') < 1 || cfg.getInt('x') > 30)
	{
		printf("redis queue range: 1~30 \n");
		return 0;
	}

	if(cfg.getInt('y') < 1 || cfg.getInt('y') > 30)
	{
		printf("redis queue range: 1~30 \n");
		return 0;
	}

	if(cfg.getInt('z') < 1 || cfg.getInt('z') > 30)
	{
		printf("redis queue range: 1~30 \n");
		return 0;
	}

	common::Log::create(cfg.getString('l').c_str());

	//db decrypt
	//内网： iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf
	//外网： iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf
	string origin = common::unmix(cfg.getString('d'));

	TaskHandle* th = new TaskHandle(origin);
	if(th->db _EQ_ NULL)
	{
		LOG_DEBUG("initialize database error");
		return 0;
	}

	session::RedisQueue* rq_upload = new session::RedisQueue("127.0.0.1", cfg.getInt('r'));
	session::RedisQueue* rq_sending = new session::RedisQueue("127.0.0.1", cfg.getInt('s'));

	th->rc_upload = rq_upload->open();
	th->rc_sending = rq_sending->open();

	if(th->rc_upload == NULL || th->rc_sending == NULL)
	{
		LOG_DEBUG("connect redis error");
		return 0;
	}

	th->key = cfg.getInt('k');
	th->mod_x = cfg.getInt('x');
	th->mod_y = cfg.getInt('y');
	th->mod_z = cfg.getInt('z');

	common::Thread* thread = new common::Thread;
	//heartbeat slave
	session::HeartBeat::create(cfg.getString('a').c_str(), cfg.getInt('h'), cfg.getInt('i'), cfg.getInt('p'), thread);
	//event
	session::Session* se = new session::Session(session::SESSION_TYPE_LISTENER, NULL, cfg.getInt('p'), thread, th);
	se->start();

	return thread->join();
}


int jzt(int argc, char* argv[])
{
#ifdef JZT
	common::Config cfg(argc, argv, "m:p:r:s:d:l:k:x:y:z:");

	if(!cfg.exist('m') || !cfg.exist('p') || !cfg.exist('r') || !cfg.exist('s') || !cfg.exist('l') || !cfg.exist('k') || !cfg.exist('x') || !cfg.exist('y') || !cfg.exist('z'))
	{
		return 0;
	}

	int mode = cfg.getInt('m');

	if(cfg.getInt('x') < 1 || cfg.getInt('x') > 30)
	{
		printf("redis queue range: 1~30 \n");
		return 0;
	}

	if(cfg.getInt('y') < 1 || cfg.getInt('y') > 30)
	{
		printf("redis queue range: 1~30 \n");
		return 0;
	}

	if(cfg.getInt('z') < 1 || cfg.getInt('z') > 30)
	{
		printf("redis queue range: 1~30 \n");
		return 0;
	}

	common::Log::create(cfg.getString('l').c_str());

	TaskHandle* th;

	if(mode == 0)
	{
		//db decrypt
		//内网： iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf
		//外网： iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf
		string origin = common::unmix(cfg.getString('d'));
		th = new TaskHandle(origin);
		if(th->db _EQ_ NULL)
		{
			LOG_DEBUG("initialize database error");
			return 0;
		}
	}
	else
	{
		th = new TaskHandle();
	}

	th->mode = mode;

	session::RedisQueue* rq_upload = new session::RedisQueue("127.0.0.1", cfg.getInt('r'));
	session::RedisQueue* rq_sending = new session::RedisQueue("127.0.0.1", cfg.getInt('s'));

	th->rc_upload = rq_upload->open();
	th->rc_sending = rq_sending->open();

	if(th->rc_upload == NULL || th->rc_sending == NULL)
	{
		LOG_DEBUG("connect redis error");
		return 0;
	}

	th->key = cfg.getInt('k');
	th->mod_x = cfg.getInt('x');
	th->mod_y = cfg.getInt('y');
	th->mod_z = cfg.getInt('z');

	common::Thread* thread = new common::Thread;
	//event
	session::Session* se = new session::Session(session::SESSION_TYPE_LISTENER, NULL, cfg.getInt('p'), thread, th);
	se->start();

	return thread->join();
#endif
	return 0;
}











