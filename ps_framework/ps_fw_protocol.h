#ifndef __PS_FW_PROTOCOL__
#define __PS_FW_PROTOCOL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//	File Property : public ����ͷ�ļ��Ƕ��⹫���ģ�
//	Description :	I. ������Э��Ļ��࣬framework�ṩ��ͷ�ļ��� protocol ��ʹ��
//					II. ��ΪЭ��Ļ��࣬ͬʱ�ṩһЩͨ�õ��ַ���������
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
