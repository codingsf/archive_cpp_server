////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��22�� zengpw �������
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description : 
//				I. �̳߳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "thread_pool.h"

namespace PS_FW
{
	ThreadPool::ThreadPool(void* (*threadCallback)(void*),void* pSubject,int size)
	{
		m_pool = new ThreadList;
		m_pool->clear();
		for (int i = 0; i < size; ++i)
		{
			Thread* pThread = new Thread(threadCallback,pSubject);
			append(pThread);
		}
	}

	ThreadPool::~ThreadPool()
	{
		if(m_pool != NULL)
		{
			ThreadList::iterator pos;
			for (pos = m_pool->begin();pos != m_pool->end();++pos)
			{
				delete (*pos);
			}

			delete m_pool;
		}
	}

	int ThreadPool::append(Thread* pThread)
	{
		m_pool->push_back(pThread);

		return 0;
	}
}	//PS_FW


