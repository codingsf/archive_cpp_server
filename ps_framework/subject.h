#ifndef __PS_FW_SUBJECT__
#define __PS_FW_SUBJECT__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��22��
//	Modify history :
//					2014��8��22�� zengpw ����
//					2014��9��1�� zengpw �������
//	File Property : private
//	Description :	I. �ṩ���������¼������
//					II. ���һ����Ҫ����֪ͨ�����࣬����Լ̳��� Subject
//					III. ������࣬�޷������죬ֻ�ܱ��̳�
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "observer.h"

namespace PS_FW
{
	class Subject
	{
	public:
		virtual ~Subject(){}
	public:
		virtual void Attach(Observer* pObserver)
		{
			m_pListObserver->push_back(pObserver);
		}

		virtual void Detach(Observer* pObserver)
		{
			m_pListObserver->remove(pObserver);
		}

		virtual void Notify()
		{
			printf("\n[PS_FW::Subject]: Observer number = %d",m_pListObserver->size());
			std::list<Observer*>::iterator pos;
			for (pos = m_pListObserver->begin();pos != m_pListObserver->end();++pos)
			{
				(*pos)->Update(this);
			}
		}
	protected:
		Subject()
		{
			m_pListObserver = new std::list<Observer*>;
		}
	private:
		std::list<Observer*>* m_pListObserver;
	};
}	//PS_FW

#endif //__PS_FW_SUBJECT__
