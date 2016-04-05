/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息分析和入库
//
//
*/

#include "rfid_upload_handle.h"

session::HANDLE_RTN RfidUploadHandle::ridSOF(const protocol::PacketRFID* in)
{
	bool error = false;

	TlvRfidHandle rh;
	RfidDbInterface ri;

	TLV_RFID_PTR pTlv = in->getTLV();
	LOG_DEBUG("========= ridSOF processing begin");

	if(pTlv->type _EQ_ RID_SOF)
	{
		TLV_RFID_RS232 rs232;
		TLV_RFID_HANDLE_RS232 rs232_handle;

		memset(&rs232, 0, sizeof(rs232));
		memcpy(&rs232, pTlv->value, pTlv->len);

		int rtn = rh.tlv_rfid_rs232_handle(rs232, rs232_handle);

		if(rtn _NEQ_ THR_OK)
		{
			error = true;
		}
		else
		{
			rtn = ri.rs232_update(db, rs232_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
			}
		}
	}

	LOG_DEBUG("========= ridSOF processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}