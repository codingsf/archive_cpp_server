#ifndef __PS_FW_THREAD__
#define __PS_FW_THREAD__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014-8-21
//	Modify history :
//					2014-9-19 zengpw �������
//					2014-9-22 zengpw �������
//					2014-9-24 zengpw �Ż�˵�������
//	File Property : public
//	Description :
//					I. �ṩ���������̵߳ķ���
//					II. ������Ƶ�Ԫ����Ϊ�̳߳�ʹ�õ�
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
		explicit Thread(func_addr func,func_arg arg);
		virtual ~Thread();
	public:
		// set get ����
		int getStatus();
		void setStatus(int st);
		int getEventStatus();
		void setEventStatus(int st);
		// wait thread
		void join();
		/*---------------------
		// attach() ע�ắ�������̳߳ػ���ػص�ʹ��
		// detach() ��ע��
		// run() ���лص�
		---------------------*/
		void attach(func_addr func,func_arg arg);
		void detach();
		void callEvent();
	public:
		/*---------------------
		// m_tid �̱߳�ţ��������ͣ�
		---------------------*/
		pthread_t m_tid;
	private:
		/*---------------------
		// m_status ����״̬�� =0 wait �� =1 work
		// m_mxLock �߳���
		---------------------*/
		int m_status;
		pthread_mutex_t m_mxLock;
		/*---------------------
		// m_run �Ƿ���ִ�лص��¼��� =-1 unregist event , =0 wait �� =1 running
		// m_func �ص�������רΪ�̳߳�ʹ��
		// m_arg �ص���������
		---------------------*/
		int m_event;
		func_addr m_func;
		func_arg m_arg;
	};
}	//PS_FW

#endif //__PS_FW_THREAD__
