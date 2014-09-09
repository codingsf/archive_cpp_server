#ifndef __PS_FW_PROTOCOL__
#define __PS_FW_PROTOCOL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//					2014��9��3�� zengpw �޸Ķ���
//					2014��9��9�� zengpw ��ȷ��ȫҪ��
//	File Property : public
//	Description :	
//					I.�������ӣ���д���ݽ�������
//					II. ������Э��Ļ��࣬framework�ṩ��ͷ�ļ��� protocol ��ʹ��
//					III. ��ΪЭ��Ļ��࣬ͬʱ�ṩһЩͨ�õ��ַ���������
//					IV. Ҫ�����̰߳�ȫ��
//
//
////////////////////////////////////////////////////////////////////////////////////////

namespace PS_FW
{
	class Protocol
	{
	public:
		Protocol(){}
		virtual ~Protocol(){}
	public:
		//У����յİ��Ƿ���������Э���һ���֣���ʹ���߼̳�ʵ��
		// szBegin ����ʼ��ַ
		// szEnd ��������ַ
		// flag ȡֵ�� 1�������� || 2������ + ���ְ��� || 3�����ְ��� || 4���������
		// pos �� flag = 2 ʱ������ ��ƫ�Ƶ�ַ
		virtual int verifyPacket(const char* szBegin,const char* szEnd,int& flag,int& pos);
	};
}	//PS_FW

#endif //__PS_FW_PROTOCOL__
