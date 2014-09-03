#ifndef __PS_FW_THREAD_MANAGER__
#define __PS_FW_THREAD_MANAGER__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ����
//	File Property : private
//	Description :
//					I. �����߳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "thread.h"
#include "channel.h"

namespace PS_FW
{
	class ThreadManager
	{
	public:
		ThreadManager();
		ThreadManager(Channel* pChannel);
		virtual ~ThreadManager(){}
	public:
		/* �����߳� */
		int openListenThread(const char* ip,const int port);
		/* �ͻ����߳� */
		int openConnThread(const char* ip,const int port);
		/* ע�� Channel */
		int attach(Channel* pChannel);
		/* �ȴ������̷߳��� */
		int join();
	public:
		char m_szIP[64];
		int m_port;
	public:
		Thread* m_pThreadListen;
		Channel* m_pChannel;
	};
}	//PS_FW

#endif //__PS_FW_THREAD_MANAGER__
