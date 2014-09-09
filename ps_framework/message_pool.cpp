////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//					2014��9��9�� zengpw �������
//	File Property : private
//	Description :
//				I. ��Ϣ����ͷ�ļ�
//				III. Ŀǰ���ǲ��� v1.3 ��� std::list ���Ժ����� std::queue �����ܻ�������
//				IV. ��Ϣ���л�����֪ͨ�䴦���̣߳��������Ҫ�̳��� subject
//
////////////////////////////////////////////////////////////////////////////////////////

#include "message_pool.h"

namespace PS_FW
{
	MessagePool::MessagePool()
	{
		if(pthread_mutex_init(&m_mxLock,NULL) == 0)
		{
			m_pPool = new Pool;
		}
	}


	MessagePool::~MessagePool()
	{
		while(!m_pPool->empty())
		{
			void* task = m_pPool->front();
			m_pPool->pop_front();
			free(task);
		}

		delete m_pPool;
		pthread_mutex_destroy(&m_mxLock);
	}


	int MessagePool::push(void* msg)
	{
		pthread_mutex_lock(&m_mxLock);
		m_pPool->push_back(msg);
		pthread_mutex_unlock(&m_mxLock);

		return 0;
	}


	int MessagePool::pushByChar(char* szMessage)
	{
		if(this->push((void*)szMessage) != 0)
		{
			return -1;
		}

		return 0;
	}


	int MessagePool::pop(void** msg)
	{
		pthread_mutex_lock(&m_mxLock);
		if(!m_pPool->empty())
		{
			*msg = m_pPool->front();
			m_pPool->pop_front();
		}
		else
		{			
			pthread_mutex_unlock(&m_mxLock);
			return -1;
		}
		pthread_mutex_unlock(&m_mxLock);

		return 0;
	}


	int MessagePool::popByChar(char* szMessage)
	{
		void* task = NULL;

		if(this->pop(&task) == 0)
		{
			szMessage = (char*)task;

			return 0;
		}

		return -1;
	}
}	//PS_FW

