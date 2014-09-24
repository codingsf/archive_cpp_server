#ifndef __PS_FW_THREAD_POOL__
#define __PS_FW_THREAD_POOL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014-8-21
//	Modify history :
//					2014-9-19 zengpw �������
//					2014-9-22 zengpw �������
//					2014-9-24 zengpw �������
//	File Property : public
//	Description :
//					I. �ṩ�̳߳أ�ȫ��Ψһ
//					II. �߳������ǹ̶��ģ������̱߳����ǿɸ��õ�
//	Future :
//					I. ���¿����������
//					II. ������ loop() �����ŵĻ�ȡ���߳�����
//
//
////////////////////////////////////////////////////////////////////////////////////////

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
	private:
		static void* loop(void* pool);
	private:
		/*---------------------
		// m_st �̳߳�״̬�� =0 close �� =1 open
		// m_count �̳߳����ü���
		// m_condLock ��������
		// m_lock ����
		// pThread ָ��ոմ������Ǹ��߳�
		// m_vcThread ���ڵ��߳�
		---------------------*/
		int m_st;
		int m_count;
		pthread_cond_t m_condLock;
		pthread_mutex_t m_lock;
		std::vector<Thread*> m_vcThread;
	};
}	//PS_FW

#endif //__PS_FW_THREAD_POOL__
