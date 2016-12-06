/*
//
//	Last Modify Date: 2016-12-02
//	Author: zengpw
//	History:
//					2015-10-20 zengpw created
//	File Property: private
//	Future:
//					I. tlv 消息处理
//
//
*/

#ifndef _TLV_HANDLE_DATA_H_
#define _TLV_HANDLE_DATA_H_

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


//公共结构体 BasicData 的处理结果
typedef struct HANDLE_BasicData
{
	string rssi;									//场强
	string move;									//移动标记
	string flag;									// =0 光曝正常； =1 光曝报警
	string voltage;									//电压
	string temperature;								//温度
	string exposure;								//光曝
	//额外数据
	string alarm;
	string addr;
}HANDLE_BasicData;

//标准报文 TLV_DEV_FIRM 的处理结果
typedef struct TLV_HANDLE_DEV_FIRM
{
	string dev_id;
	string version;
	string imei;
	string iccid;
}TLV_HANDLE_DEV_FIRM;

//标准报文 TLV_DEV_FIRM_V11 的处理结果
typedef struct TLV_HANDLE_DEV_FIRM_V11
{
	string dev_id;
	string version;
	string imei;
	string iccid;
	string mac;
}TLV_HANDLE_DEV_FIRM_V11;

//标准报文 TLV_DEV_MSG 的处理结果
typedef struct TLV_HANDLE_DEV_MSG
{
	string dev_id;
	string state;
	string rssi;
	string version;
	string voltage;
	string flag;
}TLV_HANDLE_DEV_MSG;

//标准报文 TLV_GPS 的处理结果
typedef struct TLV_HANDLE_GPS
{
	string dev_id;
	string order_id;
	string datetime;
	HANDLE_BasicData hbd;
	string latitude;
	string longitude;
	//额外数据
	string mode;									// 0 = gps; 1 = lbs
}TLV_HANDLE_GPS;

//标准报文 TLV_LBS 的处理结果
typedef struct TLV_HANDLE_LBS
{
	string dev_id;
	string order_id;
	string datetime;
	HANDLE_BasicData hbd;
	string latitude;
	string longitude;
	string url;
	//额外数据
	string mode;									// 0 = gps; 1 = lbs
}TLV_HANDLE_LBS;

//标准报文 TLV_TH 的处理结果
typedef struct TLV_HANDLE_TH
{
	string dev_id;
	string order_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string t0;
	string h0;
	//额外数据
	//温度绝对值超过80℃则标记
	string overflow;
}TLV_HANDLE_TH;

//标准报文 CID_UNBAND - TLV_RTN 的处理结果
typedef struct TLV_HANDLE_UNBAND_RTN
{
	string dev_id;
	string rtn;
}TLV_HANDLE_UNBAND_RTN;

//标准报文 CID_DEV_UNBAND - TLV_RTN 的处理结果
typedef struct TLV_HANDLE_DEV_UNBAND_RTN
{
	string dev_id;
	string datetime;
}TLV_HANDLE_DEV_UNBAND_RTN;

//标准报文 CID_PARAMETER - TLV_RTN 的处理结果
typedef struct TLV_HANDLE_PARAMETER_RTN
{
	string dev_id;
	string rtn;
}TLV_HANDLE_PARAMETER_RTN;

//标准报文 CID_PARAMETER_VERSION - TLV_RTN 的处理结果
typedef struct TLV_HANDLE_VERSION_RTN
{
	string dev_id;
	string rtn;
}TLV_HANDLE_VERSION_RTN;

//标准报文 TLV_MANAGE_LBS 的处理结果
typedef struct TLV_HANDLE_MANAGE_LBS
{
	string dev_id;
	string datetime;
	string rssi;
	string state;
	string voltage;
	string temperature;
	string latitude;
	string longitude;
	string url;
	string addr;
}TLV_HANDLE_MANAGE_LBS;

//标准报文 TLV_TH2 的处理结果
typedef struct TLV_HANDLE_TH2
{
	string dev_id;
	string order_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string t0;
	string h0;
	string t1;
	string h1;
	//额外数据
	//温度绝对值超过80℃则标记
	string overflow;
}TLV_HANDLE_TH2;

//标准报文 TLV_HANDLE_EXPOSURE 的处理结果
typedef struct TLV_HANDLE_EXPOSURE
{
	string dev_id;
	string order_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string exposure;
}TLV_HANDLE_EXPOSURE;

//标准报文 TLV_HANDLE_SHOCK 的处理结果
typedef struct TLV_HANDLE_SHOCK
{
	string dev_id;
	string order_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string x;
	string y;
	string z;
	string max;
}TLV_HANDLE_SHOCK;

//标准报文 TLV_HANDLE_TILT 的处理结果
typedef struct TLV_HANDLE_TILT
{
	string dev_id;
	string order_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string x;
	string y;
	string z;
	string max;
}TLV_HANDLE_TILT;

//标准报文 TLV_HANDLE_BARO 的处理结果
typedef struct TLV_HANDLE_BARO
{
	string dev_id;
	string order_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string baro;
}TLV_HANDLE_BARO;

//标准报文 CID_DEV_BAND_TIME - TLV_TIME_6 的处理结果
typedef struct TLV_HANDLE_DEVBAND_TIME
{
	string dev_id;
	string datetime;
}TLV_HANDLE_DEVBAND_TIME;

//标准报文 TLV_DEV_TYPE 的处理结果
typedef struct TLV_HANDLE_DEV_TYPE
{
	string dev_id;
	string type;
}TLV_HANDLE_DEV_TYPE;

//标准报文 TLV_JZT_DEV 的处理结果
typedef struct TLV_HANDLE_JZT_DEV
{
	string dev_id;
	string voltage;
	string datetime;
}TLV_HANDLE_JZT_DEV;

//标准报文 TLV_JZT_LBS 的处理结果
typedef struct TLV_HANDLE_JZT_LBS
{
	string dev_id;
	string datetime;
	string latitude;
	string longitude;
	string url;
}TLV_HANDLE_JZT_LBS;

//标准报文 TLV_JZT_TH2 的处理结果
typedef struct TLV_HANDLE_JZT_TH2
{
	string dev_id;
	string datetime;
	string datetime_gps;
	string alarm;
	string t0;
	string h0;
	string t1;
	string h1;
	//额外数据
	//温度绝对值超过80℃则标记
	string overflow;
}TLV_HANDLE_JZT_TH2;

#endif