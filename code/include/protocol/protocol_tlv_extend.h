/*
//
//	Last Modify Date: 2016-09-26
//	Author: zengpw
//	History:
//					2016-09-21 zengpw created
//	File Property: public
//	Future:
//					I. tlv 子协议结构 - 自解析协议
//
//
*/

#ifndef _PROTOCOL_TLV_EXTEND_H_
#define _PROTOCOL_TLV_EXTEND_H_

#include "protocol_tlv_data.h"

using namespace std;

namespace protocol
{
	namespace tlv
	{
		#pragma pack(push, 1)
		//标准报文50 - TLV_BODY_FID
		typedef struct TLV_BODY_FID
		{
			int fid;
		}TLV_BODY_FID;

		//标准报文34 - TLV_JZT_DEV
		typedef struct TLV_JZT_DEV
		{
			unsigned short voltage;
			DateTime dt;
		}TLV_JZT_DEV;

		//标准报文35 - TLV_JZT_LBS
		typedef struct TLV_JZT_LBS
		{
			DateTime dt;
			LbsData ld[3];
		}TLV_JZT_LBS;

		//标准报文36 - TLV_JZT_TH2
		typedef struct TLV_JZT_TH2
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;
			short t0;
			short h0;
			short t1;
			short h1;
		}TLV_JZT_TH2;

		#pragma pack(pop)
	}// namespace tlv
};// namespace protocol

#endif