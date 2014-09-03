#ifndef __PS_FW_THREAD__
#define __PS_FW_THREAD__
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

#include "sys_include.h"

namespace PS_FW
{
	class Thread
	{
	public:
		Thread(void* (*threadCallback)(void*),void* pSubject);
		virtual ~Thread();
	public:
		pthread_t m_tid;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_THREAD__
