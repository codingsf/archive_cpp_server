#ifndef __PS_FW_CONNECTION_MANAGER__
#define __PS_FW_CONNECTION_MANAGER__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ���ӹ���
//	File Property : private
//	Description :
//					I.�����Ķ��������������ȫ�ֵģ����Ž����˳�������
//					II.���� SocketManager ��Ĺ۲���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "socket_manager.h"
#include "observer.h"

namespace PS_FW
{
	class ConnectionManager : public Observer
	{
	public:
		ConnectionManager(SocketManager* pSocketManager);
		virtual ~ConnectionManager();
	public:
		virtual void Update(Subject* pSubject);
	private:
		SocketManager* m_pSocketManager;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_CONNECTION_MANAGER__
