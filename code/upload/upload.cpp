/*
//
//	Last Modify Date: 2016-03-24
//	Author: zengpw
//	History:
//					2015-10-09 zengpw created
//	File Property: private
//	Future:
//					I. 消息分析和入库
//
//
*/

#include "upload.h"

int main(int argc, char* argv[])
{
	common::Config cfg(argc, argv, "r:q:d:j:l:");

	if(!cfg.exist('r') || !cfg.exist('q') || !cfg.exist('d') || !cfg.exist('j') || !cfg.exist('l'))
	{
		printf("formal  : upload -redis 6379 -queue msg_upload -database iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -java_addr 192.168.1.88:8888 -log path ../log/upload.log \n");
		printf("example : upload -r 6379 -q msg_upload -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -j 192.168.1.88:8888 -l ../log/upload.log \n");
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

	UploadHandle* uh = new UploadHandle(origin);
	if(uh->db _EQ_ NULL)
	{
		LOG_DEBUG("initialize database error");
		return 0;
	}
	uh->setJavaAddr(cfg.getString('j'));

	session::RedisQueue* rq = new session::RedisQueue("127.0.0.1", cfg.getInt('r'), uh);
	rq->run(cfg.getString('q'));

	common::PyCall::Finalize();
	LOG_DEBUG("program exit");

	return 0;
}
