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

#include "print.h"

namespace DRAGON
{
	int Print::welcome()
	{
		printf("\n");
		printf("      ==================================================\n");
		printf("      =                                                =\n");
		printf("      =    Dragon                                      =\n");
		printf("      =      v 0.01                                    =\n");
		printf("      =                                                =\n");
		printf("      ==================================================\n");
		printf("\n");

		return 0;
	}

	int Print::choseProgramType()
	{
		printf("\n");
		printf("      ==================================================\n");
		printf("      =                                                =\n");
		printf("      =    please chose the program type               =\n");
		printf("      =                                                =\n");
		printf("      =    1.  server                                  =\n");
		printf("      =    2.  client                                  =\n");
		printf("      =                                                =\n");
		printf("      ==================================================\n");
		printf("\n");

		return 0;
	}

	int Print::choseProgramTypeResult()
	{
		printf("\n");
		printf("must be press 1 or 2 \n");
		printf("\n");

		return 0;
	}
}	//DRAGON

