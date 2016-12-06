/*
//
//	Last Modify Date: 2016-12-02
//	Author: zengpw
//	History:
//					2015-10-21 zengpw created
//	File Property: private
//	Future:
//					I. 数据库操作
//
//
*/

#include "db_interface.h"

TLV_HANDLE_RTN DbInterface::firm_update(common::DB* db, TLV_HANDLE_DEV_FIRM& firm_handle)
{
	string sql;

	sql = "update t_device ";
	sql += "set fd_version = '";
	sql += firm_handle.version + "', fd_imei = '";
	sql += firm_handle.imei + "', fd_phone_cid = '";
	sql += firm_handle.iccid + "' where fd_id = '";
	sql += firm_handle.dev_id + "'";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;	
}


TLV_HANDLE_RTN DbInterface::firm_update_ver11(common::DB* db, TLV_HANDLE_DEV_FIRM_V11& firm_handle)
{
	string sql;

	sql = "update t_device ";
	sql += "set fd_version = '";
	sql += firm_handle.version + "', fd_imei = '";
	sql += firm_handle.imei + "', fd_phone_cid = '";
	sql += firm_handle.iccid + "', fd_mac = '";
	sql += firm_handle.mac + "' where fd_id = '";
	sql += firm_handle.dev_id + "'";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::gps_update(common::DB* db, TLV_HANDLE_GPS& gps_handle)
{
	if(common::dateTimeFilter(gps_handle.datetime))
	{
		LOG_DEBUG("dev_id = " << gps_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//谷歌地球gps数据 转 高德数据
	char buf[24];
	stringstream ss;

	// latitude 纬度范围 -90° ~ +90°
	// longitude 经度范围 -180° ~ +180°
	string partition_x = gps_handle.latitude.substr(1, 2);
	string partition_y = gps_handle.longitude.substr(0, 3);

	double dx, dy;
	string x, y;

	ss << gps_handle.latitude;
	ss >> dx;
	ss.str("");
	ss.clear();

	// 11位数字（第1位是0，在转换成 double 类型时丢失），去掉后面的0；剩下7位数字，然后除以 100000
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.3f", dx/100000/10000);
	ss << buf;
	ss >> x;
	ss.str("");
	ss.clear();

	ss << gps_handle.longitude;
	ss >> dy;
	ss.str("");
	ss.clear();

	// 12位数字，去掉后面的0；剩下8位数字，然后除以 100000
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%3.3f", dy/100000/10000);
	ss << buf;
	ss >> y;
	ss.str("");
	ss.clear();

	sql = "select fd_offset_lng, fd_offset_lat from t_gps_offset subpartition(offset_max_";
	sql += partition_y + "_";
	sql += partition_x + ") where fd_lng = '";
	sql += y + "' and fd_lat = '";
	sql += x + "'";

	LOG_DEBUG("select sql: " << sql);

	if(!db->check())
	{
		return THR_ERROR;
	}

	try
	{
		otl_nocommit_stream os(1, sql.c_str(), *db->conn);

		try
		{
			db->conn->commit();

			while(!os.eof())
			{
				double offset_dx, offset_dy;
				string offset_x, offset_y;

				memset(buf, 0, sizeof(buf));
				os >> buf;
				offset_y = buf;

				memset(buf, 0, sizeof(buf));
				os >> buf;
				offset_x = buf;

				LOG_DEBUG("offset_x : " << offset_x);
				LOG_DEBUG("offset_y : " << offset_y);

				ss << offset_y;
				ss >> offset_dy;
				ss.str("");
				ss.clear();

				ss << offset_x;
				ss >> offset_dx;
				ss.str("");
				ss.clear();

				if(fabs(offset_dx) > 0.00000001 && fabs(offset_dy) > 0.00000001)
				{
					LOG_DEBUG("gps data need recalculate.");

					offset_dy = offset_dy*100000*10000;
					offset_dx = offset_dx*100000*10000;

					//加入偏移值
					dx += offset_dx;
					dy += offset_dy;

					//补到12位长度
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "0%11lf", dx);
					ss << buf;
					gps_handle.latitude = ss.str();
					gps_handle.latitude += "000000000000";
					gps_handle.latitude = gps_handle.latitude.substr(0, 12);
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					sprintf(buf, "%12lf", dy);
					ss << buf;
					gps_handle.longitude = ss.str();
					gps_handle.longitude += "000000000000";
					gps_handle.longitude = gps_handle.longitude.substr(0, 12);
					ss.str("");
					ss.clear();

					LOG_DEBUG("new latitude = " << gps_handle.latitude << ", new longitude = " << gps_handle.longitude);
				}
			}

			os.close();
		}
		catch(...)
		{
			os.close();
			throw;
		}
	}
	catch(otl_exception &p)
	{
		LOG_DEBUG("DB select exception!");
		LOG_DEBUG("conn.msg = " << p.msg);
		LOG_DEBUG("conn.stm_text = " << p.stm_text);
		LOG_DEBUG("conn.var_info = " << p.var_info);
		db->status = false;

		//ORA-03135: 连接失去联系
		//遇到此错误直接返回，不执行 rollback()
		if(strstr((char*)p.msg, "ORA-03135") != NULL)
			return THR_ERROR;

		db->conn->rollback();
		return THR_ERROR;
	}
	catch(...)
	{
		LOG_DEBUG("DB select occur unknown exception!");
		db->status = false;
		db->conn->rollback();
		return THR_ERROR;
	}

	//更新轨迹
	if(gps_handle.hbd.flag == "0")
	{
		sql = "insert into t_transway_info ";

		sql += "(FD_ID,";
		sql += "FD_ORDER_ID,";
		sql += "FD_GOODS_GPS_LONG,";
		sql += "FD_GOODS_GPS_LAT,";
		sql += "FD_GOODS_SENSOR_STATE,";
		sql += "FD_GOODS_GPS_MOD,";
		sql += "FD_GOODS_TEMPERATURE,";
		sql += "FD_GOODS_HUMIDITY,";
		sql += "FD_GOODS_ACCELER,";
		sql += "FD_GOODS_ACCELER_X,";
		sql += "FD_GOODS_ACCELER_Y,";
		sql += "FD_GOODS_ACCELER_Z,";
		sql += "FD_GOODS_EXPOSURE,";
		sql += "FD_GOODS_VOLTAGE,";
		sql += "FD_GPS_MODE,";
		sql += "FD_RESERVE,";
		sql += "FD_UPDATE_DATE,";
		sql += "FD_GOODS_ANGLE_X,";
		sql += "FD_GOODS_ANGLE_Y,";
		sql += "FD_GOODS_ANGLE_Z,";
		sql += "FD_MOVE_MARK,";
		sql += "GPS_ADDRESS";
		sql += ") values(";

		sql += "TRANS_INFO_SEQ.nextval, ";
		sql += "'";
		sql += gps_handle.order_id + "', '";
		sql += gps_handle.longitude + "', '";
		sql += gps_handle.latitude + "', ";
		sql += "null, ";
		sql += "null, '";
		sql += gps_handle.hbd.temperature + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += gps_handle.hbd.exposure + "', '";
		sql += gps_handle.hbd.voltage + "', '";
		sql += gps_handle.mode + "', '";
		sql += gps_handle.hbd.rssi + "', '";
		sql += gps_handle.datetime + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += gps_handle.hbd.move + "', '";
		sql += gps_handle.hbd.addr + "')";
	}
	else
	{
		session::HttpInterface hi;
		hi.gps2addrAmap(gps_handle.latitude, gps_handle.longitude, gps_handle.hbd.addr);

		LOG_DEBUG("alarm = " << gps_handle.hbd.alarm << ", addr = " << gps_handle.hbd.addr);

		sql = "insert into t_goods_warning ";

		sql += "(FD_ID,";
		sql += "FD_ORDER_ID,";
		sql += "FD_GOODS_GPS_LONG,";
		sql += "FD_GOODS_GPS_LAT,";
		sql += "FD_GOODS_SENSOR_STATE,";
		sql += "FD_GOODS_GPS_MODE,";
		sql += "FD_GOODS_TEMPERATURE,";
		sql += "FD_GOODS_HUMIDITY,";
		sql += "FD_GOODS_ACCELER,";
		sql += "FD_GOODS_ACCELER_X,";
		sql += "FD_GOODS_ACCELER_Y,";
		sql += "FD_GOODS_ACCELER_Z,";
		sql += "FD_GOODS_EXPOSURE,";
		sql += "FD_GOODS_VOLTAGE,";
		sql += "FD_GPS_MODE,";
		sql += "FD_RESERVE,";
		sql += "FD_WARNING_TIME,";
		sql += "FD_GOODS_ANGLE_X,";
		sql += "FD_GOODS_ANGLE_Y,";
		sql += "FD_GOODS_ANGLE_Z,";
		sql += "FD_WARNING_REASON,";
		sql += "FD_WARNING_ADDR,";
		sql += "FD_WARNING_LEVEL,";
		sql += "FD_GOODS_STATE";
		sql += ") values(";

		sql += "GOODS_WARNING_SEQ.nextval, ";
		sql += "'";
		sql += gps_handle.order_id + "', '";
		sql += gps_handle.longitude + "', '";
		sql += gps_handle.latitude + "', ";
		sql += "null, ";
		sql += "null, '";
		sql += gps_handle.hbd.temperature + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += gps_handle.hbd.exposure + "', '";
		sql += gps_handle.hbd.voltage + "', '";
		sql += gps_handle.mode + "', '";
		sql += gps_handle.hbd.rssi + "', '";
		sql += gps_handle.datetime + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += gps_handle.hbd.alarm + "', '";
		sql += gps_handle.hbd.addr + "', '";
		sql += "1', '";
		sql += "-1')";
	}

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	//取分表号
	string sub = common::getMod(gps_handle.order_id, 10);

	sql = "insert into t_position_info_";
	sql += sub;
	sql += " (FD_ID,";
	sql += "FD_ORDER_ID,";
	sql += "FD_GOODS_GPS_LONG,";
	sql += "FD_GOODS_GPS_LAT,";
	sql += "FD_GOODS_VOLTAGE,";
	sql += "FD_GPS_MODE,";
	sql += "RSSI,";
	sql += "FD_UPDATE_DATE,";
	sql += "FD_INSERT_DATE,";
	sql += "GPS_ADDRESS,";
	sql += "WARNING_TYPE,";
	sql += "GPS_URL,";
	sql += "FD_DEVICE_ID";
	sql += ") values(";

	sql += "position_info_seq.nextval, '";
	sql += gps_handle.order_id + "', '";
	sql += gps_handle.longitude + "', '";
	sql += gps_handle.latitude + "', '";
	sql += gps_handle.hbd.voltage + "', '";
	sql += gps_handle.mode + "', '";
	sql += gps_handle.hbd.rssi + "', '";
	sql += gps_handle.datetime + "', ";
	sql += "to_char(sysdate, 'YYYYMMDDHH24MISS'), '";
	sql += gps_handle.hbd.addr + "', ";
	sql += "'00000000', ";
	sql += "null, '";
	sql += gps_handle.dev_id + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::lbs_update(common::DB* db, TLV_HANDLE_LBS& lbs_handle)
{
	if(common::dateTimeFilter(lbs_handle.datetime))
	{
		LOG_DEBUG("dev_id = " << lbs_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//更新轨迹
	if(lbs_handle.hbd.flag == "0")
	{
		sql = "insert into t_transway_info ";

		sql += "(FD_ID,";
		sql += "FD_ORDER_ID,";
		sql += "FD_GOODS_GPS_LONG,";
		sql += "FD_GOODS_GPS_LAT,";
		sql += "FD_GOODS_SENSOR_STATE,";
		sql += "FD_GOODS_GPS_MOD,";
		sql += "FD_GOODS_TEMPERATURE,";
		sql += "FD_GOODS_HUMIDITY,";
		sql += "FD_GOODS_ACCELER,";
		sql += "FD_GOODS_ACCELER_X,";
		sql += "FD_GOODS_ACCELER_Y,";
		sql += "FD_GOODS_ACCELER_Z,";
		sql += "FD_GOODS_EXPOSURE,";
		sql += "FD_GOODS_VOLTAGE,";
		sql += "FD_GPS_MODE,";
		sql += "FD_RESERVE,";
		sql += "FD_UPDATE_DATE,";
		sql += "FD_GOODS_ANGLE_X,";
		sql += "FD_GOODS_ANGLE_Y,";
		sql += "FD_GOODS_ANGLE_Z,";
		sql += "FD_MOVE_MARK,";
		sql += "GPS_ADDRESS";
		sql += ") values(";

		sql += "TRANS_INFO_SEQ.nextval, ";
		sql += "'";
		sql += lbs_handle.order_id + "', '";
		sql += lbs_handle.longitude + "', '";
		sql += lbs_handle.latitude + "', ";
		sql += "null, ";
		sql += "null, '";
		sql += lbs_handle.hbd.temperature + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += lbs_handle.hbd.exposure + "', '";
		sql += lbs_handle.hbd.voltage + "', '";
		sql += lbs_handle.mode + "', '";
		sql += lbs_handle.hbd.rssi + "', '";
		sql += lbs_handle.datetime + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += lbs_handle.hbd.move + "', '";
		sql += lbs_handle.hbd.addr + "')";
	}
	else
	{
		sql = "insert into t_goods_warning ";

		sql += "(FD_ID,";
		sql += "FD_ORDER_ID,";
		sql += "FD_GOODS_GPS_LONG,";
		sql += "FD_GOODS_GPS_LAT,";
		sql += "FD_GOODS_SENSOR_STATE,";
		sql += "FD_GOODS_GPS_MODE,";
		sql += "FD_GOODS_TEMPERATURE,";
		sql += "FD_GOODS_HUMIDITY,";
		sql += "FD_GOODS_ACCELER,";
		sql += "FD_GOODS_ACCELER_X,";
		sql += "FD_GOODS_ACCELER_Y,";
		sql += "FD_GOODS_ACCELER_Z,";
		sql += "FD_GOODS_EXPOSURE,";
		sql += "FD_GOODS_VOLTAGE,";
		sql += "FD_GPS_MODE,";
		sql += "FD_RESERVE,";
		sql += "FD_WARNING_TIME,";
		sql += "FD_GOODS_ANGLE_X,";
		sql += "FD_GOODS_ANGLE_Y,";
		sql += "FD_GOODS_ANGLE_Z,";
		sql += "FD_WARNING_REASON,";
		sql += "FD_WARNING_ADDR,";
		sql += "FD_WARNING_LEVEL,";
		sql += "FD_GOODS_STATE";
		sql += ") values(";

		sql += "GOODS_WARNING_SEQ.nextval, ";
		sql += "'";
		sql += lbs_handle.order_id + "', '";
		sql += lbs_handle.longitude + "', '";
		sql += lbs_handle.latitude + "', ";
		sql += "null, ";
		sql += "null, '";
		sql += lbs_handle.hbd.temperature + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += lbs_handle.hbd.exposure + "', '";
		sql += lbs_handle.hbd.voltage + "', '";
		sql += lbs_handle.mode + "', '";
		sql += lbs_handle.hbd.rssi + "', '";
		sql += lbs_handle.datetime + "', ";
		sql += "null, ";
		sql += "null, ";
		sql += "null, '";
		sql += lbs_handle.hbd.alarm + "', '";
		sql += lbs_handle.hbd.addr + "', '";
		sql += "1', '";
		sql += "-1')";
	}

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	//取分表号
	string sub = common::getMod(lbs_handle.order_id, 10);

	sql = "insert into t_position_info_";
	sql += sub;
	sql += " (FD_ID,";
	sql += "FD_ORDER_ID,";
	sql += "FD_GOODS_GPS_LONG,";
	sql += "FD_GOODS_GPS_LAT,";
	sql += "FD_GOODS_VOLTAGE,";
	sql += "FD_GPS_MODE,";
	sql += "RSSI,";
	sql += "FD_UPDATE_DATE,";
	sql += "FD_INSERT_DATE,";
	sql += "GPS_ADDRESS,";
	sql += "WARNING_TYPE,";
	sql += "GPS_URL,";
	sql += "FD_DEVICE_ID";
	sql += ") values(";

	sql += "position_info_seq.nextval, '";
	sql += lbs_handle.order_id + "', '";
	sql += lbs_handle.longitude + "', '";
	sql += lbs_handle.latitude + "', '";
	sql += lbs_handle.hbd.voltage + "', '";
	sql += lbs_handle.mode + "', '";
	sql += lbs_handle.hbd.rssi + "', '";
	sql += lbs_handle.datetime + "', ";
	sql += "to_char(sysdate, 'YYYYMMDDHH24MISS'), '";
	sql += lbs_handle.hbd.addr + "', ";
	sql += "'00000000', '";
	sql += lbs_handle.url + "', '";
	sql += lbs_handle.dev_id + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


//默认：00000000
//含义：温度1 | 湿度1 | 温度2 | 湿度2 | 光曝 | 震动 | 倾角 | 气压
TLV_HANDLE_RTN DbInterface::warning_type_update(common::DB* db, const string& order_id, const string& ref_date, const int& position)
{
	string sql;

	//计算截取字符串的位置
	stringstream ss;

	ss << position - 1;
	string pos1 = ss.str();
	ss.str("");
	ss.clear();

	ss << position + 1;
	string pos2 = ss.str();
	ss.str("");
	ss.clear();

	//取分表号
	string sub = common::getMod(order_id, 10);

	sql = "update t_position_info_";
	sql += sub;
	sql += " set warning_type =";
	sql += " substr(warning_type, 0, ";
	sql += pos1;
	sql += ")||'1'||substr(warning_type, ";
	sql += pos2;
	sql += ")";
	sql += " where fd_order_id = '";
	sql += order_id + "' ";
	sql += " and fd_update_date = '";
	sql += ref_date + "' ";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


//默认：00000000
//含义：温度1 | 湿度1 | 温度2 | 湿度2 | 光曝 | 震动 | 倾角 | 气压
TLV_HANDLE_RTN DbInterface::warning_type_update_ver12(common::DB* db, const string& order_id, const string& ref_date, const int& position, const string& level)
{
	string sql;

	//计算截取字符串的位置
	stringstream ss;

	ss << position - 1;
	string pos1 = ss.str();
	ss.str("");
	ss.clear();

	ss << position + 1;
	string pos2 = ss.str();
	ss.str("");
	ss.clear();

	//取分表号
	string sub = common::getMod(order_id, 10);

	sql = "update t_position_info_";
	sql += sub;
	sql += " set warning_type =";
	sql += " substr(warning_type, 0, ";
	sql += pos1;
	sql += ")||'";
	sql += level;
	sql += "'||substr(warning_type, ";
	sql += pos2;
	sql += ")";
	sql += " where fd_order_id = '";
	sql += order_id + "' ";
	sql += " and fd_update_date = '";
	sql += ref_date + "' ";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::th_update(common::DB* db, TLV_HANDLE_TH& th_handle)
{
	if(th_handle.overflow == "1")
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", temperature overflow!");
		return THR_TRUE;
	}

	if(common::dateTimeFilter(th_handle.datetime) || common::dateTimeFilter(th_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(th_handle.order_id, 5);

	//更新轨迹
	sql = "insert into da_express_th_";
	sql += sub;
	sql += " (fd_order_id, fd_info_id, fd_flag, fd_update_date, fd_goods_temperature_1, fd_goods_humidity_1) values(";
	sql += "'";
	sql += th_handle.order_id + "', '";
	sql += th_handle.datetime_gps + "', '";
	sql += th_handle.alarm + "', '";
	sql += th_handle.datetime + "', '";
	sql += th_handle.t0 + "', '";
	sql += th_handle.h0 + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(th_handle.alarm.substr(7, 1) == "1")
		if(warning_type_update(db, th_handle.order_id, th_handle.datetime_gps, 1) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(6, 1) == "1")
		if(warning_type_update(db, th_handle.order_id, th_handle.datetime_gps, 2) _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::th_update_ver12(common::DB* db, TLV_HANDLE_TH& th_handle)
{
	if(th_handle.overflow == "1")
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", temperature overflow!");
		return THR_TRUE;
	}

	if(common::dateTimeFilter(th_handle.datetime) || common::dateTimeFilter(th_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(th_handle.order_id, 5);

	//更新轨迹
	sql = "insert into da_express_th_";
	sql += sub;
	sql += " (fd_order_id, fd_info_id, fd_flag, fd_update_date, fd_goods_temperature_1, fd_goods_humidity_1) values(";
	sql += "'";
	sql += th_handle.order_id + "', '";
	sql += th_handle.datetime_gps + "', '";
	sql += th_handle.alarm + "', '";
	sql += th_handle.datetime + "', '";
	sql += th_handle.t0 + "', '";
	sql += th_handle.h0 + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(th_handle.alarm.substr(7, 1) != "0")
		if(warning_type_update_ver12(db, th_handle.order_id, th_handle.datetime_gps, 1, th_handle.alarm.substr(7, 1)) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(6, 1) == "1")
		if(warning_type_update_ver12(db, th_handle.order_id, th_handle.datetime_gps, 2, "1") _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::unband_update(common::DB* db, TLV_HANDLE_UNBAND_RTN& unband_handle)
{
	if(unband_handle.rtn == "1")
	{
		string sql;

		sql = "update T_USER_ORDER ";
		sql += "set fd_order_state = 5, fd_update_date = default ";
		sql += "where fd_device_id = '";
		sql += unband_handle.dev_id;
		sql += "' and fd_order_state != 5";

		LOG_DEBUG("update sql: " << sql);

		if(!db->update(sql))
		{
			return THR_ERROR;
		}

		sql = "update T_USER_GOODS ";
		sql += "set fd_state = 4, fd_reserve = '终端已回复解绑成功', fd_update_date = default, fd_end_time = to_char(sysdate, 'YYYYMMDDHH24MISS') ";
		sql += "where fd_device_id = '";
		sql += unband_handle.dev_id;
		sql += "' and fd_state != 4";

		LOG_DEBUG("update sql: " << sql);

		if(!db->update(sql))
		{
			return THR_ERROR;
		}
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::dev_unband_update(common::DB* db, TLV_HANDLE_DEV_UNBAND_RTN& dev_unband_handle)
{
	string sql;

	sql = "update T_USER_ORDER ";
	sql += "set fd_order_state = 5, fd_update_date = '";
	sql += dev_unband_handle.datetime;
	sql += "' ";
	sql += "where fd_order_id in (select fd_order_id from T_USER_GOODS where fd_device_id = '";
	sql += dev_unband_handle.dev_id;
	sql += "' and fd_state != 4)";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	sql = "update T_USER_GOODS ";
	sql += "set fd_state = 4, fd_reserve = '终端自解绑', fd_update_date = '";
	sql += dev_unband_handle.datetime;
	sql += "', fd_end_time = to_char(sysdate, 'YYYYMMDDHH24MISS') ";
	sql += "where fd_device_id = '";
	sql += dev_unband_handle.dev_id;
	sql += "' and fd_state != 4";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::dev_msg_update(common::DB* db, TLV_HANDLE_DEV_MSG& dev_msg_handle)
{
	string sql;

	sql = "insert into t_device_trace_all ";
	sql += "(fd_id, fd_device_id, fd_goods_sensor_state, fd_reserve, fd_goods_voltage) values(";
	sql += "t_device_trace_autoid.nextval, '";
	sql += dev_msg_handle.dev_id + "', '";
	sql += dev_msg_handle.state + "', '";
	sql += dev_msg_handle.rssi + "', '";
	sql += dev_msg_handle.voltage + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	sql = "update t_device ";
	sql += "set fd_version = '";
	sql += dev_msg_handle.version + "' ";
	sql += "where fd_id = '";
	sql += dev_msg_handle.dev_id + "'";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::dev_type_update(common::DB* db, TLV_HANDLE_DEV_TYPE& dev_type_handle)
{
	string sql;

	sql = "update t_device ";
	sql += "set fd_device_model = '";
	sql += dev_type_handle.type + "', ";
	sql += "fd_update_date = to_char(sysdate, 'YYYYMMDDHH24MISS') ";
	sql += "where fd_id = '";
	sql += dev_type_handle.dev_id + "'";

	LOG_DEBUG("update sql: " << sql);

	if(!db->update(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::parameter_update(common::DB* db, TLV_HANDLE_PARAMETER_RTN& parameter_handle)
{
	if(parameter_handle.rtn == "0")
	{
		string sql;

		sql = "update t_device_config_task_linux ";
		sql += "set fd_flag = '2' ";
		sql += "where fd_device_id = '";
		sql += parameter_handle.dev_id + "'";

		LOG_DEBUG("update sql: " << sql);

		if(!db->update(sql))
		{
			return THR_ERROR;
		}
	}

	if(parameter_handle.rtn == "1")
	{
		string sql;

		sql = "update t_device_config_task_linux ";
		sql += "set fd_flag = '0' ";
		sql += "where fd_device_id = '";
		sql += parameter_handle.dev_id + "'";

		LOG_DEBUG("update sql: " << sql);

		if(!db->update(sql))
		{
			return THR_ERROR;
		}
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::version_update(common::DB* db, TLV_HANDLE_VERSION_RTN& ver_handle)
{
	if(ver_handle.rtn == "0")
	{
		string sql;

		sql = "update T_VERSION_TASK ";
		sql += "set fd_flag = '2' ";
		sql += "where fd_device_id = '";
		sql += ver_handle.dev_id + "'";

		LOG_DEBUG("update sql: " << sql);

		if(!db->update(sql))
		{
			return THR_ERROR;
		}
	}

	if(ver_handle.rtn == "1")
	{
		string sql;

		sql = "update T_VERSION_TASK ";
		sql += "set fd_flag = '0' ";
		sql += "where fd_device_id = '";
		sql += ver_handle.dev_id + "'";

		LOG_DEBUG("update sql: " << sql);

		if(!db->update(sql))
		{
			return THR_ERROR;
		}
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::manage_lbs_update(common::DB* db, TLV_HANDLE_MANAGE_LBS& m_lbs_handle)
{
	string sql;

	sql = "insert into t_device_trace_all ";
	sql += "(fd_id, fd_device_id, fd_goods_sensor_state, fd_reserve, fd_goods_voltage, ";
	sql += "fd_update_date, fd_goods_temperature, fd_goods_gps_long, fd_goods_gps_lat, gps_address) values(";
	sql += "t_device_trace_autoid.nextval, '";
	sql += m_lbs_handle.dev_id + "', '";
	sql += m_lbs_handle.state + "', '";
	sql += m_lbs_handle.rssi + "', '";
	sql += m_lbs_handle.voltage + "', '";
	sql += m_lbs_handle.datetime + "', '";
	sql += m_lbs_handle.temperature + "', '";
	sql += m_lbs_handle.longitude + "', '";
	sql += m_lbs_handle.latitude + "', '";
	sql += m_lbs_handle.addr + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::th2_update(common::DB* db, TLV_HANDLE_TH2& th_handle)
{
	if(th_handle.overflow == "1")
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", temperature overflow!");
		return THR_TRUE;
	}

	if(common::dateTimeFilter(th_handle.datetime) || common::dateTimeFilter(th_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(th_handle.order_id, 5);

	//更新轨迹
	sql = "insert into da_express_th_";
	sql += sub;
	sql += " (fd_order_id, fd_info_id, fd_flag, fd_update_date, fd_goods_temperature_1, fd_goods_humidity_1, fd_goods_temperature_2, fd_goods_humidity_2) values(";
	sql += "'";
	sql += th_handle.order_id + "', '";
	sql += th_handle.datetime_gps + "', '";
	sql += th_handle.alarm + "', '";
	sql += th_handle.datetime + "', '";
	sql += th_handle.t0 + "', '";
	sql += th_handle.h0 + "', ";

	if(th_handle.t1 == "81.0")
	{
		sql += "null, '";
	}
	else
	{
		sql += "'";
		sql += th_handle.t1 + "', '";
	}

	sql += th_handle.h1 + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(th_handle.alarm.substr(7, 1) == "1")
		if(warning_type_update(db, th_handle.order_id, th_handle.datetime_gps, 1) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(6, 1) == "1")
		if(warning_type_update(db, th_handle.order_id, th_handle.datetime_gps, 2) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(5, 1) == "1")
		if(warning_type_update(db, th_handle.order_id, th_handle.datetime_gps, 3) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(4, 1) == "1")
		if(warning_type_update(db, th_handle.order_id, th_handle.datetime_gps, 4) _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::th2_update_ver12(common::DB* db, TLV_HANDLE_TH2& th_handle)
{
	if(th_handle.overflow == "1")
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", temperature overflow!");
		return THR_TRUE;
	}

	if(common::dateTimeFilter(th_handle.datetime) || common::dateTimeFilter(th_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << th_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(th_handle.order_id, 5);

	//更新轨迹
	sql = "insert into da_express_th_";
	sql += sub;
	sql += " (fd_order_id, fd_info_id, fd_flag, fd_update_date, fd_goods_temperature_1, fd_goods_humidity_1, fd_goods_temperature_2, fd_goods_humidity_2) values(";
	sql += "'";
	sql += th_handle.order_id + "', '";
	sql += th_handle.datetime_gps + "', '";
	sql += th_handle.alarm + "', '";
	sql += th_handle.datetime + "', '";
	sql += th_handle.t0 + "', '";
	sql += th_handle.h0 + "', ";

	if(th_handle.t1 == "81.0")
	{
		sql += "null, '";
	}
	else
	{
		sql += "'";
		sql += th_handle.t1 + "', '";
	}

	sql += th_handle.h1 + "')";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(th_handle.alarm.substr(7, 1) != "0")
		if(warning_type_update_ver12(db, th_handle.order_id, th_handle.datetime_gps, 1, th_handle.alarm.substr(7, 1)) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(6, 1) == "1")
		if(warning_type_update_ver12(db, th_handle.order_id, th_handle.datetime_gps, 2, "1") _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(5, 1) != "0")
		if(warning_type_update_ver12(db, th_handle.order_id, th_handle.datetime_gps, 3, th_handle.alarm.substr(5, 1)) _EQ_ THR_ERROR)
			return THR_ERROR;

	if(th_handle.alarm.substr(4, 1) == "1")
		if(warning_type_update_ver12(db, th_handle.order_id, th_handle.datetime_gps, 4, "1") _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::exposure_update(common::DB* db, TLV_HANDLE_EXPOSURE& exposure_handle)
{
	if(common::dateTimeFilter(exposure_handle.datetime) || common::dateTimeFilter(exposure_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << exposure_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(exposure_handle.order_id, 10);

	//更新轨迹
	sql = "insert into t_exposure_info_";
	sql += sub;
	sql += " (fd_id, fd_order_id, fd_info_id, fd_goods_exposure, fd_flag, fd_update_date, fd_insert_date) values(";
	sql += "exposure_info_seq.nextval, '";
	sql += exposure_handle.order_id + "', '";
	sql += exposure_handle.datetime_gps + "', '";
	sql += exposure_handle.exposure + "', '";
	sql += exposure_handle.alarm + "', '";
	sql += exposure_handle.datetime + "', ";
	sql += "to_char(sysdate, 'YYYYMMDDHH24MISS'))";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(exposure_handle.alarm == "1")
		if(warning_type_update(db, exposure_handle.order_id, exposure_handle.datetime_gps, 5) _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::shock_update(common::DB* db, TLV_HANDLE_SHOCK& shock_handle)
{
	if(common::dateTimeFilter(shock_handle.datetime) || common::dateTimeFilter(shock_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << shock_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(shock_handle.order_id, 10);

	//更新轨迹
	sql = "insert into t_acceler_info_";
	sql += sub;
	sql += " (fd_id, fd_order_id, fd_info_id, fd_goods_acceler_x, fd_goods_acceler_y, fd_goods_acceler_z, fd_goods_acceler, fd_flag, fd_update_date, fd_insert_date) values(";
	sql += "acceler_info_seq.nextval, '";
	sql += shock_handle.order_id + "', '";
	sql += shock_handle.datetime_gps + "', '";
	sql += shock_handle.x + "', '";
	sql += shock_handle.y + "', '";
	sql += shock_handle.z + "', '";
	sql += shock_handle.max + "', '";
	sql += shock_handle.alarm + "', '";
	sql += shock_handle.datetime + "', ";
	sql += "to_char(sysdate, 'YYYYMMDDHH24MISS'))";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(shock_handle.alarm == "1")
		if(warning_type_update(db, shock_handle.order_id, shock_handle.datetime_gps, 6) _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::tilt_update(common::DB* db, TLV_HANDLE_TILT& tilt_handle)
{
	if(common::dateTimeFilter(tilt_handle.datetime) || common::dateTimeFilter(tilt_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << tilt_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(tilt_handle.order_id, 10);

	//更新轨迹
	sql = "insert into t_angle_info_";
	sql += sub;
	sql += " (fd_id, fd_order_id, fd_info_id, fd_goods_angle_x, fd_goods_angle_y, fd_goods_angle_z, fd_goods_angle, fd_flag, fd_update_date, fd_insert_date) values(";
	sql += "angle_info_seq.nextval, '";
	sql += tilt_handle.order_id + "', '";
	sql += tilt_handle.datetime_gps + "', '";
	sql += tilt_handle.x + "', '";
	sql += tilt_handle.y + "', '";
	sql += tilt_handle.z + "', '";
	sql += tilt_handle.max + "', '";
	sql += tilt_handle.alarm + "', '";
	sql += tilt_handle.datetime + "', ";
	sql += "to_char(sysdate, 'YYYYMMDDHH24MISS'))";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(tilt_handle.alarm == "1")
		if(warning_type_update(db, tilt_handle.order_id, tilt_handle.datetime_gps, 7) _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}


TLV_HANDLE_RTN DbInterface::baro_update(common::DB* db, TLV_HANDLE_BARO& baro_handle)
{
	if(common::dateTimeFilter(baro_handle.datetime) || common::dateTimeFilter(baro_handle.datetime_gps))
	{
		LOG_DEBUG("dev_id = " << baro_handle.dev_id << ", datetime overflow!");
		return THR_TRUE;
	}

	string sql;

	//取分表号
	string sub = common::getMod(baro_handle.order_id, 10);

	//更新轨迹
	sql = "insert into t_baro_info_";
	sql += sub;
	sql += " (fd_id, fd_order_id, fd_info_id, fd_goods_baro, fd_flag, fd_update_date, fd_insert_date) values(";
	sql += "baro_info_seq.nextval, '";
	sql += baro_handle.order_id + "', '";
	sql += baro_handle.datetime_gps + "', '";
	sql += baro_handle.baro + "', '";
	sql += baro_handle.alarm + "', '";
	sql += baro_handle.datetime + "', ";
	sql += "to_char(sysdate, 'YYYYMMDDHH24MISS'))";

	LOG_DEBUG("insert sql: " << sql);

	if(!db->insert(sql))
	{
		return THR_ERROR;
	}

	if(baro_handle.alarm == "1")
		if(warning_type_update(db, baro_handle.order_id, baro_handle.datetime_gps, 8) _EQ_ THR_ERROR)
			return THR_ERROR;

	return THR_TRUE;
}