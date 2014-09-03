////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description :
//				I. �������ͷ�ļ�
//				II.  ���ն����� IO �߳� put �������߳� get �����Ͷ���ֻ�� �����߳� put
//				III. Ŀǰ���ǲ��� v1.3 ��� std::list ���Ժ����� std::queue �����ܻ�������
//
////////////////////////////////////////////////////////////////////////////////////////

#include "msg_queue.h"

namespace PS_FW
{
	MsgQueue::MsgQueue()
	{
		m_queue = new MsgList;

		if(pthread_mutex_init(&m_mxLock,NULL))
		{
			destory();
		}
	}


	int MsgQueue::put(void* msg)
	{
		pthread_mutex_lock(&m_mxLock);
		m_queue->push_back(msg);
		pthread_mutex_unlock(&m_mxLock);
		Notify();

		return 0;
	}


	int MsgQueue::get(void** msg)
	{
		pthread_mutex_lock(&m_mxLock);
		if(!m_queue->empty())
		{
			*msg = m_queue->front();
			m_queue->pop_front();
		}
		else
		{			
			pthread_mutex_unlock(&m_mxLock);
			return -1;
		}
		pthread_mutex_unlock(&m_mxLock);

		return 0;
	}


	int MsgQueue::destory()
	{
		while(!m_queue->empty())
		{
			void* task = m_queue->front();
			m_queue->pop_front();
			/* �д��Ľ� */
			free(task);
		}

		delete m_queue;

		pthread_mutex_destroy(&m_mxLock);

		return 0;
	}
}	//PS_FW


