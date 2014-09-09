#ifndef __PS_FW_EVENT__
#define __PS_FW_EVENT__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���ڹ���ɹ������� socket
//					2014��9��8�� zengpw ���ڹ������з������¼�
//					2014��9��9�� zengpw ���Ʒ�����¼�
//	File Property : private
//	Description :
//					I. �¼��۲���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "subject.h"
#include "observer.h"
#include "channel.h"
#include "connection.h"
#include "_session.h"

namespace PS_FW
{
	class Event : public Observer
	{
	public:
		Event();
		virtual ~Event();
	public:
		int AttachConnection(Connection* pConnection);
		int AttachSession(Session* pSession);
	public:
		virtual void Update(Subject* pSubject);
	private:
		/*---------------------
		// use : ����Connection�����ڲ�ͨ��
		// m_pConnection ����ʵ��ĳЩ������Ĳ���
		// m_pChannelServer �������
		// m_pChannelClient ���Ӽ���
		// m_pChannelRead ���ݵ������
		---------------------*/
		Connection* m_pConnection;
		Channel* m_pChannelServer;
		Channel* m_pChannelClient;
		Channel* m_pChannelRead;
		/*---------------------
		// use : ����Session
		// m_pSession ��Ӧ�� Connection �� Session
		---------------------*/
		Session* m_pSession;
	};
}	//PS_FW

#endif //__PS_FW_EVENT__
