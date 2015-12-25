/*
//
//	Last Modify Date: 2015-12-25
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
	common::Config cfg(argc, argv, "h:p:r:d:l:");
	if(!cfg.exist('h') || !cfg.exist('p') || !cfg.exist('r') || !cfg.exist('d') || !cfg.exist('l'))
	{
		printf("formal  : balance -heartbeat 21000 -port 22000 -redis upload 6379 -database coglink2/coglink2013@CLORCL -log path ../log/balance.log \n");
		printf("example : balance -h 21000 -p 22000 -r 6379 -d coglink2/coglink2013@CLORCL -l ../log/balance.log \n");
		return 0;
	}

	common::Log::create(cfg.getString('l').c_str());

	BalanceHandle* bh = new BalanceHandle(cfg.getString('d'));
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

	common::Thread* thread = new common::Thread;
	// heartbeat master
	session::HeartBeat::create(cfg.getInt('h'), thread);
	//event
	session::Session* se = new session::Session(session::SESSION_TYPE_LISTENER, NULL, cfg.getInt('p'), thread, bh);
	se->start();

	return thread->join();
}
