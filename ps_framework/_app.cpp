////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���� framework ���
//					2014��9��3�� zengpw �����̹߳���
//					2014��9��7�� zengpw �޸ģ���С�� app �ܳ��е������
//	File Property : public
//	Description :	I.���� framework �����
//					II.���Ե��� App �� public ���������������������߿ͻ���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_app.h"
#include "sys_include.h"
#include "event.h"
#include "connection.h"

namespace PS_FW
{
	App::App()
	{
		m_pProtocol = new Protocol;
		m_pSession = new Session;
		m_pConnection = new Connection;
		m_pConnection->setProtocol(m_pProtocol);
		m_pConnection->setSession(m_pSession);

		m_pEvent = new Event;
		m_pEvent->AttachConnection(m_pConnection);
		m_pEvent->AttachSession(m_pSession);
	}


	App::~App()
	{
		//TODO: ��Ҫ������ΰ�ȫ������APP���������еĶ���Ŀǰ�����κδ���
	}


	int App::server(const char* ip,const int port)
	{
		return m_pConnection->server(ip,port);
	}


	int App::client(const char* ip,const int port)
	{
		return m_pConnection->client(ip,port);
	}


	void App::join()
	{
		return m_pConnection->join();
	}
}	//PS_FW
