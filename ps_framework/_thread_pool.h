#ifndef __PS_FW_THREAD_POOL__
#define __PS_FW_THREAD_POOL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014-8-21
//	Modify history :
//					2014-9-19 zengpw �������
//					2014-9-22 zengpw �������
//	File Property : public
//	Description :
//					I. �ṩ�̳߳أ�ȫ��Ψһ
//	Future :
//					I. ���¿����������
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_include.h"
#include "_thread.h"

namespace PS_FW
{
	class ThreadPool
	{
	public:
		explicit ThreadPool(int size);
		virtual ~ThreadPool();
	public:
		int add(func_addr func,func_arg arg);
		void destroy();
	private:
		bool isEmpty(void* pool);
		static void* loop(void* pool);
	private:
		/*---------------------
		// m_count �̳߳����ü���
		// m_status �̳߳�״̬��=0 close �� =1 open
		// m_vcThread ���ڵ��߳�
		// m_mxLock ����
		---------------------*/
		int m_count;
		int m_status;
		std::vector<Thread*> m_vcThread;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_THREAD_POOL__
