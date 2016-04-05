/*
//
//	Last Modify Date: 2016-03-29
//	Author: zengpw
//	History:
//					2015-10-20 zengpw created
//	File Property: private
//	Future:
//					I. tlv 消息处理
//
//
*/

#include "tlv_handle.h"

TLV_HANDLE_RTN TlvHandle::basic_data_handle(BasicData& bd, HANDLE_BasicData& hbd)
{
	char buf[64];
	stringstream ss;

	//场强
	ss << (int)bd.rssi;
	hbd.rssi = ss.str();
	ss.str("");
	ss.clear();

	//移动标记
	ss << (int)bd.move;
	hbd.move = ss.str();
	ss.str("");
	ss.clear();

	//光曝告警标记
	ss << (int)bd.flag;
	hbd.flag = ss.str();
	ss.str("");
	ss.clear();

	if(hbd.flag == "1")
	{
		hbd.alarm = "2";
	}

	//电压
	ss << bd.voltage;
	hbd.voltage = ss.str();
	ss.str("");
	ss.clear();

	//温度
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)bd.temperature/10);

	ss << buf;
	hbd.temperature = ss.str();
	ss.str("");
	ss.clear();

	//光曝
	ss << bd.exposure;
	hbd.exposure = ss.str();
	ss.str("");
	ss.clear();

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_firm_handle(TLV_DEV_FIRM& firm, TLV_HANDLE_DEV_FIRM& firm_handle)
{
	LOG_DEBUG("====== tlv_firm_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_DEV_FIRM content : ");
	LOG_DEBUG("version = " << firm.version << \
			", imei = " << firm.imei << \
			", iccid = " << firm.iccid << \
			", mac = " << firm.mac);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	stringstream ss;

	ss << firm.version;
	firm_handle.version = ss.str();
	ss.str("");
	ss.clear();

	// imei[15] 通常被写满数据，从而导致没有 '\0' 结束符
	if(firm.imei[0] != '\0')
		firm_handle.imei.assign((char*)firm.imei, sizeof(firm.imei));
	else
		firm_handle.imei = "0";

	if(firm.iccid[0] != '\0')
		firm_handle.iccid.assign((char*)firm.iccid, sizeof(firm.iccid));
	else
		firm_handle.iccid = "0";

	if(firm.mac[0] != '\0')
		firm_handle.mac.assign((char*)firm.mac, sizeof(firm.mac));
	else
		firm_handle.mac = "0";

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_DEV_FIRM content : ");
	LOG_DEBUG("dev_id = " << firm_handle.dev_id << \
			", version = " << firm_handle.version << \
			", imei = " << firm_handle.imei << \
			", iccid = " << firm_handle.iccid << \
			", mac = " << firm_handle.mac);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_gps_handle(TLV_GPS& gps, TLV_HANDLE_GPS& gps_handle)
{
	LOG_DEBUG("====== tlv_gps_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_GPS content : ");
	printDateTime2k(gps.dt);
	LOG_DEBUG("rssi = " << (int)gps.bd.rssi << \
			", move = " << (int)gps.bd.move << \
			", flag = " << (int)gps.bd.flag << \
			", voltage = " << gps.bd.voltage << \
			", temperature = " << gps.bd.temperature << \
			", exposure = " << gps.bd.exposure);
	LOG_DEBUG("sn_lat = " << (int)gps.sn_lat << \
			", ew_long = " << (int)gps.ew_long << \
			", latitude = " << gps.latitude << \
			", longitude = " << gps.longitude);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	char buf[64];
	stringstream ss;

	//时间
	gps_handle.datetime = DateTime2String2k(gps.dt);

	//基础数据
	if(basic_data_handle(gps.bd, gps_handle.hbd) _NEQ_ THR_OK)
	{
		return THR_FAIL;
	}

	//经纬度，需要补长度到 12 位
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%08u", gps.latitude);
	ss << buf;
	gps_handle.latitude = ss.str();
	gps_handle.latitude += "000000000000";
	gps_handle.latitude = gps_handle.latitude.substr(0, 12);
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%8u", gps.longitude);
	ss << buf;
	gps_handle.longitude = ss.str();
	gps_handle.longitude += "000000000000";
	gps_handle.longitude = gps_handle.longitude.substr(0, 12);
	ss.str("");
	ss.clear();

	//查询告警地址
	session::HttpInterface hi;
	hi.gps2addrAmap(gps_handle.latitude, gps_handle.longitude, gps_handle.hbd.addr);

	//额外的数据
	gps_handle.mode = "0";

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_GPS content : ");
	LOG_DEBUG("dev_id = " << gps_handle.dev_id << \
			", order_id = " << gps_handle.order_id << \
			", datetime = " << gps_handle.datetime);
	LOG_DEBUG("rssi = " << gps_handle.hbd.rssi << \
			", move = " << gps_handle.hbd.move << \
			", flag = " << gps_handle.hbd.flag << \
			", voltage = " << gps_handle.hbd.voltage << \
			", temperature = " << gps_handle.hbd.temperature << \
			", exposure = " << gps_handle.hbd.exposure);
	LOG_DEBUG("latitude = " << gps_handle.latitude << \
			", longitude = " << gps_handle.longitude);
	LOG_DEBUG("alarm = " << gps_handle.hbd.alarm << \
			", addr = " << gps_handle.hbd.addr);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_lbs_handle(TLV_LBS& lbs, TLV_HANDLE_LBS& lbs_handle)
{
	LOG_DEBUG("====== tlv_lbs_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_LBS content : ");
	printDateTime2k(lbs.dt);
	LOG_DEBUG("rssi = " << (int)lbs.bd.rssi << \
			", move = " << (int)lbs.bd.move << \
			", flag = " << (int)lbs.bd.flag << \
			", voltage = " << lbs.bd.voltage << \
			", temperature = " << lbs.bd.temperature << \
			", exposure = " << lbs.bd.exposure);

	for(unsigned int i = 0; i < sizeof(lbs.ld)/sizeof(LbsData); i++)
	{
		LOG_DEBUG("cell = " << (int)lbs.ld[i].cell << \
				", mnc = " << (int)lbs.ld[i].mnc << \
				", mcc = " << lbs.ld[i].mcc << \
				", lac = " << lbs.ld[i].lac << \
				", cell_id = " << lbs.ld[i].cell_id << \
				", bsic = " << (int)lbs.ld[i].bsic << \
				", rssi = " << (int)lbs.ld[i].rssi);
	}

	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	char buf[64];
	stringstream ss;

	//时间
	lbs_handle.datetime = DateTime2String2k(lbs.dt);

	//基础数据
	if(basic_data_handle(lbs.bd, lbs_handle.hbd) _NEQ_ THR_OK)
	{
		return THR_FAIL;
	}

	// LBS 转经纬度
	session::HttpInterface hi;
	if(!hi.lbs2gps(lbs, lbs_handle.latitude, lbs_handle.longitude, lbs_handle.url))
	{
		return THR_FAIL;
		// lbs_handle.latitude = "000000000000";
		// lbs_handle.longitude = "000000000000";
	}
	else
	{
		//移除字符 '.'
		string::iterator it_lat = remove_if(lbs_handle.latitude.begin(), lbs_handle.latitude.end(), bind2nd(equal_to<char>(), '.'));
		lbs_handle.latitude.erase(it_lat, lbs_handle.latitude.end());
		string::iterator it_longi = remove_if(lbs_handle.longitude.begin(), lbs_handle.longitude.end(), bind2nd(equal_to<char>(), '.'));
		lbs_handle.longitude.erase(it_longi, lbs_handle.longitude.end());

		//补到12位长度
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "0%s", lbs_handle.latitude.c_str());
		ss << buf;
		lbs_handle.latitude = ss.str() + "000000000000";
		lbs_handle.latitude = lbs_handle.latitude.substr(0, 12);
		ss.str("");
		ss.clear();

		lbs_handle.longitude += "000000000000";
		lbs_handle.longitude = lbs_handle.longitude.substr(0, 12);
	}

	//查询告警地址
	hi.gps2addrAmap(lbs_handle.latitude, lbs_handle.longitude, lbs_handle.hbd.addr);

	//额外的数据
	lbs_handle.mode = "1";

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_LBS content : ");
	LOG_DEBUG("dev_id = " << lbs_handle.dev_id << \
			", order_id = " << lbs_handle.order_id << \
			", datetime = " << lbs_handle.datetime);
	LOG_DEBUG("rssi = " << lbs_handle.hbd.rssi << \
			", move = " << lbs_handle.hbd.move << \
			", flag = " << lbs_handle.hbd.flag << \
			", voltage = " << lbs_handle.hbd.voltage << \
			", temperature = " << lbs_handle.hbd.temperature << \
			", exposure = " << lbs_handle.hbd.exposure);
	LOG_DEBUG("latitude = " << lbs_handle.latitude << \
			", longitude = " << lbs_handle.longitude);
	LOG_DEBUG("alarm = " << lbs_handle.hbd.alarm << \
			", addr = " << lbs_handle.hbd.addr);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_th_handle(TLV_TH& th, TLV_HANDLE_TH& th_handle)
{
	LOG_DEBUG("====== tlv_th_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_TH content : ");
	printDateTime2k(th.dt);
	printDateTime2k(th.dt_gps);
	LOG_DEBUG("alarm = " << (int)th.alarm << \
			", t0 = " << th.t0 << \
			", h0 = " << th.h0);

	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	char buf[64];
	stringstream ss;

	th_handle.datetime = DateTime2String2k(th.dt);
	th_handle.datetime_gps = DateTime2String2k(th.dt_gps);
	th_handle.alarm = common::uchar2binary(th.alarm);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)th.t0/10);

	ss << buf;
	th_handle.t0 = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)th.h0/10);

	ss << buf;
	th_handle.h0 = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 生成额外数据
	if(th.t0 > 800 || th.t0 < -800)
		th_handle.overflow = "1";
	else
		th_handle.overflow = "0";

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_TH content : ");
	LOG_DEBUG("dev_id = " << th_handle.dev_id << \
			", order_id = " << th_handle.order_id << \
			", datetime = " << th_handle.datetime);
	LOG_DEBUG("datetime_gps = " << th_handle.datetime_gps);
	LOG_DEBUG("alarm = " << th_handle.alarm << \
			", t0 = " << th_handle.t0 << \
			", h0 = " << th_handle.h0 << \
			", overflow = " << th_handle.overflow);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_unband_handle(TLV_RTN& rtn, TLV_HANDLE_UNBAND_RTN& unband_handle)
{
	LOG_DEBUG("====== tlv_unband_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_RTN content : ");
	LOG_DEBUG("rtn = " << (int)rtn);
	LOG_DEBUG("---------- end.");

	stringstream ss;

	ss << (int)rtn;
	unband_handle.rtn = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_UNBAND_RTN content : ");
	LOG_DEBUG("dev_id = " << unband_handle.dev_id);
	LOG_DEBUG("rtn = " << unband_handle.rtn);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_dev_unband_handle(TLV_TIME& tm, TLV_HANDLE_DEV_UNBAND_RTN& dev_unband_handle)
{
	LOG_DEBUG("====== tlv_unband_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_TIME content : ");
	printDateTimeSec(tm);
	LOG_DEBUG("---------- end.");

	dev_unband_handle.datetime = DateTime2StringSec(tm);

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_DEV_UNBAND_RTN content : ");
	LOG_DEBUG("dev_id = " << dev_unband_handle.dev_id);
	LOG_DEBUG("datetime = " << dev_unband_handle.datetime);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_dev_msg_handle(TLV_DEV_MSG& tdm, TLV_HANDLE_DEV_MSG& dev_msg_handle)
{
	LOG_DEBUG("====== tlv_dev_msg_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_DEV_MSG content : ");
	LOG_DEBUG("state = " << (int)tdm.state << \
			", rssi = " << (int)tdm.rssi << \
			", version = " << tdm.version << \
			", voltage = " << tdm.voltage << \
			", flag = " << tdm.flag);
	LOG_DEBUG("---------- end.");

	stringstream ss;

	ss << (int)tdm.state;
	dev_msg_handle.state = ss.str();
	ss.str("");
	ss.clear();

	ss << (int)tdm.rssi;
	dev_msg_handle.rssi = ss.str();
	ss.str("");
	ss.clear();

	ss << tdm.version;
	dev_msg_handle.version = ss.str();
	ss.str("");
	ss.clear();

	ss << tdm.voltage;
	dev_msg_handle.voltage = ss.str();
	ss.str("");
	ss.clear();

	ss << tdm.flag;
	dev_msg_handle.flag = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_DEV_MSG content : ");
	LOG_DEBUG("dev_id = " << dev_msg_handle.dev_id << \
			", state = " << dev_msg_handle.state << \
			", rssi = " << dev_msg_handle.rssi << \
			", version = " << dev_msg_handle.version << \
			", voltage = " << dev_msg_handle.voltage << \
			", flag = " << dev_msg_handle.flag);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_parameter_handle(TLV_RTN& rtn, TLV_HANDLE_PARAMETER_RTN& parameter_handle)
{
	LOG_DEBUG("====== tlv_parameter_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_RTN content : ");
	LOG_DEBUG("rtn = " << (int)rtn);
	LOG_DEBUG("---------- end.");

	stringstream ss;

	ss << (int)rtn;
	parameter_handle.rtn = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_PARAMETER_RTN content : ");
	LOG_DEBUG("dev_id = " << parameter_handle.dev_id);
	LOG_DEBUG("rtn = " << parameter_handle.rtn);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_version_handle(TLV_RTN& rtn, TLV_HANDLE_VERSION_RTN& ver_handle)
{
	LOG_DEBUG("====== tlv_version_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_RTN content : ");
	LOG_DEBUG("rtn = " << (int)rtn);
	LOG_DEBUG("---------- end.");

	stringstream ss;

	ss << (int)rtn;
	ver_handle.rtn = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_VERSION_RTN content : ");
	LOG_DEBUG("dev_id = " << ver_handle.dev_id);
	LOG_DEBUG("rtn = " << ver_handle.rtn);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_manage_lbs_handle(TLV_MANAGE_LBS& m_lbs, TLV_HANDLE_MANAGE_LBS& m_lbs_handle)
{
	LOG_DEBUG("====== tlv_lbs_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_MANAGE_LBS content : ");
	printDateTime2k(m_lbs.dt);
	LOG_DEBUG("rssi = " << (int)m_lbs.rssi << \
			", state = " << (int)m_lbs.state << \
			", voltage = " << m_lbs.voltage << \
			", temperature = " << m_lbs.temperature);

	for(unsigned int i = 0; i < sizeof(m_lbs.ld)/sizeof(LbsData); i++)
	{
		LOG_DEBUG("cell = " << (int)m_lbs.ld[i].cell << \
				",mnc = " << (int)m_lbs.ld[i].mnc << \
				",mcc = " << m_lbs.ld[i].mcc << \
				",lac = " << m_lbs.ld[i].lac << \
				",cell_id = " << m_lbs.ld[i].cell_id << \
				",bsic = " << (int)m_lbs.ld[i].bsic << \
				", rssi = " << (int)m_lbs.ld[i].rssi);
	}

	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	char buf[64];
	stringstream ss;

	//时间
	m_lbs_handle.datetime = DateTime2String2k(m_lbs.dt);

	ss << (int)m_lbs.rssi;
	m_lbs_handle.rssi = ss.str();
	ss.str("");
	ss.clear();

	ss << (int)m_lbs.state;
	m_lbs_handle.state = ss.str();
	ss.str("");
	ss.clear();

	ss << m_lbs.voltage;
	m_lbs_handle.voltage = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)m_lbs.temperature/10);

	ss << buf;
	m_lbs_handle.temperature = ss.str();
	ss.str("");
	ss.clear();

	// LBS 转经纬度
	session::HttpInterface hi;
	if(!hi.lbs2gps(m_lbs, m_lbs_handle.latitude, m_lbs_handle.longitude, m_lbs_handle.url))
	{
		m_lbs_handle.latitude = "000000000000";
		m_lbs_handle.longitude = "000000000000";
	}
	else
	{
		//移除字符 '.'
		string::iterator it_lat = remove_if(m_lbs_handle.latitude.begin(), m_lbs_handle.latitude.end(), bind2nd(equal_to<char>(), '.'));
		m_lbs_handle.latitude.erase(it_lat, m_lbs_handle.latitude.end());
		string::iterator it_longi = remove_if(m_lbs_handle.longitude.begin(), m_lbs_handle.longitude.end(), bind2nd(equal_to<char>(), '.'));
		m_lbs_handle.longitude.erase(it_longi, m_lbs_handle.longitude.end());

		//补到12位长度
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "0%s", m_lbs_handle.latitude.c_str());
		ss << buf;
		m_lbs_handle.latitude = ss.str() + "000000000000";
		m_lbs_handle.latitude = m_lbs_handle.latitude.substr(0, 12);
		ss.str("");
		ss.clear();

		m_lbs_handle.longitude += "000000000000";
		m_lbs_handle.longitude = m_lbs_handle.longitude.substr(0, 12);
	}

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_MANAGE_LBS content : ");
	LOG_DEBUG("dev_id = " << m_lbs_handle.dev_id << \
			", datetime = " << m_lbs_handle.datetime);
	LOG_DEBUG("rssi = " << m_lbs_handle.rssi << \
			", state = " << m_lbs_handle.state << \
			", voltage = " << m_lbs_handle.voltage << \
			", temperature = " << m_lbs_handle.temperature);
	LOG_DEBUG("latitude = " << m_lbs_handle.latitude << \
			", longitude = " << m_lbs_handle.longitude);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_th2_handle(TLV_TH2& th, TLV_HANDLE_TH2& th_handle)
{
	LOG_DEBUG("====== tlv_th2_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_TH2 content : ");
	printDateTime2k(th.dt);
	printDateTime2k(th.dt_gps);
	LOG_DEBUG("alarm = " << (int)th.alarm << \
			", t0 = " << th.t0 << \
			", h0 = " << th.h0 << \
			", t1 = " << th.t1 << \
			", h1 = " << th.h1);

	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	char buf[64];
	stringstream ss;

	th_handle.datetime = DateTime2String2k(th.dt);
	th_handle.datetime_gps = DateTime2String2k(th.dt_gps);
	th_handle.alarm = common::uchar2binary(th.alarm);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)th.t0/10);

	ss << buf;
	th_handle.t0 = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)th.h0/10);

	ss << buf;
	th_handle.h0 = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)th.t1/10);

	ss << buf;
	th_handle.t1 = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.1f", (float)th.h1/10);

	ss << buf;
	th_handle.h1 = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 生成额外数据
	if(th.t0 > 800 || th.t0 < -800 || th.t1 > 800 || th.t1 < -800)
		th_handle.overflow = "1";
	else
		th_handle.overflow = "0";

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_TH2 content : ");
	LOG_DEBUG("dev_id = " << th_handle.dev_id << \
			", order_id = " << th_handle.order_id << \
			", datetime = " << th_handle.datetime);
	LOG_DEBUG("datetime_gps = " << th_handle.datetime_gps);
	LOG_DEBUG("alarm = " << th_handle.alarm << \
			", t0 = " << th_handle.t0 << \
			", h0 = " << th_handle.h0 << \
			", t1 = " << th_handle.t1 << \
			", h1 = " << th_handle.h1 << \
			", overflow = " << th_handle.overflow);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_exposure_handle(TLV_EXPOSURE& exposure_data, TLV_HANDLE_EXPOSURE& exposure_handle)
{
	LOG_DEBUG("====== tlv_exposure_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_EXPOSURE content : ");
	printDateTime2k(exposure_data.dt);
	printDateTime2k(exposure_data.dt_gps);
	LOG_DEBUG("alarm = " << (int)exposure_data.alarm << \
			", exposure = " << exposure_data.exposure);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	stringstream ss;

	exposure_handle.datetime = DateTime2String2k(exposure_data.dt);
	exposure_handle.datetime_gps = DateTime2String2k(exposure_data.dt_gps);

	ss << (int)exposure_data.alarm;
	exposure_handle.alarm = ss.str();
	ss.str("");
	ss.clear();

	ss << exposure_data.exposure;
	exposure_handle.exposure = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_EXPOSURE content : ");
	LOG_DEBUG("dev_id = " << exposure_handle.dev_id << \
			", order_id = " << exposure_handle.order_id << \
			", datetime = " << exposure_handle.datetime);
	LOG_DEBUG("datetime_gps = " << exposure_handle.datetime_gps);
	LOG_DEBUG("alarm = " << exposure_handle.alarm << \
			", exposure = " << exposure_handle.exposure);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_shock_handle(TLV_SHOCK& shock_data, TLV_HANDLE_SHOCK& shock_handle)
{
	LOG_DEBUG("====== tlv_shock_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_SHOCK content : ");
	printDateTime2k(shock_data.dt);
	printDateTime2k(shock_data.dt_gps);
	LOG_DEBUG("alarm = " << (int)shock_data.alarm << \
			", x = " << shock_data.x << \
			", y = " << shock_data.y << \
			", z = " << shock_data.z << \
			", max = " << shock_data.max);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	char buf[64];
	stringstream ss;

	shock_handle.datetime = DateTime2String2k(shock_data.dt);
	shock_handle.datetime_gps = DateTime2String2k(shock_data.dt_gps);

	ss << (int)shock_data.alarm;
	shock_handle.alarm = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%2.3f", (float)shock_data.x/1000);

	ss << buf;
	shock_handle.x = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%2.3f", (float)shock_data.y/1000);

	ss << buf;
	shock_handle.y = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%2.3f", (float)shock_data.z/1000);

	ss << buf;
	shock_handle.z = ss.str();
	ss.str("");
	ss.clear();

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%2.3f", (float)shock_data.max/1000);

	ss << buf;
	shock_handle.max = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_SHOCK content : ");
	LOG_DEBUG("dev_id = " << shock_handle.dev_id << \
			", order_id = " << shock_handle.order_id << \
			", datetime = " << shock_handle.datetime);
	LOG_DEBUG("datetime_gps = " << shock_handle.datetime_gps);
	LOG_DEBUG("alarm = " << shock_handle.alarm << \
			", x = " << shock_handle.x << \
			", y = " << shock_handle.y << \
			", z = " << shock_handle.z << \
			", max = " << shock_handle.max);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_tilt_handle(TLV_TILT& tilt_data, TLV_HANDLE_TILT& tilt_handle)
{
	LOG_DEBUG("====== tlv_tilt_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_TILT content : ");
	printDateTime2k(tilt_data.dt);
	printDateTime2k(tilt_data.dt_gps);
	LOG_DEBUG("alarm = " << (int)tilt_data.alarm << \
			", x = " << tilt_data.x << \
			", y = " << tilt_data.y << \
			", z = " << tilt_data.z << \
			", max = " << tilt_data.max);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	stringstream ss;

	tilt_handle.datetime = DateTime2String2k(tilt_data.dt);
	tilt_handle.datetime_gps = DateTime2String2k(tilt_data.dt_gps);

	ss << (int)tilt_data.alarm;
	tilt_handle.alarm = ss.str();
	ss.str("");
	ss.clear();

	ss << tilt_data.x;
	tilt_handle.x = ss.str();
	ss.str("");
	ss.clear();

	ss << tilt_data.y;
	tilt_handle.y = ss.str();
	ss.str("");
	ss.clear();

	ss << tilt_data.z;
	tilt_handle.z = ss.str();
	ss.str("");
	ss.clear();

	ss << tilt_data.max;
	tilt_handle.max = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_TILT content : ");
	LOG_DEBUG("dev_id = " << tilt_handle.dev_id << \
			", order_id = " << tilt_handle.order_id << \
			", datetime = " << tilt_handle.datetime);
	LOG_DEBUG("datetime_gps = " << tilt_handle.datetime_gps);
	LOG_DEBUG("alarm = " << tilt_handle.alarm << \
			", x = " << tilt_handle.x << \
			", y = " << tilt_handle.y << \
			", z = " << tilt_handle.z << \
			", max = " << tilt_handle.max);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}


TLV_HANDLE_RTN TlvHandle::tlv_baro_handle(TLV_BARO& baro_data, TLV_HANDLE_BARO& baro_handle)
{
	LOG_DEBUG("====== tlv_baro_handle processing ======");

	//-------------- 打印入参
	LOG_DEBUG("---------- TLV_BARO content : ");
	printDateTime2k(baro_data.dt);
	printDateTime2k(baro_data.dt_gps);
	LOG_DEBUG("alarm = " << (int)baro_data.alarm << \
			", baro = " << baro_data.baro);
	LOG_DEBUG("---------- end.");

	//-------------- 处理参数
	stringstream ss;

	baro_handle.datetime = DateTime2String2k(baro_data.dt);
	baro_handle.datetime_gps = DateTime2String2k(baro_data.dt_gps);

	ss << (int)baro_data.alarm;
	baro_handle.alarm = ss.str();
	ss.str("");
	ss.clear();

	ss << baro_data.baro;
	baro_handle.baro = ss.str();
	ss.str("");
	ss.clear();

	//-------------- 打印出参
	LOG_DEBUG("---------- TLV_HANDLE_BARO content : ");
	LOG_DEBUG("dev_id = " << baro_handle.dev_id << \
			", order_id = " << baro_handle.order_id << \
			", datetime = " << baro_handle.datetime);
	LOG_DEBUG("datetime_gps = " << baro_handle.datetime_gps);
	LOG_DEBUG("alarm = " << baro_handle.alarm << \
			", baro = " << baro_handle.baro);
	LOG_DEBUG("---------- end.");

	return THR_OK;
}