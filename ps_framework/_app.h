#ifndef __PS_FW_APP__
#define __PS_FW_APP__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���� framework ���
//					2014��9��3�� zengpw �����̹߳���
//					2014��9��7�� zengpw �޸ģ���С�� app �ܳ��е������
//	File Property : public
//	Description :	I.���� framework �����
//					II.���Ե��� App �� public ���������������������߿ͻ���
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_protocol.h"
#include "_session.h"

namespace PS_FW
{
	class Event;
	class Connection;

	class App
	{
	public:
		App();
		virtual ~App();
	public:
		//��������ˣ�Ŀǰ�̶̹߳��� 1 server 1 accept ; 1 client 1 read 3 deal
		// 1������ˣ�1�������̣߳�����ÿ�����ӵĿͻ��ˣ�����1���������̣߳�3�����������߳�
		int server(const char* ip,const int port);

		/* �����ͻ��ˣ�������ӳɹ��������1���������̣߳�3�����������߳� */
		int client(const char* ip,const int port);

		/* �ȴ��̷߳��� */
		void join();
	private:
		/*---------------------
		// use : ���еı���
		// m_pProtocol ʹ�õ�Э��
		// m_pSession
		// m_pConnection �� �� session ��1��1��ϵ
		// m_pEvent �¼�����
		---------------------*/
		Protocol* m_pProtocol;
		Session* m_pSession;
		Connection* m_pConnection;
		Event* m_pEvent;
		/*---------------------
		// use : ���е���Ϣ��
		//
		---------------------*/
	};
}	//PS_FW

#endif //__PS_FW_APP__
