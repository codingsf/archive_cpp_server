/*
//
//	Last Modify Date: 2016-01-28
//	Author: zengpw
//	History:
//					2015-10-09 zengpw created
//	File Property: private
//	Future:
//					I. 消息分析和入库
//
//
*/

#include "upload_handle.h"

void UploadHandle::setJavaAddr(string java_addr)
{
	this->java_addr = java_addr;
}


session::HANDLE_RTN UploadHandle::cidLogin(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidLogin processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_DEV_FIRM)
		{
			TLV_DEV_FIRM firm;
			TLV_HANDLE_DEV_FIRM firm_handle;

			memset(&firm, 0, sizeof(firm));
			memcpy(&firm, iter->value, iter->len);

			firm_handle.dev_id = in->getDevId();
			int rtn = th.tlv_firm_handle(firm, firm_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.firm_update(db, firm_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidLogin processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataGps(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataGps processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_GPS)
		{
			TLV_GPS gps;
			TLV_HANDLE_GPS gps_handle;

			memset(&gps, 0, sizeof(gps));
			memcpy(&gps, iter->value, iter->len);

			gps_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, gps_handle.dev_id, gps_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_gps_handle(gps, gps_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.gps_update(db, gps_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDataGps processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataLbs processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_LBS)
		{
			TLV_LBS lbs;
			TLV_HANDLE_LBS lbs_handle;

			memset(&lbs, 0, sizeof(lbs));
			memcpy(&lbs, iter->value, iter->len);

			lbs_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, lbs_handle.dev_id, lbs_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_lbs_handle(lbs, lbs_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.lbs_update(db, lbs_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDataLbs processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataTh(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataTh processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_TH)
		{
			TLV_TH th_data;
			TLV_HANDLE_TH th_handle;

			memset(&th_data, 0, sizeof(th_data));
			memcpy(&th_data, iter->value, iter->len);

			th_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, th_handle.dev_id, th_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_th_handle(th_data, th_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.th_update(db, th_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}

			//通知 java
			session::HttpInterface hi;

			if(th_handle.alarm.substr(th_handle.alarm.length() - 1, 1) == "1" && th_handle.overflow == "0")
				hi.notifyJava(this->java_addr, th_handle.dev_id, "1", th_handle.t0, "1");

			if(th_handle.alarm.substr(th_handle.alarm.length() - 2, 1) == "1")
				hi.notifyJava(this->java_addr, th_handle.dev_id, "2", th_handle.h0, "1");
		}
	}

	LOG_DEBUG("========= cidDataTh processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidUnband processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_RTN)
		{
			TLV_RTN unband_data;
			TLV_HANDLE_UNBAND_RTN unband_handle;

			memset(&unband_data, 0, sizeof(unband_data));
			memcpy(&unband_data, iter->value, iter->len);

			unband_handle.dev_id = in->getDevId();
			int rtn = th.tlv_unband_handle(unband_data, unband_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.unband_update(db, unband_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidUnband processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDevUnband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDevUnband processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_TIME)
		{
			TLV_TIME tm;
			TLV_HANDLE_DEV_UNBAND_RTN dev_unband_handle;

			memset(&tm, 0, sizeof(tm));
			memcpy(&tm, iter->value, iter->len);

			dev_unband_handle.dev_id = in->getDevId();
			int rtn = th.tlv_dev_unband_handle(tm, dev_unband_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.dev_unband_update(db, dev_unband_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDevUnband processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDevband(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	LOG_DEBUG("========= cidDevband processing begin, pTlvGrp->size() = 0");

	session::DbPublic dp;
	int order_type = 0;

	if(!dp.public_get_order_type(db, in->getDevId(), order_type))
	{
		error = true;
	}

	if(order_type > 0)
	{
		//通知 java 生成订单
		session::HttpInterface hi;
		hi.notifyJavaCreateOrder(this->java_addr, in->getDevId());
	}

	LOG_DEBUG("========= cidDevband processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDevMsg(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDevMsg processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_DEV_MSG)
		{
			TLV_DEV_MSG tdm;
			TLV_HANDLE_DEV_MSG dev_msg_handle;

			memset(&tdm, 0, sizeof(tdm));
			memcpy(&tdm, iter->value, iter->len);

			dev_msg_handle.dev_id = in->getDevId();
			int rtn = th.tlv_dev_msg_handle(tdm, dev_msg_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.dev_msg_update(db, dev_msg_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDevMsg processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidParameter(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

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

	LOG_DEBUG("========= cidParameter processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidParameterVersion(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

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

	LOG_DEBUG("========= cidParameterVersion processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidManageLbs(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

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

	LOG_DEBUG("========= cidManageLbs processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataTh2(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataTh2 processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_TH2)
		{
			TLV_TH2 th_data;
			TLV_HANDLE_TH2 th_handle;

			memset(&th_data, 0, sizeof(th_data));
			memcpy(&th_data, iter->value, iter->len);

			th_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, th_handle.dev_id, th_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_th2_handle(th_data, th_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.th2_update(db, th_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}

			//通知 java
			session::HttpInterface hi;

			if(th_handle.alarm.substr(th_handle.alarm.length() - 1, 1) == "1" && th_handle.overflow == "0")
				hi.notifyJava(this->java_addr, th_handle.dev_id, "1", th_handle.t0, "1");

			if(th_handle.alarm.substr(th_handle.alarm.length() - 2, 1) == "1")
				hi.notifyJava(this->java_addr, th_handle.dev_id, "2", th_handle.h0, "1");

			if(th_handle.alarm.substr(th_handle.alarm.length() - 3, 1) == "1" && th_handle.overflow == "0")
				hi.notifyJava(this->java_addr, th_handle.dev_id, "1", th_handle.t1, "2");

			if(th_handle.alarm.substr(th_handle.alarm.length() - 4, 1) == "1")
				hi.notifyJava(this->java_addr, th_handle.dev_id, "2", th_handle.h1, "2");
		}
	}

	LOG_DEBUG("========= cidDataTh2 processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataExposure(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataExposure processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_EXPOSURE)
		{
			TLV_EXPOSURE exposure_data;
			TLV_HANDLE_EXPOSURE exposure_handle;

			memset(&exposure_data, 0, sizeof(exposure_data));
			memcpy(&exposure_data, iter->value, iter->len);

			exposure_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, exposure_handle.dev_id, exposure_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_exposure_handle(exposure_data, exposure_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.exposure_update(db, exposure_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDataExposure processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataShock(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataShock processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_SHOCK)
		{
			TLV_SHOCK shock_data;
			TLV_HANDLE_SHOCK shock_handle;

			memset(&shock_data, 0, sizeof(shock_data));
			memcpy(&shock_data, iter->value, iter->len);

			shock_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, shock_handle.dev_id, shock_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_shock_handle(shock_data, shock_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.shock_update(db, shock_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDataShock processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataTilt(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataTilt processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_TILT)
		{
			TLV_TILT tilt_data;
			TLV_HANDLE_TILT tilt_handle;

			memset(&tilt_data, 0, sizeof(tilt_data));
			memcpy(&tilt_data, iter->value, iter->len);

			tilt_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, tilt_handle.dev_id, tilt_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_tilt_handle(tilt_data, tilt_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.tilt_update(db, tilt_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDataTilt processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}


session::HANDLE_RTN UploadHandle::cidDataBaro(const protocol::PacketIn* in, protocol::PacketOut* out)
{
	bool error = false;

	stringstream ss;
	TlvHandle th;
	DbInterface di;

	TLVGroupPtr pTlvGrp = in->getTLVGroup();
	LOG_DEBUG("========= cidDataBaro processing begin, pTlvGrp->size() = " << pTlvGrp->size());

	for(TLVGroupIter iter = pTlvGrp->begin(); iter != pTlvGrp->end(); ++iter)
	{
		if(iter->type _EQ_ TID_BARO)
		{
			TLV_BARO baro_data;
			TLV_HANDLE_BARO baro_handle;

			memset(&baro_data, 0, sizeof(baro_data));
			memcpy(&baro_data, iter->value, iter->len);

			baro_handle.dev_id = in->getDevId();
			if(!di.public_get_order_id(db, baro_handle.dev_id, baro_handle.order_id))
			{
				//如果 in->getDevId()是无订单的，则直接移到关闭队列
				return session::HANDLE_RTN_CLOSE;
			}

			int rtn = th.tlv_baro_handle(baro_data, baro_handle);

			if(rtn _NEQ_ THR_OK)
			{
				error = true;
				continue;
			}

			rtn = di.baro_update(db, baro_handle);

			if(rtn _NEQ_ THR_TRUE)
			{
				error = true;
				continue;
			}
		}
	}

	LOG_DEBUG("========= cidDataBaro processing end.");

	#if 0
		//单次调试用，程序接收包处理后即退出
		return session::HANDLE_RTN_STOP;
	#else
		//正常逻辑
		if(error)
			return session::HANDLE_RTN_CONTINUE;
		else
			return session::HANDLE_RTN_REMOVE;
	#endif
}