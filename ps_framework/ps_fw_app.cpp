////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���� framework ���
//	File Property : public
//	Description :	I.���� framework �����
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "ps_fw_app.h"
#include "sys_include.h"
#include "channel.h"
#include "socket_manager.h"

namespace PS_FW
{
	App::App()
	{
		this->init();
	}

	int App::init()
	{
		//�������ڼ������ӻ��������ӵ� Channel
		m_pChannel =  new Channel;
		//���� Channel �Ĺ۲���
		m_pSocketManager = new SocketManager(m_pChannel);

		return 0;
	}


	int App::startServer(const char* ip,const int port)
	{
		m_pChannel->acceptConn(ip,port);

		return 0;
	}


	int App::startConn(const char* ip,const int port)
	{
		m_pChannel->conn(ip,port);

		return 0;
	}
}	//PS_FW
