////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���� framework ���
//					2014��9��3�� zengpw �����̹߳���
//	File Property : public
//	Description :	I.���� framework �����
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "_app.h"
#include "channel.h"
#include "socket_manager.h"
#include "thread_manager.h"

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
		//���� ThreadManager ����ע����� Channel
		m_pThreadManager = new ThreadManager(m_pChannel);
		return 0;
	}


	int App::startServer(const char* ip,const int port)
	{
		return m_pThreadManager->openListenThread(ip,port);
	}


	int App::startConn(const char* ip,const int port)
	{
		return m_pThreadManager->openConnThread(ip,port);
	}


	int App::join()
	{
		return m_pThreadManager->join();
	}
}	//PS_FW
