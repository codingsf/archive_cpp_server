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
//					2014��9��10�� zengpw ������Ϣ��
//	File Property : public
//	Description :	
//					I.���� framework �����
//					II.���Ե��� App �� public ���������������������߿ͻ���
//	Future :
//					I. �ṩ��ȫ���ٵ�ʵ��
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_session.h"

namespace PS_FW
{
	class Event;
	class Connection;
	class Task;

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
		// m_pSession
		// m_pMessagePool ��Ϣ��
		// m_pConnection
		// m_pEvent �¼�����
		---------------------*/
		Session* m_pSession;
		Task* m_pMessagePool;
		Connection* m_pConnection;
		Event* m_pEvent;
	};
}	//PS_FW

#endif //__PS_FW_APP__
