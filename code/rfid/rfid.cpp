/*
//
//	Last Modify Date: 2016-02-18
//	Author: zengpw
//	History:
//					2016-02-18 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息预处理
//
//
*/

#include "rfid.h"

int main(int argc, char* argv[])
{
	common::Config cfg(argc, argv, "r:q:p:l:");

	if(!cfg.exist('r') || !cfg.exist('q') || !cfg.exist('p') || !cfg.exist('l'))
	{
		printf("formal  : rfid -port 22501 -redis upload 6379 -queue 1 -log path ../log/rfid.log \n");
		printf("example : rfid -p 22501 -r 6379 -q 1 -l ../log/rfid.log \n");
		return 0;
	}

	if(cfg.getInt('q') < 1 || cfg.getInt('q') > 9)
	{
		printf("redis queue range: 1~9 \n");
		return 0;
	}

	common::Log::create(cfg.getString('l').c_str());

	RfidHandle* rh = new RfidHandle();
	session::RedisQueueRFID* rq_upload = new session::RedisQueueRFID("127.0.0.1", cfg.getInt('r'));

	rh->rc_upload = rq_upload->open();
	if(rh->rc_upload == NULL)
	{
		LOG_DEBUG("connect redis error");
		return 0;
	}

	rh->mod = cfg.getInt('q');

	//event
	session::SessionRFID* se = new session::SessionRFID(session::SESSION_TYPE_LISTENER, NULL, cfg.getInt('p'), NULL, rh);
	se->startNoThread();

	LOG_DEBUG("program exit");

	return 0;
}
