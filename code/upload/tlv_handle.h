/*
//
//	Last Modify Date: 2016-09-22
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
#include "session/http_interface.h"
#include "tlv_handle_data.h"

using namespace std;

class TlvHandle
{
public:
	TLV_HANDLE_RTN basic_data_handle(BasicData& bd, HANDLE_BasicData& hbd);
public:
	TLV_HANDLE_RTN tlv_firm_handle(TLV_DEV_FIRM& firm, TLV_HANDLE_DEV_FIRM& firm_handle);
	TLV_HANDLE_RTN tlv_firm_handle_ver11(TLV_DEV_FIRM_V11& firm, TLV_HANDLE_DEV_FIRM_V11& firm_handle);
	TLV_HANDLE_RTN tlv_gps_handle(TLV_GPS& gps, TLV_HANDLE_GPS& gps_handle);
	TLV_HANDLE_RTN tlv_lbs_handle(TLV_LBS& lbs, TLV_HANDLE_LBS& lbs_handle);
	TLV_HANDLE_RTN tlv_th_handle(TLV_TH& th, TLV_HANDLE_TH& th_handle);
	TLV_HANDLE_RTN tlv_th_handle_ver12(TLV_TH_V12& th, TLV_HANDLE_TH& th_handle);
	TLV_HANDLE_RTN tlv_unband_handle(TLV_RTN& rtn, TLV_HANDLE_UNBAND_RTN& unband_handle);
	TLV_HANDLE_RTN tlv_dev_unband_handle(TLV_TIME& tm, TLV_HANDLE_DEV_UNBAND_RTN& dev_unband_handle);
	TLV_HANDLE_RTN tlv_dev_msg_handle(TLV_DEV_MSG& tdm, TLV_HANDLE_DEV_MSG& dev_msg_handle);
	TLV_HANDLE_RTN tlv_dev_type_handle(TLV_DEV_TYPE& tdt, TLV_HANDLE_DEV_TYPE& dev_type_handle);
	TLV_HANDLE_RTN tlv_parameter_handle(TLV_RTN& rtn, TLV_HANDLE_PARAMETER_RTN& parameter_handle);
	TLV_HANDLE_RTN tlv_version_handle(TLV_RTN& rtn, TLV_HANDLE_VERSION_RTN& ver_handle);
	TLV_HANDLE_RTN tlv_manage_lbs_handle(TLV_MANAGE_LBS& m_lbs, TLV_HANDLE_MANAGE_LBS& m_lbs_handle);
	TLV_HANDLE_RTN tlv_th2_handle(TLV_TH2& th, TLV_HANDLE_TH2& th_handle);
	TLV_HANDLE_RTN tlv_th2_handle_ver12(TLV_TH2_V12& th, TLV_HANDLE_TH2& th_handle);
	TLV_HANDLE_RTN tlv_exposure_handle(TLV_EXPOSURE& exposure_data, TLV_HANDLE_EXPOSURE& exposure_handle);
	TLV_HANDLE_RTN tlv_shock_handle(TLV_SHOCK& shock_data, TLV_HANDLE_SHOCK& shock_handle);
	TLV_HANDLE_RTN tlv_tilt_handle(TLV_TILT& tilt_data, TLV_HANDLE_TILT& tilt_handle);
	TLV_HANDLE_RTN tlv_baro_handle(TLV_BARO& baro_data, TLV_HANDLE_BARO& baro_handle);
	TLV_HANDLE_RTN tlv_band_time_handle(TLV_TIME_6& band_time, TLV_HANDLE_DEVBAND_TIME& band_time_handle);
public:
	TLV_HANDLE_RTN tlv_jzt_dev_handle(TLV_JZT_DEV& jzt_dev, TLV_HANDLE_JZT_DEV& jzt_dev_handle);
	TLV_HANDLE_RTN tlv_jzt_lbs_handle(TLV_JZT_LBS& lbs, TLV_HANDLE_JZT_LBS& lbs_handle);
	TLV_HANDLE_RTN tlv_jzt_th2_handle(TLV_JZT_TH2& th, TLV_HANDLE_JZT_TH2& th_handle);
};

#endif