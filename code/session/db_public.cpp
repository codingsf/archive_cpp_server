/*
//
//	Last Modify Date: 2016-08-26
//	Author: zengpw
//	History:
//					2015-11-04 zengpw created
//	File Property: private
//	Future:
//					I. 数据库操作
//
//
*/

#include "db_public.h"

namespace session
{
	bool DbPublic::public_get_order_id(common::DB* db, const string& dev_id, string& order_id)
	{
		string sql;

		sql = "select nvl(fd_order_id, null) from t_user_goods ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "' and fd_state !='4'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		char buf[64];
		memset(buf, 0, sizeof(buf));

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				while(!os.eof())
				{
					os >> buf;
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		if(buf[0] == '\0')
		{
			LOG_DEBUG("message: dev_id " << dev_id << " can't find order id!");
			return false;
		}

		stringstream ss;

		ss << buf;
		order_id = ss.str();
		ss.str("");
		ss.clear();

		LOG_DEBUG("order_id : " << order_id);

		return true;
	}


	bool DbPublic::public_get_protocol_ver(common::DB* db, const string& dev_id, string& protocol_ver)
	{
		string sql;

		sql = "select nvl(fd_version, null) from t_device ";
		sql += "where fd_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		char buf[64];
		memset(buf, 0, sizeof(buf));

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				while(!os.eof())
				{
					os >> buf;
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		if(buf[0] == '\0')
		{
			LOG_DEBUG("message: dev_id " << dev_id << " can't find fd_version!");
			return false;
		}

		stringstream ss;

		ss << buf;
		protocol_ver = ss.str();
		ss.str("");
		ss.clear();

		LOG_DEBUG("protocol_ver : " << protocol_ver);

		return true;
	}


	bool DbPublic::public_get_parameter_request(common::DB* db, const string& dev_id, TLV_PARAMETER& para)
	{
		string sql;

		sql = "select fd_apn, fd_sys_net, fd_ter_exit, fd_ter_show, fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, fd_gprs_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config ";
		sql += "where fd_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle << \
				", lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_request_ver11(common::DB* db, const string& dev_id, TLV_PARAMETER_V11& para)
	{
		string sql;

		sql = "select fd_apn, fd_sys_net, fd_ter_exit, fd_ter_show, fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_vibrate_collect_enable, fd_vibrate_enable, fd_vibrate_sound_enable, ";
		sql += "fd_angle_collect_enable, fd_angle_enable, fd_angle_sound_enable, fd_angle_start, fd_angle_x, fd_angle_y, fd_angle_z, ";
		sql += "fd_air_collect_enable, fd_air_enable, fd_air_sound_enable, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_angle_storage_rate, fd_angle_value, fd_vibrate_storage_rate, fd_vibrate_value, fd_air_storage_rate, fd_air_value, fd_air_minvalue, ";
		sql += "fd_gprs_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config ";
		sql += "where fd_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.1开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_fix_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_x = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_y = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_z = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					//------------ v1.1门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_min;
					ss.str("");
					ss.clear();

					//------------ 额外数据
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("shock_switch = " << (int)para.shock_switch << \
				", shock_alarm_switch = " << (int)para.shock_alarm_switch << \
				", shock_voice_switch = " << (int)para.shock_voice_switch << \
				", tilt_switch = " << (int)para.tilt_switch << \
				", tilt_alarm_switch = " << (int)para.tilt_alarm_switch << \
				", tilt_voice_switch = " << (int)para.tilt_voice_switch << \
				", tilt_fix_flag = " << (int)para.tilt_fix_flag << \
				", tilt_x = " << (int)para.tilt_x << \
				", tilt_y = " << (int)para.tilt_y << \
				", tilt_z = " << (int)para.tilt_z << \
				", baro_switch = " << (int)para.baro_switch << \
				", baro_alarm_switch = " << (int)para.baro_alarm_switch << \
				", baro_voice_switch = " << (int)para.baro_voice_switch);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle);
		LOG_DEBUG("tilt_store_cycle = " << para.tilt_store_cycle << \
				", tilt_limit = " << para.tilt_limit << \
				", shock_cycle = " << para.shock_cycle << \
				", shock_limit = " << para.shock_limit << \
				", baro_store_cycle = " << para.baro_store_cycle << \
				", baro_max = " << para.baro_max << \
				", baro_min = " << para.baro_min);
		LOG_DEBUG("lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_request_ver13(common::DB* db, const string& dev_id, TLV_PARAMETER_V13& para)
	{
		string sql;

		sql = "select fd_apn, fd_sys_net, fd_ter_exit, fd_ter_show, fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_vibrate_collect_enable, fd_vibrate_enable, fd_vibrate_sound_enable, ";
		sql += "fd_angle_collect_enable, fd_angle_enable, fd_angle_sound_enable, fd_angle_start, fd_angle_x, fd_angle_y, fd_angle_z, ";
		sql += "fd_air_collect_enable, fd_air_enable, fd_air_sound_enable, ";
		sql += "fd_pdf_enable, fd_pdf_choice, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_angle_storage_rate, fd_angle_value, fd_vibrate_storage_rate, fd_vibrate_value, fd_air_storage_rate, fd_air_value, fd_air_minvalue, ";
		sql += "fd_gprs_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config ";
		sql += "where fd_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.1开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_fix_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_x = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_y = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_z = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.3开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					//------------ v1.1门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_min;
					ss.str("");
					ss.clear();

					//------------ 额外数据
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("shock_switch = " << (int)para.shock_switch << \
				", shock_alarm_switch = " << (int)para.shock_alarm_switch << \
				", shock_voice_switch = " << (int)para.shock_voice_switch << \
				", tilt_switch = " << (int)para.tilt_switch << \
				", tilt_alarm_switch = " << (int)para.tilt_alarm_switch << \
				", tilt_voice_switch = " << (int)para.tilt_voice_switch << \
				", tilt_fix_flag = " << (int)para.tilt_fix_flag << \
				", tilt_x = " << (int)para.tilt_x << \
				", tilt_y = " << (int)para.tilt_y << \
				", tilt_z = " << (int)para.tilt_z << \
				", baro_switch = " << (int)para.baro_switch << \
				", baro_alarm_switch = " << (int)para.baro_alarm_switch << \
				", baro_voice_switch = " << (int)para.baro_voice_switch << \
				", pdf_switch = " << (int)para.pdf_switch << \
				", pdf = " << (int)para.pdf);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle);
		LOG_DEBUG("tilt_store_cycle = " << para.tilt_store_cycle << \
				", tilt_limit = " << para.tilt_limit << \
				", shock_cycle = " << para.shock_cycle << \
				", shock_limit = " << para.shock_limit << \
				", baro_store_cycle = " << para.baro_store_cycle << \
				", baro_max = " << para.baro_max << \
				", baro_min = " << para.baro_min);
		LOG_DEBUG("lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_request_ver14(common::DB* db, const string& dev_id, TLV_PARAMETER_V14& para)
	{
		string sql;

		sql = "select fd_apn, fd_sys_net, fd_ter_exit, fd_ter_show, fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_vibrate_collect_enable, fd_vibrate_enable, fd_vibrate_sound_enable, ";
		sql += "fd_angle_collect_enable, fd_angle_enable, fd_angle_sound_enable, fd_angle_start, fd_angle_x, fd_angle_y, fd_angle_z, ";
		sql += "fd_air_collect_enable, fd_air_enable, fd_air_sound_enable, ";
		sql += "fd_pdf_enable, fd_pdf_choice, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_pre_temperature_value, fd_pre_temperature_minvalue, fd_pre_temperature_value2, fd_pre_temperature_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_angle_storage_rate, fd_angle_value, fd_vibrate_storage_rate, fd_vibrate_value, fd_air_storage_rate, fd_air_value, fd_air_minvalue, ";
		sql += "fd_gprs_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config ";
		sql += "where fd_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.1开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_fix_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_x = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_y = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_z = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.3开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					//------------ v1.4门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_pMax;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_pMin;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_pMax;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_pMin;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					//------------ v1.1门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_min;
					ss.str("");
					ss.clear();

					//------------ 额外数据
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("shock_switch = " << (int)para.shock_switch << \
				", shock_alarm_switch = " << (int)para.shock_alarm_switch << \
				", shock_voice_switch = " << (int)para.shock_voice_switch << \
				", tilt_switch = " << (int)para.tilt_switch << \
				", tilt_alarm_switch = " << (int)para.tilt_alarm_switch << \
				", tilt_voice_switch = " << (int)para.tilt_voice_switch << \
				", tilt_fix_flag = " << (int)para.tilt_fix_flag << \
				", tilt_x = " << (int)para.tilt_x << \
				", tilt_y = " << (int)para.tilt_y << \
				", tilt_z = " << (int)para.tilt_z << \
				", baro_switch = " << (int)para.baro_switch << \
				", baro_alarm_switch = " << (int)para.baro_alarm_switch << \
				", baro_voice_switch = " << (int)para.baro_voice_switch << \
				", pdf_switch = " << (int)para.pdf_switch << \
				", pdf = " << (int)para.pdf);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", t0_pMax = " << para.t0_pMax << \
				", t0_pMin = " << para.t0_pMin << \
				", t1_pMax = " << para.t1_pMax << \
				", t1_pMin = " << para.t1_pMin << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle);
		LOG_DEBUG("tilt_store_cycle = " << para.tilt_store_cycle << \
				", tilt_limit = " << para.tilt_limit << \
				", shock_cycle = " << para.shock_cycle << \
				", shock_limit = " << para.shock_limit << \
				", baro_store_cycle = " << para.baro_store_cycle << \
				", baro_max = " << para.baro_max << \
				", baro_min = " << para.baro_min);
		LOG_DEBUG("lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_sending(common::DB* db, const string& dev_id, TLV_PARAMETER& para)
	{
		string sql;

		sql = "select fd_data_ip, fd_data_port, fd_apn, fd_phone_username, fd_phone_password, fd_sys_net, fd_ter_exit, fd_ter_show, ";
		sql += "fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_lbs_send_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config_task_linux ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.addr;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.port;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.user;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.pass;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle << \
				", lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_sending_ver11(common::DB* db, const string& dev_id, TLV_PARAMETER_V11& para)
	{
		string sql;

		sql = "select fd_data_ip, fd_data_port, fd_apn, fd_phone_username, fd_phone_password, fd_sys_net, fd_ter_exit, fd_ter_show, ";
		sql += "fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_vibrate_collect_enable, fd_vibrate_enable, fd_vibrate_sound_enable, ";
		sql += "fd_angle_collect_enable, fd_angle_enable, fd_angle_sound_enable, fd_angle_start, fd_angle_x, fd_angle_y, fd_angle_z, ";
		sql += "fd_air_collect_enable, fd_air_enable, fd_air_sound_enable, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_angle_storage_rate, fd_angle_value, fd_vibrate_storage_rate, fd_vibrate_value, ";
		sql += "fd_air_storage_rate, fd_air_value, fd_air_minvalue, ";
		sql += "fd_lbs_send_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config_task_linux ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.addr;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.port;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.user;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.pass;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.1开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_fix_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_x = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_y = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_z = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					//------------ v1.1门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_min;
					ss.str("");
					ss.clear();

					//------------ 额外数据
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("shock_switch = " << (int)para.shock_switch << \
				", shock_alarm_switch = " << (int)para.shock_alarm_switch << \
				", shock_voice_switch = " << (int)para.shock_voice_switch << \
				", tilt_switch = " << (int)para.tilt_switch << \
				", tilt_alarm_switch = " << (int)para.tilt_alarm_switch << \
				", tilt_voice_switch = " << (int)para.tilt_voice_switch << \
				", tilt_fix_flag = " << (int)para.tilt_fix_flag << \
				", tilt_x = " << (int)para.tilt_x << \
				", tilt_y = " << (int)para.tilt_y << \
				", tilt_z = " << (int)para.tilt_z << \
				", baro_switch = " << (int)para.baro_switch << \
				", baro_alarm_switch = " << (int)para.baro_alarm_switch << \
				", baro_voice_switch = " << (int)para.baro_voice_switch);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle);
		LOG_DEBUG("tilt_store_cycle = " << para.tilt_store_cycle << \
				", tilt_limit = " << para.tilt_limit << \
				", shock_cycle = " << para.shock_cycle << \
				", shock_limit = " << para.shock_limit << \
				", baro_store_cycle = " << para.baro_store_cycle << \
				", baro_max = " << para.baro_max << \
				", baro_min = " << para.baro_min);
		LOG_DEBUG("lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_sending_ver13(common::DB* db, const string& dev_id, TLV_PARAMETER_V13& para)
	{
		string sql;

		sql = "select fd_data_ip, fd_data_port, fd_apn, fd_phone_username, fd_phone_password, fd_sys_net, fd_ter_exit, fd_ter_show, ";
		sql += "fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_vibrate_collect_enable, fd_vibrate_enable, fd_vibrate_sound_enable, ";
		sql += "fd_angle_collect_enable, fd_angle_enable, fd_angle_sound_enable, fd_angle_start, fd_angle_x, fd_angle_y, fd_angle_z, ";
		sql += "fd_air_collect_enable, fd_air_enable, fd_air_sound_enable, ";
		sql += "fd_pdf_enable, fd_pdf_choice, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_angle_storage_rate, fd_angle_value, fd_vibrate_storage_rate, fd_vibrate_value, ";
		sql += "fd_air_storage_rate, fd_air_value, fd_air_minvalue, ";
		sql += "fd_lbs_send_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config_task_linux ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.addr;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.port;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.user;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.pass;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.1开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_fix_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_x = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_y = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_z = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.3开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					//------------ v1.1门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_min;
					ss.str("");
					ss.clear();

					//------------ 额外数据
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("shock_switch = " << (int)para.shock_switch << \
				", shock_alarm_switch = " << (int)para.shock_alarm_switch << \
				", shock_voice_switch = " << (int)para.shock_voice_switch << \
				", tilt_switch = " << (int)para.tilt_switch << \
				", tilt_alarm_switch = " << (int)para.tilt_alarm_switch << \
				", tilt_voice_switch = " << (int)para.tilt_voice_switch << \
				", tilt_fix_flag = " << (int)para.tilt_fix_flag << \
				", tilt_x = " << (int)para.tilt_x << \
				", tilt_y = " << (int)para.tilt_y << \
				", tilt_z = " << (int)para.tilt_z << \
				", baro_switch = " << (int)para.baro_switch << \
				", baro_alarm_switch = " << (int)para.baro_alarm_switch << \
				", baro_voice_switch = " << (int)para.baro_voice_switch << \
				", pdf_switch = " << (int)para.pdf_switch << \
				", pdf = " << (int)para.pdf);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle);
		LOG_DEBUG("tilt_store_cycle = " << para.tilt_store_cycle << \
				", tilt_limit = " << para.tilt_limit << \
				", shock_cycle = " << para.shock_cycle << \
				", shock_limit = " << para.shock_limit << \
				", baro_store_cycle = " << para.baro_store_cycle << \
				", baro_max = " << para.baro_max << \
				", baro_min = " << para.baro_min);
		LOG_DEBUG("lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_parameter_sending_ver14(common::DB* db, const string& dev_id, TLV_PARAMETER_V14& para)
	{
		string sql;

		sql = "select fd_data_ip, fd_data_port, fd_apn, fd_phone_username, fd_phone_password, fd_sys_net, fd_ter_exit, fd_ter_show, ";
		sql += "fd_light_collect_enable, fd_light_enable, fd_light_sound_enable, fd_th_collect_enable, ";
		sql += "fd_temperature_enable, fd_temperature_sound_enable, fd_humidity_enable, fd_humidity_sound_enable, ";
		sql += "fd_temperature_enable2, fd_temperature_sound_enable2, fd_humidity_enable2, fd_humidity_sound_enable2, ";
		sql += "fd_vibrate_collect_enable, fd_vibrate_enable, fd_vibrate_sound_enable, ";
		sql += "fd_angle_collect_enable, fd_angle_enable, fd_angle_sound_enable, fd_angle_start, fd_angle_x, fd_angle_y, fd_angle_z, ";
		sql += "fd_air_collect_enable, fd_air_enable, fd_air_sound_enable, ";
		sql += "fd_pdf_enable, fd_pdf_choice, ";
		sql += "fd_light_value, ";
		sql += "fd_temperature_value, fd_temperature_minvalue, fd_humidity_value, fd_humidity_minvalue, ";
		sql += "fd_temperature_value2, fd_temperature_minvalue2, fd_humidity_value2, fd_humidity_minvalue2, ";
		sql += "fd_pre_temperature_value, fd_pre_temperature_minvalue, fd_pre_temperature_value2, fd_pre_temperature_minvalue2, ";
		sql += "fd_temperature_collect_rate, fd_temperature_storage_rate, fd_temperature_storage_rate2, fd_light_collect_rate, fd_light_storage_rate, ";
		sql += "fd_angle_storage_rate, fd_angle_value, fd_vibrate_storage_rate, fd_vibrate_value, ";
		sql += "fd_air_storage_rate, fd_air_value, fd_air_minvalue, ";
		sql += "fd_lbs_send_rate, fd_lbs_send_rate2 ";
		sql += "from t_device_config_task_linux ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&para, 0, sizeof(para));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.addr;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.port;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.user;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.pass;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.net = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.band = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.led = i;
					ss.str("");
					ss.clear();

					//------------ 开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h0_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.t1_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.h1_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.1开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.shock_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_voice_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_fix_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_x = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_y = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.tilt_z = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_alarm_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.baro_voice_switch = i;
					ss.str("");
					ss.clear();

					//------------ v1.3开关
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf_switch = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.pdf = i;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					para.exposure_max = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h0_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_min;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.h1_min;
					ss.str("");
					ss.clear();

					//------------ v1.4门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_pMax;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t0_pMin;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_pMax;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.t1_pMin;
					ss.str("");
					ss.clear();

					//------------ 门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.temperature_store_cycle_alarm;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.exposure_store_cycle;
					ss.str("");
					ss.clear();

					//------------ v1.1门限
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.tilt_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.shock_limit;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_store_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_max;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.baro_min;
					ss.str("");
					ss.clear();

					//------------ 额外数据
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> para.lbs_send_cycle_alarm;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER content : ");
		LOG_DEBUG("addr = " << para.addr << \
				", port = " << para.port << \
				", apn = " << para.apn << \
				", user = " << para.user << \
				", pass = " << para.pass << \
				", net = " << (int)para.net << \
				", band = " << (int)para.band << \
				", led = " << (int)para.led);
		LOG_DEBUG("exposure_switch = " << (int)para.exposure_switch << \
				", exposure_alarm_switch = " << (int)para.exposure_alarm_switch << \
				", exposure_voice_switch = " << (int)para.exposure_voice_switch << \
				", t_switch = " << (int)para.t_switch << \
				", t0_alarm_switch = " << (int)para.t0_alarm_switch << \
				", t0_voice_switch = " << (int)para.t0_voice_switch << \
				", h0_alarm_switch = " << (int)para.h0_alarm_switch << \
				", h0_voice_switch = " << (int)para.h0_voice_switch << \
				", t1_alarm_switch = " << (int)para.t1_alarm_switch << \
				", t1_voice_switch = " << (int)para.t1_voice_switch << \
				", h1_alarm_switch = " << (int)para.h1_alarm_switch << \
				", h1_voice_switch = " << (int)para.h1_voice_switch);
		LOG_DEBUG("shock_switch = " << (int)para.shock_switch << \
				", shock_alarm_switch = " << (int)para.shock_alarm_switch << \
				", shock_voice_switch = " << (int)para.shock_voice_switch << \
				", tilt_switch = " << (int)para.tilt_switch << \
				", tilt_alarm_switch = " << (int)para.tilt_alarm_switch << \
				", tilt_voice_switch = " << (int)para.tilt_voice_switch << \
				", tilt_fix_flag = " << (int)para.tilt_fix_flag << \
				", tilt_x = " << (int)para.tilt_x << \
				", tilt_y = " << (int)para.tilt_y << \
				", tilt_z = " << (int)para.tilt_z << \
				", baro_switch = " << (int)para.baro_switch << \
				", baro_alarm_switch = " << (int)para.baro_alarm_switch << \
				", baro_voice_switch = " << (int)para.baro_voice_switch << \
				", pdf_switch = " << (int)para.pdf_switch << \
				", pdf = " << (int)para.pdf);
		LOG_DEBUG("exposure_max = " << para.exposure_max << \
				", t0_max = " << para.t0_max << \
				", t0_min = " << para.t0_min << \
				", h0_max = " << para.h0_max << \
				", h0_min = " << para.h0_min << \
				", t1_max = " << para.t1_max << \
				", t1_min = " << para.t1_min << \
				", h1_max = " << para.h1_max << \
				", h1_min = " << para.h1_min << \
				", t0_pMax = " << para.t0_pMax << \
				", t0_pMin = " << para.t0_pMin << \
				", t1_pMax = " << para.t1_pMax << \
				", t1_pMin = " << para.t1_pMin << \
				", temperature_cycle = " << para.temperature_cycle << \
				", temperature_store_cycle = " << para.temperature_store_cycle << \
				", temperature_store_cycle_alarm = " << para.temperature_store_cycle_alarm << \
				", exposure_cycle = " << para.exposure_cycle << \
				", exposure_store_cycle = " << para.exposure_store_cycle);
		LOG_DEBUG("tilt_store_cycle = " << para.tilt_store_cycle << \
				", tilt_limit = " << para.tilt_limit << \
				", shock_cycle = " << para.shock_cycle << \
				", shock_limit = " << para.shock_limit << \
				", baro_store_cycle = " << para.baro_store_cycle << \
				", baro_max = " << para.baro_max << \
				", baro_min = " << para.baro_min);
		LOG_DEBUG("lbs_send_cycle = " << para.lbs_send_cycle << \
				", lbs_send_cycle_alarm = " << para.lbs_send_cycle_alarm);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_version(common::DB* db, const string& dev_id, TLV_PARAMETER_VERSION& ver)
	{
		string sql;

		sql = "select fd_update_flag, fd_version_code, fd_server_ip, fd_server_port, ";
		sql += "fd_apn, fd_server_account, fd_server_pwd, fd_ftp_filename, fd_ftp_path ";
		sql += "from t_version_task ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				char buf[64];
				stringstream ss;
				int i;

				memset(&ver, 0, sizeof(ver));

				while(!os.eof())
				{
					//------------ 服务器
					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					ver.update_flag = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> i;
					ver.version = i;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.addr;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.port;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.apn;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.user;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.pass;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.file;
					ss.str("");
					ss.clear();

					memset(buf, 0, sizeof(buf));
					os >> buf;
					ss << buf;
					ss >> ver.path;
					ss.str("");
					ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		LOG_DEBUG("---------- TLV_PARAMETER_VERSION content : ");
		LOG_DEBUG("update_flag = " << (int)ver.update_flag << \
				", version = " << (int)ver.version << \
				", addr = " << ver.addr << \
				", port = " << ver.port << \
				", apn = " << ver.apn << \
				", user = " << ver.user << \
				", pass = " << ver.pass << \
				", file = " << ver.file << \
				", path = " << ver.path);
		LOG_DEBUG("---------- end.");

		return true;
	}


	bool DbPublic::public_get_order_type(common::DB* db, const string& dev_id, int& order_type)
	{
		string sql;

		sql = "select count(1) from t_device ";
		sql += "where fd_id = '";
		sql += dev_id;
		sql += "' and order_type = '1'";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		int i = 0;
		order_type = 0;

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				while(!os.eof())
				{
					os >> i;
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		order_type = i;

		return true;
	}


	bool DbPublic::timing_get_dev_id(common::DB* db, vector<TLV_HANDLE_FENCE> &vecThf)
	{
		string sql;
		sql = "select nvl(fd_order_id, null), nvl(fd_device_id, null) from t_user_goods where fd_state != '4' ";
		sql += "and fd_device_id in(select nvl(fd_id, null) from t_device where version_type = '1')";

		LOG_DEBUG("select sql: " << sql);

		vecThf.clear();

		if(!db->check())
		{
			return false;
		}

		char buf[64];
		stringstream ss;

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				while(!os.eof())
				{
					TLV_HANDLE_FENCE thf;

					memset(buf, 0, sizeof(buf));
					os >> buf;

					if(buf[0] != '\0')
					{
						ss << buf;
						thf.order_id = ss.str();
						ss.str("");
						ss.clear();
					}

					memset(buf, 0, sizeof(buf));
					os >> buf;

					if(buf[0] != '\0')
					{
						ss << buf;
						thf.dev_id = ss.str();
						ss.str("");
						ss.clear();

						vecThf.push_back(thf);
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		return true;
	}


	bool DbPublic::timing_get_location(common::DB* db, TLV_HANDLE_FENCE& thf)
	{
		stringstream ss;

		//取分表号
		int i = 0;
		string sub = thf.order_id.substr(thf.order_id.length() - 1);
		ss << sub;
		ss >> i;
		ss.str("");
		ss.clear();

		i = i%10;
		ss << i;
		sub = ss.str();
		ss.str("");
		ss.clear();

		string sql;

		sql = "select * from (";
		sql += "select fd_goods_gps_long, fd_goods_gps_lat, fd_update_date from t_transway_all_msg_";
		sql += sub;
		sql += " where fd_order_id = '";
		sql += thf.order_id;
		sql += "' order by fd_update_date desc";
		sql += ") where rownum = 1";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		char buf[64];

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				while(!os.eof())
				{
					memset(buf, 0, sizeof(buf));
					os >> buf;

					if(buf[0] != '\0')
					{
						ss << buf;
						thf.longitude = ss.str();
						ss.str("");
						ss.clear();
					}

					memset(buf, 0, sizeof(buf));
					os >> buf;

					if(buf[0] != '\0')
					{
						ss << buf;
						thf.latitude = ss.str();
						ss.str("");
						ss.clear();
					}

					memset(buf, 0, sizeof(buf));
					os >> buf;

					if(buf[0] != '\0')
					{
						ss << buf;
						thf.datetime = ss.str();
						ss.str("");
						ss.clear();
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		return true;
	}

	bool DbPublic::public_get_start(common::DB* db, const string& dev_id, TLV_TIME_6& time_6, TLV_EXPRESS_ID& express_id)
	{
		string sql;

		sql = "select nvl(fd_start_time, null), nvl(fd_express_id, null) from t_user_goods ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "' and fd_state !='4' order by fd_update_date desc";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		char buf6[64];
		memset(buf6, 0, sizeof(buf6));

		char buf31[31];
		memset(buf31, 0, sizeof(buf31));

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				if(!os.eof())
				{
					os >> buf6;
					os >> buf31;
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		if(buf6[0] == '\0')
		{
			LOG_DEBUG("message: dev_id " << dev_id << " can't find start time!");
			return false;
		}

		if(buf31[0] == '\0')
		{
			LOG_DEBUG("message: dev_id " << dev_id << " can't find express_id!");
			return false;
		}

		char buf14[14];// YYYYmmDD HHMMSS
		memset(buf14, 0, sizeof(buf14));

		stringstream ss;

		ss << buf6;
		ss >> buf14;
		ss.str("");
		ss.clear();

		memset(&time_6, 0, sizeof(time_6));
		time_6.dt.year = (buf14[2] - 48)*10 + (buf14[3] - 48);
		time_6.dt.mon = (buf14[4] - 48)*10 + (buf14[5] - 48);
		time_6.dt.day = (buf14[6] - 48)*10 + (buf14[7] - 48);
		time_6.dt.hour = (buf14[8] - 48)*10 + (buf14[9] - 48);
		time_6.dt.min = (buf14[10] - 48)*10 + (buf14[11] - 48);
		time_6.dt.sec = (buf14[12] - 48)*10 + (buf14[13] - 48);

		printDateTime2k(time_6.dt);

		memset(&express_id, 0, sizeof(express_id));
		memcpy(&express_id, buf31, sizeof(buf31));

		return true;
	}

	bool DbPublic::public_get_end(common::DB* db, const string& dev_id, TLV_TIME_6& para)
	{
		string sql;

		sql = "select nvl(fd_end_time, null) from t_user_goods ";
		sql += "where fd_device_id = '";
		sql += dev_id;
		sql += "' and fd_state = '4' order by fd_update_date desc";

		LOG_DEBUG("select sql: " << sql);

		if(!db->check())
		{
			return false;
		}

		char buf[64];
		memset(buf, 0, sizeof(buf));

		try
		{
			otl_nocommit_stream os(1, sql.c_str(), *db->conn);

			try
			{
				db->conn->commit();

				if(!os.eof())
				{
					os >> buf;
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
				return false;

			db->conn->rollback();
			return false;
		}
		catch(...)
		{
			LOG_DEBUG("DB select occur unknown exception!");
			db->status = false;
			db->conn->rollback();
			return false;
		}

		if(buf[0] == '\0')
		{
			LOG_DEBUG("message: dev_id " << dev_id << " can't find end time!");
			return false;
		}

		char buf14[14];// YYYYmmDD HHMMSS
		memset(buf14, 0, sizeof(buf14));

		stringstream ss;

		ss << buf;
		ss >> buf14;
		ss.str("");
		ss.clear();

		memset(&para, 0, sizeof(para));
		para.dt.year = (buf14[2] - 48)*10 + (buf14[3] - 48);
		para.dt.mon = (buf14[4] - 48)*10 + (buf14[5] - 48);
		para.dt.day = (buf14[6] - 48)*10 + (buf14[7] - 48);
		para.dt.hour = (buf14[8] - 48)*10 + (buf14[9] - 48);
		para.dt.min = (buf14[10] - 48)*10 + (buf14[11] - 48);
		para.dt.sec = (buf14[12] - 48)*10 + (buf14[13] - 48);

		printDateTime2k(para.dt);

		return true;
	}
};// namespace session