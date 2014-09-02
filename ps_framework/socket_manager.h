#ifndef __PS_FW_SOCKET_MANAGER__
#define __PS_FW_SOCKET_MANAGER__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���ڹ���ɹ������� socket
//	File Property : private
//	Description :
//					I.�������̰߳�ȫ��
//					II.�����Ķ��������������ȫ�ֵģ����Ž����˳�������
//					III.���� Channel ��Ĺ۲��ߣ����۲콨�����ӵ� Channel��
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "channel.h"
#include "observer.h"

namespace PS_FW
{
	typedef int SocketId;
	typedef int SocketStatus;
	typedef std::map<SocketId,SocketStatus> SocketMap;

	class SocketManager : public Observer
	{
	public:
		SocketManager(Channel* pChannel);
		virtual ~SocketManager();
	public:
		virtual void Update(Subject* pSubject);
		/* Ŀǰֻ�ṩ��ط��� */
		int push(SocketId socketId);
	private:
		Channel* m_pChannel;
		SocketMap* m_pMapSocket;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_SOCKET_MANAGER__
