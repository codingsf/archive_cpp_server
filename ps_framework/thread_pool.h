#ifndef __PS_FW_THREAD_POOL__
#define __PS_FW_THREAD_POOL__
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
////////////////////////////////////////////////////////////////////////////////////////

#include "thread.h"

namespace PS_FW
{
	typedef std::list<Thread*> ThreadList;

	class ThreadPool
	{
	public:
		ThreadPool(void* (*threadCallback)(void*),void* pSubject,int size);
		virtual ~ThreadPool();
	public:
		int append(Thread* pThread);
	protected:
		ThreadList* m_pool;
	};
}	//PS_FW

#endif //__PS_FW_THREAD_POOL__

