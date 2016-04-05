/*
//
//	Last Modify Date: 2016-03-24
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
	common::Config cfg(argc, argv, "r:q:s:d:i:a:h:p:l:");

	if(!cfg.exist('r') || !cfg.exist('q') || !cfg.exist('s') || !cfg.exist('d') || !cfg.exist('i') || !cfg.exist('a') || !cfg.exist('h') || !cfg.exist('p') || !cfg.exist('l'))
	{
		printf("formal  : task -address 220.248.44.218 -heartbeat 21000 -interval 30(second) -port 22001 -redis upload 6379 -queue 1 -redis sending 6380 -database iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -log path ../log/task.log \n");
		printf("example : task -a 220.248.44.218 -h 21000 -i 30 -p 22001 -r 6379 -q 1 -s 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l ../log/task.log \n");
		return 0;
	}

	if(cfg.getInt('q') < 1 || cfg.getInt('q') > 9)
	{
		printf("redis queue range: 1~9 \n");
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

	th->mod = cfg.getInt('q');

	common::Thread* thread = new common::Thread;
	//heartbeat slave
	session::HeartBeat::create(cfg.getString('a').c_str(), cfg.getInt('h'), cfg.getInt('i'), cfg.getInt('p'), thread);
	//event
	session::Session* se = new session::Session(session::SESSION_TYPE_LISTENER, NULL, cfg.getInt('p'), thread, th);
	se->start();

	return thread->join();
}
