/*
//
//	Last Modify Date: 2016-03-17
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
};// namespace session