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

#include "sys_include.h"
#include "_protocol.h"

namespace PS_FW
{
	int Protocol::verifyPacket(const char* szBegin,const char* szEnd,int& flag,int& pos)
	{
		/* ��Э��Ļ�������а����������� */
		flag = 1;

		return 0;
	}
}	//PS_FW
