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
//					IV. ThreadManager �ฺ��۲� SocketManager �����µ����ӵ���ʱ�����½��̴߳���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "channel.h"
#include "subject.h"
#include "observer.h"

namespace PS_FW
{
	typedef int SocketId;
	typedef int SocketStatus;
	typedef std::list<SocketId> SocketList;
	typedef std::map<SocketId,SocketStatus> SocketMap;

	class SocketManager : public Observer,public Subject
	{
	public:
		SocketManager(Channel* pChannel);
		virtual ~SocketManager();
	public:
		virtual void Update(Subject* pSubject);
		/* Ŀǰֻ�ṩ��ط��� */
		int push(int sockId);
		/* �ṩ�����̻߳�ȡ���� */
		int getNewSock(int& sockId);
	private:
		Channel* m_pChannel;
		/* ����� socket */
		SocketMap* m_pMapSocket;
		/* ��δ�������ӵ� socket */
		SocketList* m_pListSocket;
		/* �� */
		pthread_mutex_t m_mxMapLock;
		pthread_mutex_t m_mxListLock;
	};
}	//PS_FW

#endif //__PS_FW_SOCKET_MANAGER__
