/*
//
//	Last Modify Date: 2016-03-24
//	Author: zengpw
//	History:
//					2016-01-12 zengpw created
//	File Property: private
//	Future:
//					I. 处理一些存在时间要求的小任务
//
//
*/

#include "timing.h"

int main(int argc, char* argv[])
{
	common::Config cfg(argc, argv, "r:d:j:l:");

	if(!cfg.exist('r') || !cfg.exist('d') || !cfg.exist('j') || !cfg.exist('l'))
	{
		printf("formal  : timing -redis 6380 -database iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -java_addr 192.168.1.88:8888 -log path ../log/timing.log \n");
		printf("example : timing -r 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -j 192.168.1.88:8888 -l ../log/timing.log \n");
		return 0;
	}

	common::Log::create(cfg.getString('l').c_str());

	if(!common::PyCall::Initialize())
	{
		LOG_DEBUG("Initialize python script fail, py_http.py is not in current path!");
		return 0;
	}

	LOG_DEBUG("load python script success.");

	//db decrypt
	//内网： iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf
	//外网： iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf
	string origin = common::unmix(cfg.getString('d'));

	TimingImpl* ti = new TimingImpl(origin, cfg.getString('j'));
	if(ti->db == NULL)
	{
		LOG_DEBUG("initialize database error");
		return 0;
	}

	session::RedisQueue* rq = new session::RedisQueue("127.0.0.1", cfg.getInt('r'));
	ti->rc = rq->open();

	if(ti->rc == NULL)
	{
		LOG_DEBUG("connect redis error");
		return 0;
	}

	ti->run();

	common::PyCall::Finalize();
	LOG_DEBUG("program exit");

	return 0;
}
