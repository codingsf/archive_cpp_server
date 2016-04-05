/*
//
//	Last Modify Date: 2016-02-19
//	Author: zengpw
//	History:
//					2016-02-19 zengpw created
//	File Property: private
//	Future:
//					I. RFID消息入库
//
//
*/

#include "rfid_db_interface.h"

TLV_HANDLE_RTN RfidDbInterface::rs232_update(common::DB* db, TLV_RFID_HANDLE_RS232& rs232_handle)
{
	string sql;

	sql = "insert into t_rfid_tag ";
	sql += "(id, tag_id, create_date) values(";
	sql += "rfid_tag_seq.nextval, '";
	sql += rs232_handle.rfid + "', sysdate)";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}