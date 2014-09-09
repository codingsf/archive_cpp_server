#ifndef __PS_FW_CONNECTION__
#define __PS_FW_CONNECTION__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ���ӹ���
//					2014��9��7�� zengpw ���ʵ��
//					2014��9��9�� zengpw �淶ע�ͣ���Ӵ����߳�
//	File Property : private
//	Description :
//					I.�����Ķ��������������ȫ�ֵģ����Ž����˳�������
//					��.��ӵ�� �������� �� �������� 2�ַ�ʽ
//					��.��ֱ�ӳ��� Channel ����
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "thread.h"
#include "channel.h"
#include "_protocol.h"
#include "_session.h"

namespace PS_FW
{
	class Connection : public Thread
	{
	public:
		Connection();
		virtual ~Connection();
	public:
		int setProtocol(Protocol* pProtocol);
		int setSession(Session* pSession);
	public:
		int server(const char* ip,const int port);
		int client(const char* ip,const int port);
		int read(const int sockId);
		int send(const int sockId);
		static void* dealThread(void* pConnection);
	public:
		/*---------------------
		// use : ���õĶ���
		// m_pChannelServer �����߳�
		// m_pChannelClient �����߳�
		// m_pChannelRead ���ݶ�ȡ�߳�
		// m_pChannelSend ���ݷ����߳�
		---------------------*/
		Channel* m_pChannelServer;
		Channel* m_pChannelClient;
		Channel* m_pChannelRead;
		Channel* m_pChannelSend;
		/*---------------------
		// use : �ⲿ�Ķ���
		// m_pProtocol ʹ�õ�Э��
		// m_pSession ʹ�õ�session
		---------------------*/
		Protocol* m_pProtocol;
		Session* m_pSession;
	};
}	//PS_FW

#endif //__PS_FW_CONNECTION__
