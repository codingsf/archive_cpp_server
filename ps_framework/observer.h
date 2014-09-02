#ifndef __PS_FW_OBSERVER__
#define __PS_FW_OBSERVER__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��22��
//	Modify history :
//					2014��8��22�� zengpw ����
//					2014��9��1�� zengpw �������
//	File Property : private
//	Description :	I. �¼��۲��ߣ������� Subject ����
//					II. Subject ���������¼�����ʱ�����������ù۲��ߵ� Update �ӿ�
//					III. ������࣬�޷������죬ֻ�ܱ��̳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

namespace PS_FW
{
	class Subject;

	class Observer
	{
	public:
		virtual ~Observer(){}
	public:
		virtual void Update(Subject* pSubject) = 0;
	protected:
		Observer(){}
	};
}	//PS_FW

#endif //__PS_FW_OBSERVER__
