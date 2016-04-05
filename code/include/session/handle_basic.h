/*
//
//	Last Modify Date: 2016-03-25
//	Author: zengpw
//	History:
//					2015-09-15 zengpw created
//	File Property: public
//	Future:
//					I. TLV & RFID 公共消息处理基类
//
//
*/

#ifndef _HANDLE_BASIC_H_
#define _HANDLE_BASIC_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <errno.h>
#include "common/db.h"
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_tlv.h"
#include "protocol/protocol_rfid.h"
#include "heartbeat.h"

using namespace std;

namespace session
{
	//返回给调用方的提示
	typedef enum HANDLE_RTN
	{
		HANDLE_RTN_SEND = 0x01,				//发送 out 报文					【redis: 删除原数据】
		HANDLE_RTN_CONTINUE = 0x02,			//忽略 out 报文并继续			【redis: 不删除原数据】
		HANDLE_RTN_REMOVE = 0x04,			//忽略 out 报文并继续			【redis: 删除原数据】
		HANDLE_RTN_STOP = 0x08,				//关闭 session
		HANDLE_RTN_CLOSE = 0x10				//不再处理 out 报文
	}HANDLE_RTN;

	class HandleBasic
	{
	public:
		common::DB* db;
	public:
		HandleBasic();
		HandleBasic(string db_conn_info);
	public:
		//分发 TLV 消息
		virtual HANDLE_RTN dispatch(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN dispatch_origin_msg(const char* value, const unsigned int len, protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN conn(protocol::PacketOut* out);
		virtual HANDLE_RTN reject(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN sending(const protocol::PacketIn* in, vector<protocol::PacketOut> &vp);
		//按 CID 处理消息
		virtual HANDLE_RTN cidLogin(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDevMsg(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidUnband(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDevUnband(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDevband(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidParameterRequest(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDevInit(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDevTime(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataGps(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataTh(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataExposure(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataTh2(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidEFence(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataShock(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataTilt(const protocol::PacketIn* in, protocol::PacketOut* out);
		virtual HANDLE_RTN cidDataBaro(const protocol::PacketIn* in, protocol::PacketOut* out);
	public:
		//分发 RFID 消息
		virtual HANDLE_RTN dispatch_rfid(const protocol::PacketRFID* in);
		virtual HANDLE_RTN dispatch_rfid_origin_msg(const char* value, const unsigned int len, protocol::PacketRFID* in);
		//按 RID 处理消息
		virtual HANDLE_RTN ridSOF(const protocol::PacketRFID* in);
	};
};// namespace session

#endif