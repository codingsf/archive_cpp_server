////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��9��9�� zengpw ��ȷ��ȫҪ��
//					2014��9��10�� zengpw �޸���ƣ��������ݳ�ʼ���ӿ�
//					2014��9��11�� zengpw ����һ�� session ��������һ�� protocol ����
//	File Property : public
//	Description :	
//					I. �������ݣ���д���ݽ�������
//					II. Ҫ�����̰߳�ȫ��
//					III. protocol ��������������ַ���
//					IV. protocol ���Ի��沿������
//					V. protocol�� virtual �������ǻ����ַ��������
//	Future :
//					I. �Ż��������㷨
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "_protocol.h"

namespace PS_FW
{
	Protocol::Protocol()
	{
		m_pos = 0;
		memset(m_szDataA,'\0',30960);
		memset(m_szDataB,'\0',30960);
		m_pData = m_szDataA;
		m_pDataNext = m_szDataB;
	}


	int Protocol::messageInit(const char* pMessageIn,const int lenIn,char* pMessageOut,int& lenOut)
	{
		int flag = 0;
		int pos = 0;

		//������
		memcpy(pMessageOut,'\0',lenOut);
		//д���ػ�����
		memcpy(m_pData + m_pos,pMessageIn,lenIn);
		m_pos += lenIn;
		// flag ȡֵ�� 1�������� || 2������ + ���ְ��� || 3�����ְ��� || 4���������
		// pos �� flag = 2 ʱ������ ��ƫ�Ƶ�ַ
		this->verifyPacket(m_pData,m_pData + m_pos,flag,pos);

		if(flag == 1)
		{
			lenOut = m_pos;
			memcpy(pMessageOut,m_pData,lenOut);
			/* ��򵥵��������յ�ǰ���������������λ����0 */
			memset(m_pData,'\0',30960);
			m_pos = 0;
		}
		else if (flag == 2)
		{
			lenOut = pos;
			memcpy(pMessageOut,m_pData,lenOut);
			/* ����ǰ����������δ��ȡ�Ĳ��֣��洢�� Next ��������Ȼ�� Current �� Next ָ�뽻�� */
			/* Next �����������´η�������ʱ����� */
			memset(m_pDataNext,'\0',30960);
			memcpy(m_pDataNext,m_pData + pos,m_pos - pos);
			char* m_pDataTemp = m_pData;
			m_pData = m_pDataNext;
			m_pDataNext = m_pDataTemp;
			m_pos -= pos;
		}
		else if (flag == 3)
		{
			lenOut = 0;
		}
		else if (flag == 4)
		{
			/* �������λ����0����������л����� */
			m_pos = 0;
			memset(m_pData,'\0',30960);
			memset(m_pDataNext,'\0',30960);
			lenOut = 0;
		}
		else
		{
			/* ʵ���ϲ������� */
			m_pos = 0;
			memset(m_pData,'\0',30960);
			memset(m_pDataNext,'\0',30960);
			lenOut = 0;
		}

		return 0;
	}


	int Protocol::verifyPacket(const char* szBegin,const char* szEnd,int& flag,int& pos)
	{
		/* ��Э��Ļ�������а����������� */
		flag = 1;

		return 0;
	}
}	//PS_FW
