////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��22�� zengpw �������
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description : 
//				I. �߳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "thread.h"

namespace PS_FW
{
	//Ϊ��ƽ̨�� pthread д�� C ���ص�����
	void* threadCallback(void* arg)
	{
		Thread* pThread = (Thread*)arg;
		pThread->OnThreadRun();

		return NULL;
	}

	Thread::Thread()
	{
		if(pthread_mutex_init(&this->m_mxLock,NULL) != 0)
		{
			delete this;
			printf("\n[ps_fw_framework] thread_mutex_init fail!");
		}

		/* �˴�����ʹ�� pthread �⣬���洫���̵߳ĵ�ַ�������Ǻ�����ַ */
		if( pthread_create(&this->m_tid,NULL,threadCallback,(void*)this) == 0 )
		{
			pthread_detach(this->m_tid);
		}
	}
}	//PS_FW

