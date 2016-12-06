/*
//
//	Last Modify Date: 2016-02-24
//	Author: zengpw
//	History:
//					2016-02-04 zengpw created
//	File Property: public
//	Future:
//					I. RFID RS232上传协议
//
//
*/

#include "protocol_rfid.h"

namespace protocol
{
	PacketRFID::PacketRFID()
	{
		len = 0;
		memset(data, 0, sizeof(data));

		ptrHead = NULL;
		ptrTail = NULL;
	}

	int PacketRFID::buildFromString(const char* value, const unsigned int len)
	{
		memcpy(data, value, len);
		parseHead();
		parseBody();

		return 0;
	}

	char* PacketRFID::setHead()
	{
		return data;
	}

	char* PacketRFID::setBody()
	{
		return data + sizeof(HeadRfid);
	}

	int PacketRFID::parseHead()
	{
		LOG_DEBUG("parseHead : size = " << sizeof(HeadRfid) << ", context = " << common::printByHex(data, sizeof(HeadRfid)));

		this->len = sizeof(HeadRfid);

		ptrHead = (HeadRfidPtr)data;
		tlv.type = ptrHead->sof;
		//注意：这个长度需要 *2
		tlv.len = common::hexStr2int(ptrHead->len, sizeof(ptrHead->len))*2;

		//目前只处理 RS232 协议
		if (tlv.type != RID_SOF && (int)tlv.len != 20)
		{
			LOG_DEBUG("rfid type or len error!");
			return -1;
		}

		return (int)tlv.len + sizeof(Tail);
	}

	int PacketRFID::parseBody()
	{
		LOG_DEBUG("parseBody : size = " << (int)tlv.len + sizeof(Tail) << ", context = " << common::printByHex(data + sizeof(HeadRfid), (int)tlv.len + sizeof(Tail)));

		this->len += (int)tlv.len + sizeof(Tail);

		tlv.value = &data[sizeof(HeadRfid)];
		ptrTail = (TailPtr)&data[sizeof(HeadRfid) + (int)tlv.len];

		int origin = common::hexStr2int(ptrTail->check, sizeof(ptrTail->check));
		unsigned char calculate = checkSum((unsigned char*)data, this->len - sizeof(Tail));

		// TODO：计算校验码
		// if(origin != calculate)
		// {
		// 	LOG_DEBUG("origin = " << origin);
		// 	LOG_DEBUG("calculate = " << calculate);
		// 	LOG_DEBUG("calculate(int) = " << (int)calculate);
		// 	LOG_DEBUG("decoding..... : check verify error!");
		// 	return -1;
		// }

		if(ptrTail->eof[0] != 0x0D || ptrTail->eof[1] != 0x0A)
		{
			LOG_DEBUG("decoding..... : eof verify error!");
			return -1;
		}

		printDetail();

		return 0;
	}

	unsigned char PacketRFID::checkSum(unsigned char* uBuff, unsigned char uBuffLen)
	{
		unsigned char i, uSum = 0;

		for(i = 0; i < uBuffLen; i++)
		{
			uSum = uSum + uBuff[i];
		}

		uSum = (~uSum) + 1;

		return uSum;
	}

	void PacketRFID::printDetail()
	{
		LOG_DEBUG("All: size = " << len << ", context = " << common::printByHex(data, len));
		LOG_DEBUG("Head: size = " << sizeof(HeadRfid) << ", context = " << common::printByHex((char*)ptrHead, sizeof(HeadRfid)));
		LOG_DEBUG("TLV group: size = 1");
		LOG_DEBUG("tlv type: 0x" << common::printByHex(&tlv.type, 1));
		LOG_DEBUG("tlv len: 0x" << common::printByHex(&tlv.len, 1));
		LOG_DEBUG("tlv value: size = " << (int)tlv.len << ", context = " << common::printByHex(tlv.value, (int)tlv.len));
		LOG_DEBUG("check sum & eof = " << common::printByHex((char*)ptrTail, sizeof(Tail)));
	}

	const char* PacketRFID::getMsg() const
	{
		return data;
	}

	unsigned short PacketRFID::getMsgLen() const
	{
		return len;
	}

	unsigned char PacketRFID::getSOF() const
	{
		return ptrHead->sof;
	}

	TLV_RFID_PTR PacketRFID::getTLV() const
	{
		return &tlv;
	}
};// namespace protocol