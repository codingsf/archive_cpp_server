////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���ڹ���ɹ������� socket
//	File Property : private
//	Description :
//					I.�������̰߳�ȫ��
//					II.�����Ķ��������������ȫ�ֵģ����Ž����˳�������
//					III.���� Channel ��Ĺ۲��ߣ����۲콨�����ӵ� Channel��
//					IV. ThreadManager �ฺ��۲� SocketManager �����µ����ӵ���ʱ�����½��̴߳���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "socket_manager.h"

namespace PS_FW
{
	SocketManager::SocketManager(Channel* pChannel)
	{
		m_pMapSocket = new SocketMap;
		m_pListSocket = new SocketList;

		if(pthread_mutex_init(&m_mxMapLock,NULL))
		{
			delete m_pMapSocket;
		}

		if(pthread_mutex_init(&m_mxListLock,NULL))
		{
			delete m_pListSocket;
		}

		m_pChannel = pChannel;
		m_pChannel->Attach(this);
	}

	SocketManager::~SocketManager()
	{
		m_pChannel->Detach(this);
		delete m_pMapSocket;
		delete m_pListSocket;

		pthread_mutex_destroy(&m_mxMapLock);
		pthread_mutex_destroy(&m_mxListLock);
	}


	void SocketManager::Update(Subject* pSubject)
	{
		if(m_pChannel == pSubject)
		{
			//������۲�� Channel ������֪ͨ
			printf("[PS_FW::SocketManager]: receive notify from channel ! ");
			this->push(m_pChannel->m_sockP);
		}
	}


	int SocketManager::push(int sockId)
	{
		pthread_mutex_lock(&m_mxMapLock);
		m_pMapSocket->insert(std::make_pair(sockId,0));
		pthread_mutex_unlock(&m_mxMapLock);

		pthread_mutex_lock(&m_mxListLock);
		m_pListSocket->push_back(sockId);
		pthread_mutex_unlock(&m_mxListLock);

		//֪ͨ�۲��ߣ�ȡ�����µ� socket
		Notify();

		return 0;
	}


	int SocketManager::getNewSock(int& sockId)
	{
		pthread_mutex_lock(&m_mxListLock);
		if(!m_pListSocket->empty())
		{
			sockId = m_pListSocket->front();
			m_pListSocket->pop_front();
		}
		else
		{			
			pthread_mutex_unlock(&m_mxListLock);
			return -1;
		}
		pthread_mutex_unlock(&m_mxListLock);

		return 0;
	}
}	//PS_FW

