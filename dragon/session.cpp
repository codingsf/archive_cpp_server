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

#include "sys_include.h"
#include "_session.h"

namespace PS_FW
{
	Session::Session()
	{
		isStart = 0;
	}


	int Session::init(Protocol* pProtocol)
	{
		if(isStart == 0)
		{
			m_pProtocol = pProtocol;
			m_lenIn = 30960;
			memcpy(m_szMessageIn,'\0',m_lenIn);
			
			m_mapMsgTypeMap.insert(std::make_pair("<logon>1</logon>","PS_FW_LOGON"));
			m_mapMsgTypeMap.insert(std::make_pair("<logout>1</logout>","PS_FW_LOGOUT"));

			if(this->onInitMsgTypeMap(m_mapMsgTypeMap) != 0)
			{
				printf("\n[PS_FW::Session]: onInit fail ! \n");
				return -1;
			}

			isStart = 1;
		}

		return 0;
	}


	void Session::receive(const char* pMessageIn)
	{
		this->m_pProtocol->messageInit(pMessageIn,strlen(pMessageIn),m_szMessageIn,m_lenIn);
		this->onReceive(m_szMessageIn,m_lenIn);
	}
}	//PS_FW

