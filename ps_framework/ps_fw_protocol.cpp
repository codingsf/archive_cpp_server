////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description :	I. ������Э��Ļ��࣬framework�ṩ��ͷ�ļ��� protocol ��ʹ��
//					II. ��ΪЭ��Ļ��࣬ͬʱ�ṩһЩͨ�õ��ַ���������
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "ps_fw_protocol.h"

namespace PS_FW
{
	int Protocol::verifyPacket(const char* szBegin,const char* szEnd,int& flag,int& pos)
	{
		/* ��Э��Ļ�������а����������� */
		flag = 1;

		return 0;
	}
}	//PS_FW
