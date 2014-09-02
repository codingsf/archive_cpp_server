#ifndef __PS_FW_THREAD__
#define __PS_FW_THREAD__
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

#include "sys_include.h"

namespace PS_FW
{
	class Thread
	{
	public:
		Thread();
		virtual ~Thread(){}
	public:
		virtual void OnThreadRun(){}
	public:
		pthread_t m_tid;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_THREAD__
