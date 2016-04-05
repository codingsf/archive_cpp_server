/*
//
//	Last Modify Date: 2016-02-17
//	Author: zengpw
//	History:
//					2016-02-04 zengpw created
//	File Property: public
//	Future:
//					I. RFID RS232上传协议
//
//
*/

#ifndef _PROTOCOL_RFID_H_
#define _PROTOCOL_RFID_H_

#include <sys/time.h>
#include "common/log.h"
#include "common/util.h"
#include "protocol_rfid_sys.h"
#include "protocol_rfid_data.h"

using namespace std;
using namespace protocol::rfid;

namespace protocol
{
	class PacketRFID
	{
	public:
		//读取数据缓冲区设置为最大 10KB
		char data[10240];
		unsigned int len;
	public:
		HeadRfidPtr ptrHead;
		TLV_RFID     tlv;
		TailPtr ptrTail;
	public:
		PacketRFID();
		int buildFromString(const char* value, const unsigned int len);
	public:
		char* setHead();
		char* setBody();
		int parseHead();
		int parseBody();
		unsigned char checkSum(unsigned char* uBuff, unsigned char uBuffLen);
	public:
		void printDetail();
		const char* getMsg() const;
		unsigned short getMsgLen() const;
		unsigned char getSOF() const;
		TLV_RFID_PTR getTLV() const;
	};
};// namespace protocol

#endif