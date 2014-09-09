#ifndef __PS_FW_MESSAGE_POOL__
#define __PS_FW_MESSAGE_POOL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//					2014��9��9�� zengpw �������
//	File Property : private
//	Description :
//				I. ��Ϣ����ͷ�ļ�
//				III. Ŀǰ���ǲ��� v1.3 ��� std::list ���Ժ����� std::queue �����ܻ�������
//				IV. ��Ϣ���л�����֪ͨ�䴦���̣߳��������Ҫ�̳��� subject
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "subject.h"

namespace PS_FW
{
	typedef std::list<void*> Pool;

	class MessagePool : public Subject
	{
	public:
		MessagePool();
		virtual ~MessagePool();
	public:
		int pushByChar(char* szMessage);
		int popByChar(char* szMessage);
	private:
		int push(void* msg);
		int pop(void** msg);
	private:
		Pool* m_pPool;
		//��Ϣ���ڲ���
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_MESSAGE_POOL__
