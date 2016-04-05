/*
//
//	Last Modify Date: 2016-03-24
//	Author: zengpw
//	History:
//					2015-08-12 zengpw created
//	File Property: private
//	Future:
//					I. 负载均衡
//
//
*/

#include "balance.h"

int main(int argc, char* argv[])
{
	common::Config cfg(argc, argv, "h:p:r:q:d:l:");
	if(!cfg.exist('h') || !cfg.exist('p') || !cfg.exist('r') || !cfg.exist('q') || !cfg.exist('d') || !cfg.exist('l'))
	{
		printf("formal  : balance -heartbeat 21000 -port 22000 -redis upload 6379 -queue 1 -database iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -log path ../log/balance.log \n");
		printf("example : balance -h 21000 -p 22000 -r 6379 -q 1 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l ../log/balance.log \n");
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

	BalanceHandle* bh = new BalanceHandle(origin);
	if(bh->db _EQ_ NULL)
	{
		LOG_DEBUG("initialize database error");
		return 0;
	}

	session::RedisQueue* rc = new session::RedisQueue("127.0.0.1", cfg.getInt('r'));
	bh->rc = rc->open();
	if(bh->rc == NULL)
	{
		LOG_DEBUG("connect redis error");
		return 0;
	}

	bh->mod = cfg.getInt('q');

	common::Thread* thread = new common::Thread;
	// heartbeat master
	session::HeartBeat::create(cfg.getInt('h'), thread);
	//event
	session::Session* se = new session::Session(session::SESSION_TYPE_LISTENER, NULL, cfg.getInt('p'), thread, bh);
	se->start();

	return thread->join();
}
