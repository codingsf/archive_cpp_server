/*
//
//	Last Modify Date: 2015-10-16
//	Author: zengpw
//	History:
//					2015-09-09 zengpw created
//	File Property: public
//	Future:
//					I. 基于 google protocol buffer 协议
//
//
*/

#include "protocol_pb.h"

namespace protocol
{
	namespace pb
	{
		char* PacketPB::setHead()
		{
			memset(data, 0, sizeof(data));
			return data;
		}


		char* PacketPB::setBody(unsigned int length)
		{
			return data + sizeof(Head);
		}


		int PacketPB::parseHead()
		{
			// LOG_DEBUG("PB parseHead : ");
			// common::printByHex(data, sizeof(Head));

			ptrHead = (HeadPtr)data;

			if(ptrHead->xcode[0] != 0xA1 || ptrHead->xcode[1] != 0xB2 || ptrHead->xcode[2] != 0xC3)
			{
				return -1;
			}

			len = sizeof(Head);

			return ptrHead->len;
		}


		int PacketPB::parseBody(string& strPb)
		{
			// LOG_DEBUG("PB parseBody : ");
			// common::printByHex(&data[sizeof(Head)], ptrHead->len);

			strPb.assign(&data[sizeof(Head)], ptrHead->len - sizeof(crc));
			crc = (unsigned short)data[sizeof(Head) + ptrHead->len - 2];

			// TODO 校验 CRC

			len += ptrHead->len;

			printDetail();

			return 0;
		}


		void PacketPB::build(string& strPb, unsigned int length)
		{
			Head h;

			h.xcode[0] = 0xA1;
			h.xcode[1] = 0xB2;
			h.xcode[2] = 0xC3;
			h.len = length + sizeof(crc);

			len = sizeof(Head) + h.len;

			memset(data, 0, sizeof(data));

			memcpy(data, (char*)&h, sizeof(Head));
			ptrHead = (HeadPtr)data;

			memcpy(data + sizeof(Head), strPb.c_str(), length);

			crc = protocol::createCRC(data, len - sizeof(crc));
			memcpy(data + sizeof(Head) + length, (char*)&crc, sizeof(crc));

			printDetail();
		}


		const char* PacketPB::getMsg()
		{
			return data;
		}


		unsigned int PacketPB::getMsgLen()
		{
			return len;
		}


		void PacketPB::printDetail()
		{
			// LOG_DEBUG("PB All Data : ");
			// common::printByHex(data, len);

			// LOG_DEBUG("PB Head : ");
			// common::printByHex(data, sizeof(Head));

			// LOG_DEBUG("PB Body : ");
			// common::printByHex(data + sizeof(Head), ptrHead->len - sizeof(crc));

			// LOG_DEBUG("PB CRC : ");
			// common::printByHex(data + sizeof(Head) + ptrHead->len - sizeof(crc), sizeof(crc));
		}
	}// namespace pb
};// namespace protocol
