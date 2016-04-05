/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息处理
//
//
*/

#include "tlv_rfid_handle.h"

TLV_HANDLE_RTN TlvRfidHandle::tlv_rfid_rs232_handle(TLV_RFID_RS232& rs232, TLV_RFID_HANDLE_RS232& rs232_handle)
{
	LOG_DEBUG("====== tlv_rfid_rs232_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_RFID_RS232 content : ");
	LOG_DEBUG("type_rfid_io = " << rs232.type << \
			", rfid_io = " << rs232.rfid << \
			", io = " << rs232.io);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	stringstream ss;

	// type[2] 通常被写满数据，从而导致没有 '\0' 结束符
	if(rs232.type[0] != '\0')
		rs232_handle.type.assign((char*)rs232.type, sizeof(rs232.type));
	else
		rs232_handle.type = "0";

	//转换方法与 type一致
	if(rs232.rfid[0] != '\0')
		rs232_handle.rfid.assign((char*)rs232.rfid, sizeof(rs232.rfid));
	else
		rs232_handle.rfid = "0";

	//转换方法与 type一致
	if(rs232.io[0] != '\0')
		rs232_handle.io.assign((char*)rs232.io, sizeof(rs232.io));
	else
		rs232_handle.io = "0";

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_RFID_HANDLE_RS232 content : ");
	LOG_DEBUG("type = " << rs232_handle.type << \
			", rfid = " << rs232_handle.rfid << \
			", io = " << rs232_handle.io);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}