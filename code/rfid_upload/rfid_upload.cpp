/*
//
//	Last Modify Date: 2016-03-24
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息分析和入库
//
//
*/

#include "rfid_upload.h"

int main(int argc, char* argv[])
{
	common::Config cfg(argc, argv, "r:q:d:j:l:");

	if(!cfg.exist('r') || !cfg.exist('q') || !cfg.exist('d') || !cfg.exist('l'))
	{
		printf("formal  : rfid_upload -redis 6379 -queue rfid_upload -database iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -log path ../log/rfid_upload.log \n");
		printf("example : rfid_upload -r 6379 -q rfid_upload -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l ../log/rfid_upload.log \n");
		return 0;
	}

	common::Log::create(cfg.getString('l').c_str());

	//db decrypt
	//内网： iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf
	//外网： iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf
	string origin = common::unmix(cfg.getString('d'));

	RfidUploadHandle* rh = new RfidUploadHandle(origin);
	if(rh->db _EQ_ NULL)
	{
		LOG_DEBUG("initialize database error");
		return 0;
	}

	session::RedisQueueRFID* rq = new session::RedisQueueRFID("127.0.0.1", cfg.getInt('r'), rh);
	rq->run(cfg.getString('q'));

	LOG_DEBUG("program exit");

	return 0;
}
