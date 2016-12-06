/*
//
//	Last Modify Date: 2016-09-26
//	Author: zengpw
//	History:
//					2015-09-15 zengpw created
//	File Property: public
//	Future:
//					I. TLV & RFID 公共消息处理基类
//
//
*/

#include "handle_basic.h"

namespace session
{
	HandleBasic::HandleBasic()
	{
		db = NULL;
	}

	HandleBasic::HandleBasic(string db_conn_info)
	{
		db = new common::DB(db_conn_info);

		if(db->getConn() == NULL)
		{
			delete db;
			db = NULL;
			LOG_DEBUG("connect database error : " << db_conn_info);
		}
	}

	//---------------------------------------------- TLV 消息处理
	HANDLE_RTN HandleBasic::dispatch(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		LOG_DEBUG("--------------- dispatch message, command id = 0x" << hex << setiosflags(ios::uppercase) << setfill('0') << setw(4) << in->getCID() << " ---------------");

		out->copy(in);

		if(in->getCID() == protocol::tlv::CID_LOGIN)
			return cidLogin(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_MSG)
			return cidDevMsg(in, out);

		if(in->getCID() == protocol::tlv::CID_UNBAND)
			return cidUnband(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_UNBAND)
			return cidDevUnband(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_BAND)
			return cidDevband(in, out);

		if(in->getCID() == protocol::tlv::CID_PARAMETER)
			return cidParameter(in, out);

		if(in->getCID() == protocol::tlv::CID_PARAMETER_REQUEST)
			return cidParameterRequest(in, out);

		if(in->getCID() == protocol::tlv::CID_PARAMETER_VERSION)
			return cidParameterVersion(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_INIT)
			return cidDevInit(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_TIME)
			return cidDevTime(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_GPS)
			return cidDataGps(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_LBS)
			return cidDataLbs(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_TH)
			return cidDataTh(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_EXPOSURE)
			return cidDataExposure(in, out);

		if(in->getCID() == protocol::tlv::CID_MANAGE_LBS)
			return cidManageLbs(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_TH2)
			return cidDataTh2(in, out);

		if(in->getCID() == protocol::tlv::CID_E_FENCE)
			return cidEFence(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_SHOCK)
			return cidDataShock(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_TILT)
			return cidDataTilt(in, out);

		if(in->getCID() == protocol::tlv::CID_DATA_BARO)
			return cidDataBaro(in, out);

		if(in->getCID() == protocol::tlv::CID_TIME_START)
			return cidTimeStart(in, out);

		if(in->getCID() == protocol::tlv::CID_TIME_END)
			return cidTimeEnd(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_BAND_TIME)
			return cidDevbandTime(in, out);

		if(in->getCID() == protocol::tlv::CID_DEV_TYPE)
			return cidDevType(in, out);

		if(in->getCID() == protocol::tlv::CID_SRV_STATUS)
			return cidSrvStatus(in, out);

		if(in->getCID() == protocol::tlv::CID_AUTO_UPLOAD_R)
			return cidAutoUploadR(in, out);

		if(in->getCID() == protocol::tlv::CID_JZT_DEV)
			return cidJztDev(in, out);

		if(in->getCID() == protocol::tlv::CID_JZT_LBS)
			return cidJztLbs(in, out);

		if(in->getCID() == protocol::tlv::CID_JZT_TH2)
			return cidJztTh2(in, out);

		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::dispatch_origin_msg(const char* value, const unsigned int len, protocol::PacketIn* in, protocol::PacketOut* out)
	{
		if(in->buildFromString(value, len) != 0)
			return HANDLE_RTN_CONTINUE;

		return this->dispatch(in, out);
	}

	HANDLE_RTN HandleBasic::conn(protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::reject(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::sending(const protocol::PacketIn* in, vector<protocol::PacketOut> &vp)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidLogin(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevMsg(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevband(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidParameterRequest(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevInit(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevTime(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataGps(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataTh(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataExposure(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataTh2(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidEFence(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataShock(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataTilt(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDataBaro(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidTimeStart(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidTimeEnd(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevbandTime(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidDevType(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidJztDev(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidSrvStatus(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidJztLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::cidJztTh2(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	//---------------------------------------------- 自解析协议消息处理
	HANDLE_RTN HandleBasic::cidAutoUploadR(const protocol::PacketIn* in, protocol::PacketOut* out)
	{
		return HANDLE_RTN_CONTINUE;
	}

	//---------------------------------------------- RFID 消息处理
	HANDLE_RTN HandleBasic::dispatch_rfid(const protocol::PacketRFID* in)
	{
		LOG_DEBUG("--------------- dispatch_rfid message, command id = 0x" << hex << setiosflags(ios::uppercase) << setfill('0') << setw(4) << (int)in->getSOF() << " ---------------");

		if(in->getSOF() == protocol::rfid::RID_SOF)
			return ridSOF(in);

		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::ridSOF(const protocol::PacketRFID* in)
	{
		return HANDLE_RTN_CONTINUE;
	}

	HANDLE_RTN HandleBasic::dispatch_rfid_origin_msg(const char* value, const unsigned int len, protocol::PacketRFID* in)
	{
		if(in->buildFromString(value, len) != 0)
			return HANDLE_RTN_CONTINUE;

		return this->dispatch_rfid(in);
	}
};// namespace session