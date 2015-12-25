/*
//
//	Last Modify Date: 2015-12-17
//	Author: zengpw
//	History:
//					2015-10-20 zengpw created
//	File Property: private
//	Future:
//					I. tlv 消息处理
//
//
*/

#ifndef _TLV_HANDLE_H_
#define _TLV_HANDLE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/redis.h"
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_tlv.h"
#include "http_interface.h"
#include "tlv_handle_data.h"

using namespace std;

class TlvHandle
{
public:
	TLV_HANDLE_RTN basic_data_handle(BasicData& bd, HANDLE_BasicData& hbd);
public:
	TLV_HANDLE_RTN tlv_firm_handle(TLV_DEV_FIRM& firm, TLV_HANDLE_DEV_FIRM& firm_handle);
	TLV_HANDLE_RTN tlv_gps_handle(TLV_GPS& gps, TLV_HANDLE_GPS& gps_handle);
	TLV_HANDLE_RTN tlv_lbs_handle(TLV_LBS& lbs, TLV_HANDLE_LBS& lbs_handle);
	TLV_HANDLE_RTN tlv_th_handle(TLV_TH& th, TLV_HANDLE_TH& th_handle);
	TLV_HANDLE_RTN tlv_unband_handle(TLV_RTN& rtn, TLV_HANDLE_UNBAND_RTN& unband_handle);
	TLV_HANDLE_RTN tlv_dev_unband_handle(TLV_TIME& tm, TLV_HANDLE_DEV_UNBAND_RTN& dev_unband_handle);
	TLV_HANDLE_RTN tlv_dev_msg_handle(TLV_DEV_MSG& tdm, TLV_HANDLE_DEV_MSG& dev_msg_handle);
	TLV_HANDLE_RTN tlv_parameter_handle(TLV_RTN& rtn, TLV_HANDLE_PARAMETER_RTN& parameter_handle);
	TLV_HANDLE_RTN tlv_version_handle(TLV_RTN& rtn, TLV_HANDLE_VERSION_RTN& ver_handle);
	TLV_HANDLE_RTN tlv_manage_lbs_handle(TLV_MANAGE_LBS& m_lbs, TLV_HANDLE_MANAGE_LBS& m_lbs_handle);
	TLV_HANDLE_RTN tlv_th2_handle(TLV_TH2& th, TLV_HANDLE_TH2& th_handle);
};

#endif