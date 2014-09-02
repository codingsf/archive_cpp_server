#ifndef __PS_FW_APP__
#define __PS_FW_APP__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ���� framework ���
//	File Property : public
//	Description :	I.���� framework �����
//					II.���Ե��� App �� public ���������������������߿ͻ���
//
//
////////////////////////////////////////////////////////////////////////////////////////

namespace PS_FW
{
	class Channel;
	class SocketManager;

	class App
	{
	public:
		App();
		virtual ~App(){}
	public:
		/* 
		//��������ˣ�Ŀǰ�̶̹߳��� 1 server 1 accept ; 1 client 1 read 3 deal
		// 1������ˣ�1�������̣߳�����ÿ�����ӵĿͻ��ˣ�����1���������̣߳�3�����������߳�
		*/
		int startServer(const char* ip,const int port);
		/* �����ͻ��ˣ�������ӳɹ��������1���������̣߳�3�����������߳� */
		int startConn(const char* ip,const int port);
	private:
		int init();
	private:
		/* channel for accept */
		Channel* m_pChannel;
		/* socket manager */
		SocketManager* m_pSocketManager;
	};
}	//PS_FW

#endif //__PS_FW_APP__
