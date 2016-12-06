/*
//
//	Last Modify Date: 2016-10-28
//	Author: zengpw
//	History:
//					2015-10-22 zengpw created
//	File Property: private
//	Future:
//					I. http 操作
//
//
*/

#include "http_interface.h"

namespace session
{
	bool HttpInterface::lbs2gps(TLV_LBS& lbs, string& latitude, string& longitude, string& url)
	{
		LOG_DEBUG("---------- lbs2gps filter : ");

		vector<LbsData> vLbs;

		for(unsigned int i = 0; i < sizeof(lbs.ld)/sizeof(LbsData); i++)
		{
			if(lbs.ld[i].lac != 0 && lbs.ld[i].cell_id != 0 && (int)lbs.ld[i].rssi != 0)
			{
				LOG_DEBUG("cell = " << (int)lbs.ld[i].cell << \
						", mnc = " << (int)lbs.ld[i].mnc << \
						", mcc = " << lbs.ld[i].mcc << \
						", lac = " << lbs.ld[i].lac << \
						", cell_id = " << lbs.ld[i].cell_id << \
						", bsic = " << (int)lbs.ld[i].bsic << \
						", rssi = " << (int)lbs.ld[i].rssi);

				vLbs.push_back(lbs.ld[i]);
			}
		}

		LOG_DEBUG("---------- end.");

		return lbs2gpsAmap(vLbs, latitude, longitude, url);
	}


	bool HttpInterface::lbs2gps(TLV_MANAGE_LBS& m_lbs, string& latitude, string& longitude, string& url)
	{
		LOG_DEBUG("---------- lbs2gps filter : ");

		vector<LbsData> vLbs;

		for(unsigned int i = 0; i < sizeof(m_lbs.ld)/sizeof(LbsData); i++)
		{
			if(m_lbs.ld[i].lac != 0 && m_lbs.ld[i].cell_id != 0 && (int)m_lbs.ld[i].rssi != 0)
			{
				LOG_DEBUG("cell = " << (int)m_lbs.ld[i].cell << \
						", mnc = " << (int)m_lbs.ld[i].mnc << \
						", mcc = " << m_lbs.ld[i].mcc << \
						", lac = " << m_lbs.ld[i].lac << \
						", cell_id = " << m_lbs.ld[i].cell_id << \
						", bsic = " << (int)m_lbs.ld[i].bsic << \
						", rssi = " << (int)m_lbs.ld[i].rssi);

				vLbs.push_back(m_lbs.ld[i]);
			}
		}

		LOG_DEBUG("---------- end.");

		return lbs2gpsAmap(vLbs, latitude, longitude, url);
	}


	bool HttpInterface::lbs2gps(TLV_JZT_LBS& lbs, string& latitude, string& longitude, string& url)
	{
		LOG_DEBUG("---------- lbs2gps filter : ");

		vector<LbsData> vLbs;

		for(unsigned int i = 0; i < sizeof(lbs.ld)/sizeof(LbsData); i++)
		{
			if(lbs.ld[i].lac != 0 && lbs.ld[i].cell_id != 0 && (int)lbs.ld[i].rssi != 0)
			{
				LOG_DEBUG("cell = " << (int)lbs.ld[i].cell << \
						", mnc = " << (int)lbs.ld[i].mnc << \
						", mcc = " << lbs.ld[i].mcc << \
						", lac = " << lbs.ld[i].lac << \
						", cell_id = " << lbs.ld[i].cell_id << \
						", bsic = " << (int)lbs.ld[i].bsic << \
						", rssi = " << (int)lbs.ld[i].rssi);

				vLbs.push_back(lbs.ld[i]);
			}
		}

		LOG_DEBUG("---------- end.");

		return lbs2gpsAmap(vLbs, latitude, longitude, url);
	}


	bool HttpInterface::lbs2gpsAmap(vector<LbsData> &vLbs, string& latitude, string& longitude, string& url)
	{
		stringstream ss;

		string bts = "";
		string nearbts = "";
		string request = "";

		if(vLbs.size() > 0)
		{
			ss << (int)vLbs[0].mcc;
			bts += ss.str() + ",";
			ss.str("");
			ss.clear();

			ss << (int)vLbs[0].mnc;
			bts += ss.str() + ",";
			ss.str("");
			ss.clear();

			ss << (int)vLbs[0].lac;
			bts += ss.str() + ",";
			ss.str("");
			ss.clear();

			ss << (int)vLbs[0].cell_id;
			bts += ss.str() + ",";
			ss.str("");
			ss.clear();

			ss << (int)vLbs[0].rssi - 111;
			bts += ss.str();
			ss.str("");
			ss.clear();

			for(unsigned int i = 1; i < vLbs.size(); i++)
			{
				ss << (int)vLbs[0].mcc;
				nearbts += ss.str() + ",";
				ss.str("");
				ss.clear();

				ss << (int)vLbs[0].mnc;
				nearbts += ss.str() + ",";
				ss.str("");
				ss.clear();

				ss << (int)vLbs[0].lac;
				nearbts += ss.str() + ",";
				ss.str("");
				ss.clear();

				ss << (int)vLbs[0].cell_id;
				nearbts += ss.str() + ",";
				ss.str("");
				ss.clear();

				ss << (int)vLbs[0].rssi - 111;
				nearbts += ss.str() + "|";
				ss.str("");
				ss.clear();
			}

			request = "http://apilocate.amap.com/position?accesstype=0&imei=866104021101384&cdma=0&bts=";
			request += bts;

			if(nearbts.length() > 0)
			{
				nearbts = nearbts.substr(0, nearbts.length() - 1);

				request += "&nearbts=";
				request += nearbts;
			}

			request += "&output=json&key=fe0d26ade7f0dc72e2d2efa4d10fc259";
			url = request;
			LOG_DEBUG("request : " << request);

			if(!python_call.py_requestAmap(request, latitude, longitude))
			{
				return false;
			}
		}
		else
		{
			//如果是整个 LBS 包被过滤了，则返回全0
			latitude = "000000000000";
			longitude = "000000000000";
		}

		return true;
	}


	bool HttpInterface::gps2addrAmap(const string& latitude, const string& longitude, string& addr)
	{
		//地址转换
		char buf[24];
		stringstream ss;
		double dx, dy;
		string x, y;

		// ss << latitude;
		// ss >> dx;
		// ss.str("");
		// ss.clear();

		// //最前面的 0 在转换成 double 类型时丢失
		// memset(buf, 0, sizeof(buf));
		// // sprintf(buf, "%3.3f", dx/100000/10000);
		// sprintf(buf, "%3.3f", dx);
		// ss << buf;
		// ss >> x;
		// ss.str("");
		// ss.clear();

		// ss << longitude;
		// ss >> dy;
		// ss.str("");
		// ss.clear();

		// //最前面的 0 在转换成 double 类型时丢失
		// memset(buf, 0, sizeof(buf));
		// // sprintf(buf, "%3.3f", dy/100000/10000);
		// sprintf(buf, "%3.3f", dy);
		// ss << buf;
		// ss >> y;
		// ss.str("");
		// ss.clear();

		ss << latitude;
		ss >> x;
		ss.str("");
		ss.clear();

		ss << longitude;
		ss >> y;
		ss.str("");
		ss.clear();

		//查询
		string request;

		request = "http://restapi.amap.com/v3/geocode/regeo?location=";
		request += y + ",";
		request += x;
		request += "&extensions=all&output=json&key=44e606a9c5a628e9a8bd42226249f84c";

		LOG_DEBUG("request : " << request);

		addr = "";
		if(python_call.py_requestAmapUseAddr(request, addr))
		{
			// if(addr.length() _EQ_ 0)
			// {
			// 	addr = "未获取到定位数据";
			// }

			return true;
		}

		// if(addr.length() _EQ_ 0)
		// {
		// 	addr = "未获取到定位数据";
		// }

		return false;
	}


	bool HttpInterface::notifyJava(const string& java_addr, const string& dev_id, const string& type, const string& value, const string& region, const string& up_date)
	{
		//发送告警短信
		string request;
		string result;

		request = "http://";
		request += java_addr + "/dplus/corp/linkman/sendMsg_v2.jspa?deviceId=";
		request += dev_id + "&msgType=";
		request += type + "&msgValue=";
		request += value + "&regionType=";
		request += region + "&updateDate=";
		request += up_date;

		LOG_DEBUG("request : " << request);

		if(python_call.py_notifyJava(request, result))
		{
			LOG_DEBUG("result: " << result);

			return true;
		}

		return false;
	}


	bool HttpInterface::notifyJavaCreateOrder(const string& java_addr, const string& dev_id)
	{
		//创建订单
		string request;
		string result;

		request = "http://";
		request += java_addr + "/dplus/order/order/createOrder.jspa?deviceId=";
		request += dev_id;

		LOG_DEBUG("request : " << request);

		if(python_call.py_notifyJava(request, result))
		{
			LOG_DEBUG("result: " << result);

			return true;
		}

		return false;
	}


	bool HttpInterface::notifyJavaCreateOrderTime(const string& java_addr, const string& dev_id, const string& datetime)
	{
		//创建订单
		string request;
		string result;

		request = "http://";
		request += java_addr + "/dplus/order/order/createOrderV2.jspa?deviceId=";
		request += dev_id;
		request += "&startTime=";
		request += datetime;

		LOG_DEBUG("request : " << request);

		if(python_call.py_notifyJava(request, result))
		{
			LOG_DEBUG("result: " << result);

			return true;
		}

		return false;
	}


	bool HttpInterface::notifyJavaQueryFence(const string& java_addr, const string& dev_id, const string& latitude, const string& longitude, int& rtn)
	{
		//查询电子围栏
		string request;
		string result;
		string code;

		request = "http://";
		request += java_addr + "/dplus/system/isInFence.jspa?deviceId=";
		request += dev_id;
		request += "&lng=";
		request += longitude;
		request += "&lat=";
		request += latitude;

		LOG_DEBUG("request : " << request);

		//默认是不发送围栏指令
		rtn = -1;

		if(python_call.py_notifyJavaEnhance(request, result, code))
		{
			LOG_DEBUG("result: " << result << ", code: " << code);

			if(code == "10000")
			{
				if(result == "true")
					rtn = 1;

				if(result == "false")
					rtn = 0;
			}

			return true;
		}

		return false;
	}
};// namespace session