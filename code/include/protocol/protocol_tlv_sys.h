/*
//
//	Last Modify Date: 2016-09-26
//	Author: zengpw
//	History:
//					2015-08-19 zengpw created
//	File Property: public
//	Future:
//					I. tlv 基础结构
//
//
*/

#ifndef _PROTOCOL_TLV_SYS_H_
#define _PROTOCOL_TLV_SYS_H_

#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/log.h"

using namespace std;

namespace protocol
{
	namespace tlv
	{
		#pragma pack(push, 1)

		enum CID
		{
			CID_LOGIN = 0x0001,							//登录/注册
			CID_DEV_MSG = 0x0002,						//设备信息
			CID_UNBAND = 0x0003,						//远程解绑
			CID_DEV_UNBAND = 0x0004,					//设备发起解绑
			CID_DEV_BAND = 0x0005,						//设备发起绑定
			CID_PARAMETER = 0x0006,						//运行参数下发
			CID_PARAMETER_REQUEST = 0x0007,				//设备请求运行参数
			CID_PARAMETER_VERSION = 0x0008,				//版本升级参数下发
			CID_DEV_INIT = 0x0009,						//初始化终端
			CID_DEV_TIME = 0x000A,						//终端时间校正
			CID_DATA_GPS = 0x000B,						//gps数据上传
			CID_DATA_LBS = 0x000C,						//lbs数据上传
			CID_DATA_TH = 0x000D,						//温湿度数据上传
			CID_DATA_EXPOSURE = 0x000E,					//光曝数据上传
			CID_MANAGE_LBS = 0x000F,					//资产管理 lbs 类型终端
			CID_DATA_TH2 = 0x0010,						//双温湿度数据上传
			CID_E_FENCE = 0x0011,						//电子围栏标识
			CID_DATA_SHOCK = 0x0012,					//震动数据上传
			CID_DATA_TILT = 0x0013,						//倾角数据上传
			CID_DATA_BARO = 0x0014,						//气压数据上传
			CID_DEV_TYPE = 0x0015,						//设备型号
			CID_TIME_START = 0x0023,					//起运时间下发
			CID_DEV_BAND_TIME = 0x0025,					//设备发起绑定（带时间版本）
			CID_TIME_END = 0x0026,						//签收时间下发
			CID_SRV_STATUS = 0x0027,					//服务器状态（测试）
			CID_AUTO_UPLOAD_R = 0x0030,					//自解析协议 - 上发，固定回复
			CID_AUTO_UPLOAD_DATA = 0x0032,				//自解析协议 - 上发，回复数据
			CID_JZT_DEV = 0x0034,						//九州通电量数据上传
			CID_JZT_LBS = 0x0035,						//九州通lbs数据上传
			CID_JZT_TH2 = 0x0036						//九州通双温湿度数据上传
		};

		enum TID
		{
			TID_DEV_FIRM = 0x01,					//协议版本
			TID_TIME = 0x02,						//时间
			TID_DEV_MSG = 0x03,						//设备信息
			TID_RTN = 0x04,							//1 = 成功； 0 = 失败
			TID_ORDER_ID = 0x05,					//订单号
			TID_PARAMETER = 0x06,					//运行参数
			TID_PARAMETER_VERSION = 0x07,			//版本升级参数
			TID_GPS = 0x08,							//gps上传数据
			TID_LBS = 0x09,							//lbs上传数据
			TID_TH = 0x0A,							//温湿度上传数据
			TID_EXPOSURE = 0x0B,					//光曝上传数据
			TID_MANAGE_LBS = 0x0C,					//资产管理 lbs 类型终端
			TID_TH2 = 0x0D,							//双温湿度上传数据
			TID_SHOCK = 0x0E,						//震动数据上传
			TID_TILT = 0x0F,						//倾角数据上传
			TID_BARO = 0x10,						//气压数据上传
			TID_TIME_6 = 0x11,						//时间，char[6]
			TID_EXPRESS_ID = 0x12,					//运单号
			TID_DEV_TYPE = 0x15,					//设备型号
			TID_SRV_STATUS = 0x27,					//服务器状态（测试）
			TID_JZT_DEV = 0x34,						//九州通电量信息
			TID_JZT_LBS = 0x35,						//九州通lbs上传数据
			TID_JZT_TH2 = 0x36,						//九州通双温湿度上传数据
			TID_GET_ORDER = 0x50					//请求启运时间
		};

		typedef struct Head
		{
			unsigned char	xcode[4];
			unsigned char	dev_id[12];
			unsigned short	cid;
			int				build_time;
			unsigned short	len;
		}Head;
		typedef Head* HeadPtr;

		typedef struct TLV
		{
			char type;
			unsigned char len;
			char* value;

			TLV()
			{
				value = NULL;
			}
		}TLV;
		typedef vector<TLV> TLVGroup;
		typedef const TLVGroup* TLVGroupPtr;
		typedef TLVGroup::const_iterator TLVGroupIter;

		#pragma pack(pop)
	}// namespace tlv
};// namespace protocol

#endif