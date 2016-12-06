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

#ifndef _PROTOCOL_RFID_SYS_H_
#define _PROTOCOL_RFID_SYS_H_

#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/log.h"

using namespace std;

namespace protocol
{
	namespace rfid
	{
		#pragma pack(push, 1)

		enum RID
		{
			RID_SOF = 0x0002						//RS-232
		};

		typedef struct TLV_RFID
		{
			char type;
			unsigned char len;
			char* value;

			TLV_RFID()
			{
				value = NULL;
			}
		}TLV_RFID;
		typedef const TLV_RFID* TLV_RFID_PTR;

		typedef struct HeadRfid
		{
			unsigned char	sof;
			unsigned char	addr[2];
			unsigned char	len[2];
		}HeadRfid;
		typedef HeadRfid* HeadRfidPtr;

		typedef struct Tail
		{
			unsigned char	check[2];
			unsigned char	eof[2];
		}Tail;
		typedef Tail* TailPtr;

		#pragma pack(pop)
	}// namespace rfid
};// namespace protocol

#endif