#ifndef __DRAGON_IO__
#define __DRAGON_IO__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��9��2��
//	Modify history :
//					2014��9��2�� zengpw ������Ļ���������е������
//	File Property : private
//	Description :	I. ����һЩ��Ļ���������е������
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"

namespace DRAGON
{
	class IO
	{
	public:
		IO(){}
		virtual ~IO(){}
	public:
		static int getProgramType();
	};
}	//DRAGON

#endif //__DRAGON_IO__
