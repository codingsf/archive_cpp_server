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
//					I. �������Ӻ����ݣ���д��������
//					II. Ҫ�����̰߳�ȫ��
//					III. session �� virtual �������ǻ����¼��ģ����ǻ�Ԥ�ȶ�����������
//					IV. session ���Ի��沿������
//	Future :
//					I. ʵ�ָ����������
//					II. ʵ��һ�����ұȽϿ�����ݽṹ
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_protocol.h"

namespace PS_FW
{
	typedef std::map<std::string,std::string> MsgTypeMap;

	class Session
	{
	public:
		Session();
		virtual ~Session(){}
	public:
		virtual int onReceive(const char* pMessageIn,const int lenIn) = 0;
		virtual int onInitMsgTypeMap(MsgTypeMap& pMsgTypeMap) = 0;
	public:
		void receive(const char* pMessageIn);
		int init(Protocol* pProtocol);
	protected:
		//�ַ��� - ��Ϣ����
		MsgTypeMap m_mapMsgTypeMap;
		/*---------------------
		// use : session��������
		// m_szMessageIn session����������
		// m_szMessageOut session���������
		---------------------*/
		int m_lenIn;
		int m_lenOut;
		char m_szMessageIn[30960];
		char m_szMessageOut[30960];
	private:
		int isStart;
		Protocol* m_pProtocol;
	};
}	//PS_FW

#endif //__PS_FW_SESSION__
