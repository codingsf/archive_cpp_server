#ifndef __PS_FW_SESSION__
#define __PS_FW_SESSION__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ����
//					2014��9��9�� zengpw ������Ϣ����ѭ�����ṩ�ӿ�
//	File Property : public
//	Description :
//					I. �������ӣ���д��������
//					II. session Ҫ�����̰߳�ȫ
//
//
////////////////////////////////////////////////////////////////////////////////////////

namespace PS_FW
{
	class Session
	{
	public:
		Session(){}
		virtual ~Session(){}
	public:
		virtual int getMessageType(char* szType);
		virtual int dealLogon();
		virtual int dealLogout();
	public:
		//������������
		int setMessageIn(const char* pMessageIn);
		//������������
		virtual int dealMessage();
		//��ȡ�������
		int getMessageOut(char* pMessageOut,int len);
	protected:
		/*---------------------
		// use : session��������
		// m_szMessageIn session����������
		// m_szMessageOut session��������� 
		---------------------*/
		char m_szMessageIn[10240];
		char m_szMessageOut[10240];
	};
}	//PS_FW

#endif //__PS_FW_SESSION__
