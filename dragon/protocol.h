#ifndef __PS_FW_PROTOCOL__
#define __PS_FW_PROTOCOL__
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

namespace PS_FW
{
	class Protocol
	{
	public:
		Protocol();
		virtual ~Protocol(){}
	public:
		//У����յİ��Ƿ���������Э���һ���֣���ʹ���߼̳�ʵ��
		// szBegin ����ʼ��ַ
		// szEnd ��������ַ
		// flag ȡֵ�� 1�������� || 2������ + ���ְ��� || 3�����ְ��� || 4���������
		// pos �� flag = 2 ʱ������ ��ƫ�Ƶ�ַ
		virtual int verifyPacket(const char* szBegin,const char* szEnd,int& flag,int& pos);
	public:
		//���յ��İ����г�ʼ�����˺�������Ҫ�û��̳У��û�ֻ��Ҫ��ʵ�������д verifyPacket()
		int messageInit(const char* pMessageIn,const int lenIn,char* pMessageOut,int& lenOut);
	private:
		/*---------------------
		// use : ��ȡ����ʹ��
		// m_szData A B ��2��������
		// m_pData ָ��ǰʹ�õ�������
		// m_pDataNext ָ���´�ʹ�õ�������
		// m_pos ��������ʵ�ʽ�����ַ��ƫ����
		---------------------*/
		char m_szDataA[30960];
		char m_szDataB[30960];
		char* m_pData;
		char* m_pDataNext;
		int m_pos;
	};
}	//PS_FW

#endif //__PS_FW_PROTOCOL__
