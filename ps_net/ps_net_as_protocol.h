#ifndef __PS_NET_AS_PROTOCOL__
#define __PS_NET_AS_PROTOCOL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//	File Property : public ����ͷ�ļ��Ƕ��⹫���ģ�
//	Description :	I. PS_FW::protocol �����࣬ʵ�־����Э��
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "ps_framework/ps_fw_protocol.h"

namespace PS_NET
{
	class AsProtocol : public PS_FW::Protocol
	{
	public:
		AsProtocol();
		virtual ~AsProtocol(){}
	public:
		char m_szAllowLogon[512];
	};
}	//PS_NET

#endif //__PS_NET_AS_PROTOCOL__
