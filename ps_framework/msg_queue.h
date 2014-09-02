#ifndef __PS_FW_MSG_QUEUE__
#define __PS_FW_MSG_QUEUE__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw �������
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description :
//				I. ��Ϣ����ͷ�ļ�
//				II.  ���ն����� IO �߳� put �������߳� get �����Ͷ���ֻ�� �����߳� put
//				III. Ŀǰ���ǲ��� v1.3 ��� std::list ���Ժ����� std::queue �����ܻ�������
//				IV. ��Ϣ���л�����֪ͨ�䴦���̣߳��������Ҫ�̳��� Event
//
////////////////////////////////////////////////////////////////////////////////////////

#include "sys_include.h"
#include "ps_fw_define.h"
#include "subject.h"

namespace PS_FW
{
	typedef std::list<void*> MsgList;

	class MsgQueue : public Subject
	{
	public:
		MsgQueue();
		virtual ~MsgQueue(){}
	public:
		int put(void* msg);
		int get(void** msg);
		int destory();
	private:
		MsgList* m_queue;
		pthread_mutex_t m_mxLock;
	};
}	//PS_FW

#endif //__PS_FW_MSG_QUEUE__
