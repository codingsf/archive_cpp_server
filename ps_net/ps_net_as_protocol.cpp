////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description :	I. PS_FW::protocol �����࣬ʵ�־����Э��
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "ps_net_as_protocol.h"

namespace PS_NET
{
	AsProtocol::AsProtocol()
	{
		strncpy(m_szAllowLogon,"<?xml version=\"1.0\"?><cross-domain-policy><site-control permitted-cross-domain-policies=\"all\"/><allow-access-from domain=\"*\" to-ports=\"*\"/></cross-domain-policy>",512);
	}
}	//PS_NET
