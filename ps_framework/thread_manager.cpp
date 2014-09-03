////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ����
//	File Property : public
//	Description :
//					I. �����߳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "thread_manager.h"

namespace PS_FW
{
	ThreadManager::ThreadManager()
	{
		m_pChannel = NULL;
	}

	ThreadManager::ThreadManager(Channel* pChannel)
	{
		this->attach(pChannel);
	}

	int ThreadManager::attach(Channel* pChannel)
	{
		m_pChannel = pChannel;

		return 0;
	}

	int ThreadManager::join()
	{
		while (1)
		{
			//Ŀǰֻ������ѭ��
		}

		return 0;
	}


	void* openListenThreadCallBack(void* arg)
	{
		ThreadManager* pThreadManager = (ThreadManager*)arg;
		pThreadManager->m_pChannel->acceptConn(pThreadManager->m_szIP,pThreadManager->m_port);

		return NULL;
	}

	int ThreadManager::openListenThread(const char* ip,const int port)
	{
		assert(m_pChannel != NULL);

		strncpy(m_szIP,ip,64);
		m_port = port;

		m_pThreadListen = new Thread(openListenThreadCallBack,this);

		return 0;
	}


	int ThreadManager::openConnThread(const char* ip,const int port)
	{
		assert(m_pChannel != NULL);

		m_pChannel->conn(ip,port);
		return 0;
	}
}	//PS_FW

