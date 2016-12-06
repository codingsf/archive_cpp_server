/*
//
//	Last Modify Date: 2016-09-13
//	Author: zengpw
//	History:
//					2015-09-24 zengpw created
//	File Property: public
//	Future:
//					I. tlv 子协议结构
//
//
*/

#ifndef _PROTOCOL_TLV_DATA_H_
#define _PROTOCOL_TLV_DATA_H_

#include "protocol_tlv_sys.h"

using namespace std;

namespace protocol
{
	namespace tlv
	{
		#pragma pack(push, 1)

		//公共结构体 - 时间
		typedef struct DateTime
		{
			char year;
			char mon;
			char day;
			char hour;
			char min;
			char sec;
		}DateTime;
		DateTime buildDateTime2k();
		void printDateTime2k(DateTime& dt);
		string DateTime2String2k(DateTime& dt);

		//公共结构体 - linux 时间（秒数）
		typedef int DateTimeSec;
		DateTimeSec buildDateTimeSec();
		void printDateTimeSec(DateTimeSec& dts);
		string DateTime2StringSec(DateTimeSec& dts);

		//公共结构体 - 基础数据
		typedef struct BasicData
		{
			unsigned char rssi;								//场强
			unsigned char move;								//移动标记
			unsigned char flag;								// =0 光曝正常； =1 光曝报警
			unsigned short voltage;							//电压
			short temperature;								//温度
			unsigned short exposure;						//光曝
		}BasicData;

		//公共结构体 - lbs信息
		typedef struct LbsData
		{
			unsigned char cell;
			unsigned char mnc;
			unsigned short mcc;
			unsigned short lac;
			unsigned short cell_id;
			unsigned char bsic;
			unsigned char rssi;
		}LbsData;

		//标准报文1 - TLV_DEV_FIRM
		typedef struct TLV_DEV_FIRM
		{
			unsigned short version;
			unsigned char imei[15];
			unsigned char iccid[20];
		}TLV_DEV_FIRM;

		//标准报文1 - TLV_DEV_FIRM
		//协议版本 v1.1
		typedef struct TLV_DEV_FIRM_V11
		{
			unsigned short version;
			unsigned char imei[15];
			unsigned char iccid[20];
			unsigned char mac[17];
		}TLV_DEV_FIRM_V11;

		//标准报文2 - TLV_TIME
		typedef DateTimeSec TLV_TIME;

		//标准报文3 - TLV_DEV_MSG
		typedef struct TLV_DEV_MSG
		{
			unsigned char state;
			unsigned char rssi;
			unsigned short version;
			unsigned short voltage;
			unsigned short flag;
		}TLV_DEV_MSG;

		//标准报文4 - TLV_RTN
		typedef char TLV_RTN;

		//标准报文5 - TLV_ORDER_ID
		typedef char TLV_ORDER_ID[12];

		//标准报文6 - TLV_PARAMETER
		typedef struct TLV_PARAMETER
		{
			//服务器
			unsigned char addr[30];
			unsigned char port[8];
			unsigned char apn[20];
			unsigned char user[10];
			unsigned char pass[10];
			unsigned char net;
			unsigned char band;
			unsigned char led;
			//开关
			unsigned char exposure_switch;
			unsigned char exposure_alarm_switch;
			unsigned char exposure_voice_switch;
			unsigned char t_switch;
			unsigned char t0_alarm_switch;
			unsigned char t0_voice_switch;
			unsigned char h0_alarm_switch;
			unsigned char h0_voice_switch;
			unsigned char t1_alarm_switch;
			unsigned char t1_voice_switch;
			unsigned char h1_alarm_switch;
			unsigned char h1_voice_switch;
			//门限
			unsigned short exposure_max;
			short t0_max;
			short t0_min;
			short h0_max;
			short h0_min;
			short t1_max;
			short t1_min;
			short h1_max;
			short h1_min;
			short temperature_cycle;
			short temperature_store_cycle;
			short temperature_store_cycle_alarm;
			short exposure_cycle;
			short exposure_store_cycle;
			unsigned int lbs_send_cycle;
			unsigned int lbs_send_cycle_alarm;
		}TLV_PARAMETER;

		//标准报文6 - TLV_PARAMETER
		//协议版本 v1.1
		typedef struct TLV_PARAMETER_V11
		{
			//服务器
			unsigned char addr[30];
			unsigned char port[8];
			unsigned char apn[20];
			unsigned char user[10];
			unsigned char pass[10];
			unsigned char net;
			unsigned char band;
			unsigned char led;
			//开关
			unsigned char exposure_switch;
			unsigned char exposure_alarm_switch;
			unsigned char exposure_voice_switch;
			unsigned char t_switch;
			unsigned char t0_alarm_switch;
			unsigned char t0_voice_switch;
			unsigned char h0_alarm_switch;
			unsigned char h0_voice_switch;
			unsigned char t1_alarm_switch;
			unsigned char t1_voice_switch;
			unsigned char h1_alarm_switch;
			unsigned char h1_voice_switch;
			// v1.1开关
			unsigned char shock_switch;
			unsigned char shock_alarm_switch;
			unsigned char shock_voice_switch;
			unsigned char tilt_switch;
			unsigned char tilt_alarm_switch;
			unsigned char tilt_voice_switch;
			unsigned char tilt_fix_flag;
			unsigned char tilt_x;
			unsigned char tilt_y;
			unsigned char tilt_z;
			unsigned char baro_switch;
			unsigned char baro_alarm_switch;
			unsigned char baro_voice_switch;
			//门限
			unsigned short exposure_max;
			short t0_max;
			short t0_min;
			short h0_max;
			short h0_min;
			short t1_max;
			short t1_min;
			short h1_max;
			short h1_min;
			short temperature_cycle;
			short temperature_store_cycle;
			short temperature_store_cycle_alarm;
			short exposure_cycle;
			short exposure_store_cycle;
			// v1.1门限
			short tilt_store_cycle;
			short tilt_limit;
			short shock_cycle;
			short shock_limit;
			short baro_store_cycle;
			short baro_max;
			short baro_min;
			//额外数据
			unsigned int lbs_send_cycle;
			unsigned int lbs_send_cycle_alarm;
		}TLV_PARAMETER_V11;

		//标准报文6 - TLV_PARAMETER
		//协议版本 v1.3
		typedef struct TLV_PARAMETER_V13
		{
			//服务器
			unsigned char addr[30];
			unsigned char port[8];
			unsigned char apn[20];
			unsigned char user[10];
			unsigned char pass[10];
			unsigned char net;
			unsigned char band;
			unsigned char led;
			//开关
			unsigned char exposure_switch;
			unsigned char exposure_alarm_switch;
			unsigned char exposure_voice_switch;
			unsigned char t_switch;
			unsigned char t0_alarm_switch;
			unsigned char t0_voice_switch;
			unsigned char h0_alarm_switch;
			unsigned char h0_voice_switch;
			unsigned char t1_alarm_switch;
			unsigned char t1_voice_switch;
			unsigned char h1_alarm_switch;
			unsigned char h1_voice_switch;
			// v1.1开关
			unsigned char shock_switch;
			unsigned char shock_alarm_switch;
			unsigned char shock_voice_switch;
			unsigned char tilt_switch;
			unsigned char tilt_alarm_switch;
			unsigned char tilt_voice_switch;
			unsigned char tilt_fix_flag;
			unsigned char tilt_x;
			unsigned char tilt_y;
			unsigned char tilt_z;
			unsigned char baro_switch;
			unsigned char baro_alarm_switch;
			unsigned char baro_voice_switch;
			// v1.3开关
			unsigned char pdf_switch;
			unsigned char pdf;
			//门限
			unsigned short exposure_max;
			short t0_max;
			short t0_min;
			short h0_max;
			short h0_min;
			short t1_max;
			short t1_min;
			short h1_max;
			short h1_min;
			short temperature_cycle;
			short temperature_store_cycle;
			short temperature_store_cycle_alarm;
			short exposure_cycle;
			short exposure_store_cycle;
			// v1.1门限
			short tilt_store_cycle;
			short tilt_limit;
			short shock_cycle;
			short shock_limit;
			short baro_store_cycle;
			short baro_max;
			short baro_min;
			//额外数据
			unsigned int lbs_send_cycle;
			unsigned int lbs_send_cycle_alarm;
		}TLV_PARAMETER_V13;

		//标准报文6 - TLV_PARAMETER
		//协议版本 v1.4
		typedef struct TLV_PARAMETER_V14
		{
			//服务器
			unsigned char addr[30];
			unsigned char port[8];
			unsigned char apn[20];
			unsigned char user[10];
			unsigned char pass[10];
			unsigned char net;
			unsigned char band;
			unsigned char led;
			//开关
			unsigned char exposure_switch;
			unsigned char exposure_alarm_switch;
			unsigned char exposure_voice_switch;
			unsigned char t_switch;
			unsigned char t0_alarm_switch;
			unsigned char t0_voice_switch;
			unsigned char h0_alarm_switch;
			unsigned char h0_voice_switch;
			unsigned char t1_alarm_switch;
			unsigned char t1_voice_switch;
			unsigned char h1_alarm_switch;
			unsigned char h1_voice_switch;
			// v1.1开关
			unsigned char shock_switch;
			unsigned char shock_alarm_switch;
			unsigned char shock_voice_switch;
			unsigned char tilt_switch;
			unsigned char tilt_alarm_switch;
			unsigned char tilt_voice_switch;
			unsigned char tilt_fix_flag;
			unsigned char tilt_x;
			unsigned char tilt_y;
			unsigned char tilt_z;
			unsigned char baro_switch;
			unsigned char baro_alarm_switch;
			unsigned char baro_voice_switch;
			// v1.3开关
			unsigned char pdf_switch;
			unsigned char pdf;
			//门限
			unsigned short exposure_max;
			short t0_max;
			short t0_min;
			short h0_max;
			short h0_min;
			short t1_max;
			short t1_min;
			short h1_max;
			short h1_min;
			// v1.4门限
			short t0_pMax;
			short t0_pMin;
			short t1_pMax;
			short t1_pMin;
			//门限
			short temperature_cycle;
			short temperature_store_cycle;
			short temperature_store_cycle_alarm;
			short exposure_cycle;
			short exposure_store_cycle;
			// v1.1门限
			short tilt_store_cycle;
			short tilt_limit;
			short shock_cycle;
			short shock_limit;
			short baro_store_cycle;
			short baro_max;
			short baro_min;
			//额外数据
			unsigned int lbs_send_cycle;
			unsigned int lbs_send_cycle_alarm;
		}TLV_PARAMETER_V14;

		//标准报文7 - TLV_PARAMETER_VERSION
		typedef struct TLV_PARAMETER_VERSION
		{
			unsigned char update_flag;
			unsigned char version;
			unsigned char addr[30];
			unsigned char port[8];
			unsigned char apn[20];
			unsigned char user[15];
			unsigned char pass[15];
			unsigned char file[20];
			unsigned char path[30];
		}TLV_PARAMETER_VERSION;

		//标准报文8 - TLV_GPS
		typedef struct TLV_GPS
		{
			DateTime dt;
			BasicData bd;
			unsigned char sn_lat;							//南纬北纬
			unsigned char ew_long;							//东经西经
			unsigned int latitude;							//纬度
			unsigned int longitude;							//经度
		}TLV_GPS;

		//标准报文9 - TLV_LBS
		typedef struct TLV_LBS
		{
			DateTime dt;
			BasicData bd;
			LbsData ld[3];
		}TLV_LBS;

		//标准报文A - TLV_TH
		typedef struct TLV_TH
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;							//只是代表是8位的长度，实际是按照 int 类型填充的，相当于 uint8_t
			short t0;
			short h0;
		}TLV_TH;

		//标准报文A - TLV_TH
		//协议版本 v1.2
		typedef struct TLV_TH_V12
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm[8];
			short t0;
			short h0;
		}TLV_TH_V12;

		//标准报文B - TLV_EXPOSURE
		typedef struct TLV_EXPOSURE
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;
			unsigned short exposure;
		}TLV_EXPOSURE;

		//标准报文C - TLV_MANAGE_LBS
		typedef struct TLV_MANAGE_LBS
		{
			DateTime dt;
			unsigned char rssi;
			unsigned char state;
			unsigned short voltage;
			short temperature;
			LbsData ld[3];
		}TLV_MANAGE_LBS;

		//标准报文D - TLV_TH2
		typedef struct TLV_TH2
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;
			short t0;
			short h0;
			short t1;
			short h1;
		}TLV_TH2;

		//标准报文D - TLV_TH2
		//协议版本 v1.2
		typedef struct TLV_TH2_V12
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm[8];
			short t0;
			short h0;
			short t1;
			short h1;
		}TLV_TH2_V12;

		//标准报文E - TLV_SHOCK
		typedef struct TLV_SHOCK
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;
			short x;
			short y;
			short z;
			short max;
		}TLV_SHOCK;

		//标准报文F - TLV_TILT
		typedef struct TLV_TILT
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;
			short x;
			short y;
			short z;
			short max;
		}TLV_TILT;

		//标准报文10 - TLV_BARO
		typedef struct TLV_BARO
		{
			DateTime dt;
			DateTime dt_gps;
			unsigned char alarm;
			unsigned short baro;
		}TLV_BARO;

		//标准报文11 - TLV_TIME_6
		typedef struct TLV_TIME_6
		{
			DateTime dt;
		}TLV_TIME_6;

		//标准报文12 - TLV_EXPRESS_ID
		typedef struct TLV_EXPRESS_ID
		{
			unsigned char express_id[32];
		}TLV_EXPRESS_ID;

		//标准报文15 - TLV_DEV_TYPE
		typedef struct TLV_DEV_TYPE
		{
			unsigned char type[20];
		}TLV_DEV_TYPE;

		//标准报文27 - TLV_SRV_STATUS
		typedef struct TLV_SRV_STATUS
		{
			unsigned char array_test[30];
			short short_test;
			unsigned int u_int_test;
			unsigned char char_test;
		}TLV_SRV_STATUS;

		#pragma pack(pop)
	}// namespace tlv
};// namespace protocol

#endif