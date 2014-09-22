#ifndef __PS_FW_SYS_FUNC__
#define __PS_FW_SYS_FUNC__
//////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��3��
//	Modify history :
//					2014��9��3�� zengpw ����
//					2014��9��4�� zengpw add func
//	File Property : public
//	Description : 
//					��. �ṩһЩС�����Ŀ�ƽ̨ʵ��
//
//////////////////////////////////////////////////////////////////////////

#include "_include.h"

namespace PS_FW
{
	class SYS_FUNC
	{
	public:
		/* ���ߣ�����Ϊ���� */
		static void sleep(int milliseconds)
		{
#ifdef _MSC_VER
			Sleep(milliseconds);
#endif //_MSC_VER
#ifdef __GNU_C__
			usleep(milliseconds*1000);
#endif //__GNU_C__
		}
	};	//SYS_FUNC
}	//PS_FW
#endif //__PS_FW_SYS_FUNC__
