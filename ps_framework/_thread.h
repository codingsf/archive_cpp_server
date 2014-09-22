#ifndef __PS_FW_THREAD__
#define __PS_FW_THREAD__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014-8-21
//	Modify history :
//					2014-9-19 zengpw �������
//					2014-9-22 zengpw �������
//	File Property : public
//	Description :
//					I. �ṩ���������̵߳ķ���
//					II. �ṩ������Ԫ�ع��̳߳�ʹ��
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_include.h"

namespace PS_FW
{
	typedef void*(*func_addr)(void*);
	typedef void* func_arg;

	class Thread
	{
	public:
		Thread();
		Thread(func_addr func,func_arg arg);
		virtual ~Thread();
	public:
		void join();
		int initLock();
		void destroyLock();
	public:
		/*---------------------
		// m_tid �̱߳�ţ��������ͣ�
		// m_condLock ������
		// m_mxLock �߳���
		// m_func �ص�����
		// m_arg �ص���������
		// m_status ����״̬��=0 wait �� =1 work
		---------------------*/
		pthread_t m_tid;
		pthread_cond_t m_condLock;
		pthread_mutex_t m_mxLock;
		func_addr m_func;
		func_arg m_arg;
		int m_status;
	};
}	//PS_FW

#endif //__PS_FW_THREAD__
