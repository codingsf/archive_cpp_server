#ifndef __PS_FW_THREAD_POOL_DEAL__
#define __PS_FW_THREAD_POOL_DEAL__
////////////////////////////////////////////////////////////////////////////////////////
//
//	Creater : zengpw
//	Create Date : 2014��8��21��
//	Modify history :
//					2014��8��21�� zengpw add
//	File Property : private ����ͷ�ļ���˽�еģ�
//	Description :
//				I. �����̳߳�
//				II. �̳߳ع۲���Ϣ�����Ϸ����� put �¼����� put ����ʱ����֪ͨ�̳߳أ�
//					�̳߳��յ�֪ͨ�Ժ󣬻����Ϣ����ȡ����
//				III. Ҫʹ�̳߳ؾ��й۲��߹��ܣ�����Ҫ�̳��� EventObserver
//
////////////////////////////////////////////////////////////////////////////////////////

#include "thread_pool.h"
#include "msg_queue.h"
#include "subject.h"
#include "observer.h"

namespace PS_FW
{
	class ThreadPoolDeal : public ThreadPool , public Observer
	{
	public:
		ThreadPoolDeal::ThreadPoolDeal(int size,MsgQueue* pMsgQueue) : ThreadPool(size)
		{
			m_pMsgQueue = pMsgQueue;
			m_pMsgQueue->Attach(this);
		}

		ThreadPoolDeal::~ThreadPoolDeal()
		{
			m_pMsgQueue->Detach(this);
		}

		virtual void ThreadPoolDeal::Update(Subject* pSubject)
		{
			if(m_pMsgQueue == pSubject)
			{
				//TODO
			}
		}
	private:
		MsgQueue* m_pMsgQueue;
	};
}	//PS_FW

#endif //__PS_FW_THREAD_POOL_DEAL__

