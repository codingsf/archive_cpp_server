#ifndef __PS_FW_TASK__
#define __PS_FW_TASK__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014-8-21
//	Modify history :
//					2014-9-18 zengpw �������
//	File Property : private
//	Description :
//					I. ��Ϣ����ͷ�ļ�
//					II. ���� queue ������
//					III. ���ṩ�Ľӿ��У�task �ǻ����������ͣ���Ϊ�ڲ�Ҫ֧��free()������
//	Future :
//					I. �ṩ˫���У�����������
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include "_include.h"

namespace PS_FW
{
	class Task
	{
	public:
		Task();
		virtual ~Task();
	public:
		int push(void* task);
		int pop(void** task);
	private:
		std::queue<void*>* m_queue;
		pthread_mutex_t m_queueLock;
	};
}	//PS_FW

#endif //__PS_FW_TASK__
