/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-04 zengpw created
//	File Property: public
//	Future:
//					I. RFID RS232上传协议
//
//
*/

#ifndef _PROTOCOL_RFID_DATA_H_
#define _PROTOCOL_RFID_DATA_H_

#include "protocol_rfid_sys.h"

using namespace std;

namespace protocol
{
	namespace rfid
	{
		#pragma pack(push, 1)

		//标准报文 0x02
		typedef struct TLV_RFID_RS232
		{
			unsigned char type[2];
			unsigned char rfid[8];
			unsigned char io[10];
		}TLV_RFID_RS232;

		#pragma pack(pop)
	}// namespace rfid
};// namespace protocol

#endif