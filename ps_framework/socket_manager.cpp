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
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "socket_manager.h"

namespace PS_FW
{
	SocketManager::SocketManager(Channel* pChannel)
	{
		m_pMapSocket = new SocketMap;

		if(pthread_mutex_init(&m_mxLock,NULL))
		{
			delete m_pMapSocket;
		}
		else
		{
			m_pChannel = pChannel;
			m_pChannel->Attach(this);
		}
	}

	SocketManager::~SocketManager()
	{
		m_pChannel->Detach(this);
		delete m_pMapSocket;

		pthread_mutex_destroy(&m_mxLock);
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


	int SocketManager::push(SocketId socketId)
	{
		pthread_mutex_lock(&m_mxLock);
		//TODO ֪ͨ app �ഴ�� ��ȡ������ �߳�
		pthread_mutex_unlock(&m_mxLock);

		return 0;
	}
}	//PS_FW

