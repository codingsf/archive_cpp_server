/*
//
//	Last Modify Date: 2016-10-28
//	Author: zengpw
//	History:
//					2016-09-27 zengpw created
//	File Property: private
//	Future:
//					I. 九州通消息分析和入库
//
//
*/

#include "upload_jzt_handle.h"

session::HANDLE_RTN UploadHandle::jztPush(const char* key, const char* value, const int length)
{
	if(this->rc_jzt->redis_rpush(key, value, length) != 0)
	{
		LOG_DEBUG("redis push occur serious error, please check redis!");
		return session::HANDLE_RTN_STOP;
	}

	return session::HANDLE_RTN_CONTINUE;
}

session::HANDLE_RTN UploadHandle::cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	if(mode == 0)
	{
		stringstream ss;
		TlvHandle th;
		DbInterface di;

		TLVGroupPtr pTlvGrp = in->getTLVGroup();
		LOG_DEBUG("========= cidManageLbs processing begin, pTlvGrp->size() = " << pTlvGrp->size());

		for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
		{
			if(iter->type _EQ_ TID_MANAGE_LBS)
			{
				TLV_MANAGE_LBS m_lbs;
				TLV_HANDLE_MANAGE_LBS m_lbs_handle;

				if(sizeof(m_lbs) != iter->len)
				{
					LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = lbs_manage, version error!");
					continue;
				}

				memset(&m_lbs, 0, sizeof(m_lbs));
				memcpy(&m_lbs, iter->value, iter->len);

				m_lbs_handle.dev_id = in->getDevId();

				int rtn = th.tlv_manage_lbs_handle(m_lbs, m_lbs_handle);

				if(rtn _NEQ_ THR_OK)
				{
					error = true;
					continue;
				}

				rtn = di.manage_lbs_update(db, m_lbs_handle);

				if(rtn _NEQ_ THR_TRUE)
				{
					error = true;
					continue;
				}
			}
		}
	}

	LOG_DEBUG("========= cidManageLbs processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}


session::HANDLE_RTN UploadHandle::cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	if(mode == 0)
	{
		TlvHandle th;
		DbInterface di;

		TLVGroupPtr pTlvGrp = in->getTLVGroup();
		LOG_DEBUG("========= cidParameter processing begin, pTlvGrp->size() = " << pTlvGrp->size());

		for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
		{
			if(iter->type _EQ_ TID_RTN)
			{
				TLV_RTN sending_data;
				TLV_HANDLE_PARAMETER_RTN parameter_handle;

				if(sizeof(sending_data) != iter->len)
				{
					LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = parameter, version error!");
					continue;
				}

				memset(&sending_data, 0, sizeof(sending_data));
				memcpy(&sending_data, iter->value, iter->len);

				parameter_handle.dev_id = in->getDevId();
				int rtn = th.tlv_parameter_handle(sending_data, parameter_handle);

				if(rtn _NEQ_ THR_OK)
				{
					error = true;
					continue;
				}

				rtn = di.parameter_update(db, parameter_handle);

				if(rtn _NEQ_ THR_TRUE)
				{
					error = true;
					continue;
				}
			}
		}
	}

	LOG_DEBUG("========= cidParameter processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}


session::HANDLE_RTN UploadHandle::cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	if(mode == 0)
	{
		TlvHandle th;
		DbInterface di;

		TLVGroupPtr pTlvGrp = in->getTLVGroup();
		LOG_DEBUG("========= cidParameterVersion processing begin, pTlvGrp->size() = " << pTlvGrp->size());

		for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
		{
			if(iter->type _EQ_ TID_RTN)
			{
				TLV_RTN sending_data;
				TLV_HANDLE_VERSION_RTN ver_handle;

				if(sizeof(sending_data) != iter->len)
				{
					LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = parameter_version, version error!");
					continue;
				}

				memset(&sending_data, 0, sizeof(sending_data));
				memcpy(&sending_data, iter->value, iter->len);

				ver_handle.dev_id = in->getDevId();
				int rtn = th.tlv_version_handle(sending_data, ver_handle);

				if(rtn _NEQ_ THR_OK)
				{
					error = true;
					continue;
				}

				rtn = di.version_update(db, ver_handle);

				if(rtn _NEQ_ THR_TRUE)
				{
					error = true;
					continue;
				}
			}
		}
	}

	LOG_DEBUG("========= cidParameterVersion processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}


session::HANDLE_RTN UploadHandle::cidJztDev(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidAutoJztDev processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_JZT_DEV)
		{
			TLV_JZT_DEV jzt_dev;
			TLV_HANDLE_JZT_DEV jzt_dev_handle;

			if(sizeof(jzt_dev) != iter->len)
			{
				LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = jzt_dev, version error!");
				continue;
			}

			memset(&jzt_dev, 0, sizeof(jzt_dev));
			memcpy(&jzt_dev, iter->value, iter->len);

			jzt_dev_handle.dev_id = in->getDevId();

			int rtn = th.tlv_jzt_dev_handle(jzt_dev, jzt_dev_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			// char buf[1024];
			// memset(buf, 0, sizeof(buf));
			// string msg = "{\"fdDeviceId\":\"%s\",\"fdCurrEnergy\":\"%s\",\"fdUpdateDate\":\"%s\"}";
			// sprintf(buf, msg.c_str(), jzt_dev_handle.dev_id.c_str(), jzt_dev_handle.voltage.c_str(), jzt_dev_handle.datetime.c_str());
			// if(this->jztPush("Coglink_DeviceStatus", buf, strlen(buf)) != session::HANDLE_RTN_CONTINUE)
			// {
			// 	error = true;
			// }

			StringBuffer s;
			Writer<StringBuffer> writer(s);

			writer.StartObject();
			writer.Key("fdDeviceId");
			writer.String(jzt_dev_handle.dev_id.c_str());
			writer.Key("fdCurrEnergy");
			writer.String(jzt_dev_handle.voltage.c_str());
			writer.Key("fdUpdateDate");
			writer.String(jzt_dev_handle.datetime.c_str());
			writer.EndObject();

			if(this->jztPush("Coglink_DeviceStatus", s.GetString(), strlen(s.GetString())) != session::HANDLE_RTN_CONTINUE)
			{
				error = true;
			}
		}
	}

	LOG_DEBUG("========= cidAutoJztDev processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}


session::HANDLE_RTN UploadHandle::cidJztLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidJztLbs processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_JZT_LBS)
		{
			TLV_JZT_LBS lbs;
			TLV_HANDLE_JZT_LBS lbs_handle;

			if(sizeof(lbs) != iter->len)
			{
				LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = jzt_lbs, version error!");
				continue;
			}

			memset(&lbs, 0, sizeof(lbs));
			memcpy(&lbs, iter->value, iter->len);

			lbs_handle.dev_id = in->getDevId();

			int rtn = th.tlv_jzt_lbs_handle(lbs, lbs_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			// char buf[1024];
			// memset(buf, 0, sizeof(buf));
			// string msg = "{\"fdDeviceId\":\"%s\",\"fdUpdateDate\":\"%s\",\"fdGoodsGpsLong\":\"%s\",\"fdGoodsGpsLat\":\"%s\"}";
			// sprintf(buf, msg.c_str(), lbs_handle.dev_id.c_str(), lbs_handle.datetime.c_str(), lbs_handle.longitude.c_str(), lbs_handle.latitude.c_str());
			// if(this->jztPush("Coglink_Gps", buf, strlen(buf)) != session::HANDLE_RTN_CONTINUE)
			// {
			// 	error = true;
			// }

			StringBuffer s;
			Writer<StringBuffer> writer(s);

			writer.StartObject();
			writer.Key("fdDeviceId");
			writer.String(lbs_handle.dev_id.c_str());
			writer.Key("fdUpdateDate");
			writer.String(lbs_handle.datetime.c_str());
			writer.Key("fdGoodsGpsLong");
			writer.String(lbs_handle.longitude.c_str());
			writer.Key("fdGoodsGpsLat");
			writer.String(lbs_handle.latitude.c_str());
			writer.EndObject();

			if(this->jztPush("Coglink_Gps", s.GetString(), strlen(s.GetString())) != session::HANDLE_RTN_CONTINUE)
			{
				error = true;
			}
		}
	}

	LOG_DEBUG("========= cidJztLbs processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}


session::HANDLE_RTN UploadHandle::cidJztTh2(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidJztTh2 processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_JZT_TH2)
		{
			TLV_JZT_TH2 th_data;
			TLV_HANDLE_JZT_TH2 th_handle;

			if(sizeof(th_data) != iter->len)
			{
				LOG_DEBUG("dev_id = " << in->getDevId() << ", protocol = jzt_th2, version error!");
				continue;
			}

			memset(&th_data, 0, sizeof(th_data));
			memcpy(&th_data, iter->value, iter->len);

			th_handle.dev_id = in->getDevId();

			int rtn = th.tlv_jzt_th2_handle(th_data, th_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			// char buf[1024];
			// memset(buf, 0, sizeof(buf));
			// string msg = "{\"fdDeviceId\":\"%s\",";
			// msg += "\"fdUpdateDate\":\"%s\",";
			// msg += "\"fdFlag\":\"%s\",";
			// msg += "\"fdGoodsTemperature1\":\"%s\",";
			// msg += "\"fdGoodsTemperature2\":\"%s\",";
			// msg += "\"fdGoodsHumidity1\":\"%s\",";
			// msg += "\"fdGoodsHumidity2\":\"%s\",";
			// msg += "\"fdInfoId\":\"%s\"}";
			// sprintf(buf, msg.c_str(), th_handle.dev_id.c_str(), th_handle.datetime.c_str(), th_handle.alarm.c_str(), 
			// 	th_handle.t0.c_str(), th_handle.t1.c_str(), th_handle.h0.c_str(), th_handle.h1.c_str(), th_handle.datetime_gps.c_str());
			// if(this->jztPush("Coglink_Humiture", buf, strlen(buf)) != session::HANDLE_RTN_CONTINUE)
			// {
			// 	error = true;
			// }

			StringBuffer s;
			Writer<StringBuffer> writer(s);

			writer.StartObject();
			writer.Key("fdDeviceId");
			writer.String(th_handle.dev_id.c_str());
			writer.Key("fdUpdateDate");
			writer.String(th_handle.datetime.c_str());
			writer.Key("fdFlag");
			writer.String(th_handle.alarm.c_str());
			writer.Key("fdGoodsTemperature1");
			writer.String(th_handle.t0.c_str());
			writer.Key("fdGoodsTemperature2");
			writer.String(th_handle.t1.c_str());
			writer.Key("fdGoodsHumidity1");
			writer.String(th_handle.h0.c_str());
			writer.Key("fdGoodsHumidity2");
			writer.String(th_handle.h1.c_str());
			writer.Key("fdInfoId");
			writer.String(th_handle.datetime_gps.c_str());
			writer.EndObject();

			if(this->jztPush("Coglink_Humiture", s.GetString(), strlen(s.GetString())) != session::HANDLE_RTN_CONTINUE)
			{
				error = true;
			}
		}
	}

	LOG_DEBUG("========= cidJztTh2 processing end.");

	if(error)
		return session::HANDLE_RTN_CONTINUE;
	else
		return session::HANDLE_RTN_REMOVE;
}



