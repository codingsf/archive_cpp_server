#ifndef __DRAGON_PRINT__
#define __DRAGON_PRINT__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ������Ļ��ӡ
//	File Property : private
//	Description :	I. ������Ļ��ʾ��Ϣ�Ĵ�ӡ
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"

namespace DRAGON
{
	class Print
	{
	public:
		Print(){}
		virtual ~Print(){}
	public:
		static int welcome();
		static int choseProgramType();
		static int choseProgramTypeResult();
	};
}	//DRAGON

#endif //__DRAGON_PRINT__
