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

#ifndef _TLV_RFID_HANDLE_DATA_H_
#define _TLV_RFID_HANDLE_DATA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/log.h"

using namespace std;

typedef enum TLV_HANDLE_RTN
{
	THR_OK = 0x01,					//处理正确，返回值需要处理
	THR_TRUE = 0x02,				//处理正确，返回值不需要处理
	THR_FAIL = 0x04,				//处理错误，并且需要处理错误
	THR_ERROR = 0x08				//处理错误，但是可以忽略错误
}TLV_HANDLE_RTN;


//标准报文 TLV_RFID_RS232 的处理结果
typedef struct TLV_RFID_HANDLE_RS232
{
	string type;
	string rfid;
	string io;
}TLV_RFID_HANDLE_RS232;

#endif