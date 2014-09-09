#ifndef __PS_FW_THREAD__
#define __PS_FW_THREAD__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��9��5�� zengpw �������
//					2014��9��7�� zengpw �������
//					2014��9��9�� zengpw ���������ȴ�
//	File Property : private
//	Description :
//				I. �̻߳��࣬�ṩ�����̵߳ķ���
//				��. ���Ĺ��졢�����̵߳�һϵ�з������� protected �ģ���������Ҫ�̳�������ʹ����
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"

namespace PS_FW
{
	class Thread
	{
	public:
		Thread()
		{
			if(pthread_mutex_init(&m_mxLock,NULL) != 0)
			{
				delete this;
				printf("\n[PS_FW::Thread] thread_mutex_init fail!");
			}
		}


		virtual ~Thread()
		{
			pthread_mutex_destroy(&m_mxLock);
		}


		void join()
		{
			while (1)
			{
				//�ȴ�������Ŀǰֻ������ѭ��
			}
		}
	protected:
		void run(void*(*pObjectCallBackFunc)(void*),void* pObject)
		{
			printf("\n[PS_FW::Thread] thread create start ! \n");
			if( pthread_create(&this->m_tid,NULL,pObjectCallBackFunc,pObject) == 0 )
			{
				pthread_detach(this->m_tid);
				printf("\n[PS_FW::Thread] thread create success ! \n");
			}
		}
	protected:
		pthread_t m_tid;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_THREAD__
