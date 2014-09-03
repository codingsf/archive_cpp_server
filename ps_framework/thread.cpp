////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��22�� zengpw �������
//					2014��9��3�� zengpw �������
//	File Property : private
//	Description : 
//				I. �߳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "thread.h"

namespace PS_FW
{
	Thread::Thread(void* (*threadCallback)(void*),void* pSubject)
	{
		if(pthread_mutex_init(&m_mxLock,NULL) != 0)
		{
			delete this;
			printf("\n[PS_FW::Thread] thread_mutex_init fail!");
		}

		/* �˴�����ʹ�� pthread �⣬���洫���̵߳ĵ�ַ�������Ǻ�����ַ */
		if( pthread_create(&this->m_tid,NULL,threadCallback,pSubject) == 0 )
		{
			pthread_detach(this->m_tid);
			printf("\n[PS_FW::Thread] pthread_create ok! \n");
		}
	}

	Thread::~Thread()
	{
		pthread_mutex_destroy(&m_mxLock);
		exit;
	}
}	//PS_FW

