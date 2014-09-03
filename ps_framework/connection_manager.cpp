////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ���ӹ���
//	File Property : private
//	Description :
//					I.�����Ķ��������������ȫ�ֵģ����Ž����˳�������
//					II.���� SocketManager ��Ĺ۲���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "connection_manager.h"

namespace PS_FW
{
	ConnectionManager::ConnectionManager(SocketManager* pSocketManager)
	{
		if(pthread_mutex_init(&m_mxLock,NULL) == 0)
		{
			m_pSocketManager = pSocketManager;
			m_pSocketManager->Attach(this);
		}
	}

	ConnectionManager::~ConnectionManager()
	{
		m_pSocketManager->Detach(this);

		pthread_mutex_destroy(&m_mxLock);
	}


	void ConnectionManager::Update(Subject* pSubject)
	{
		if(m_pSocketManager == pSubject)
		{
			// when new socket coming
			printf("[PS_FW::ThreadManager]: receive socket from socketmanager ! ");
			int sockId = 0;
			if(m_pSocketManager->getNewSock(sockId) == 0)
			{
				printf("[PS_FW::ThreadManager]: receive socket ok ! ");
			}
		}
	}
}	//PS_FW

