/*
//
//	Last Modify Date: 2016-02-23
//	Author: zengpw
//	History:
//					2016-02-17 zengpw created
//	File Property: public
//	Future:
//					I. Session子类，用于封装 RFID报文操作
//
//
*/

#include "session_rfid.h"

namespace session
{
	void SessionRFID::startNoThread()
	{
		if(session_type == SESSION_TYPE_LISTENER)
			listenerCb((void*)this);
	}

	void* SessionRFID::listenerCb(void* self)
	{
		SessionRFID* se = (SessionRFID*)self;
		common::Conn* conn = new common::Conn;
		conn->init(common::CONN_TYPE_LISTENER, NULL, se->port);
		conn->setReadCb(se->OnReadCb);
		conn->extra = se->hb;
		conn->run();
		//TODO: 调用 pthread_exit ，返回线程退出码；

		return NULL;
	}

	void SessionRFID::OnReadCb(struct bufferevent *bev, void *conn)
	{
		common::Conn* cn = (common::Conn*)conn;
		HandleBasic* hb = (HandleBasic*)cn->extra;

		for(;;)
		{
			protocol::PacketRFID *in = new protocol::PacketRFID;

			int len = sizeof(protocol::rfid::HeadRfid);

			if(cn->recvMsg(bev, (void*)in->setHead(), len) == 0)
			{
				LOG_DEBUG("[hint] new message coming");

				len = in->parseHead();

				if(len > 0)
				{
					if(cn->recvMsg(bev, (void*)in->setBody(), len) == 0)
					{
						if(!in->parseBody())
						{
							try
							{
								//解析上行消息
								int rtn = hb->dispatch_rfid(in);

								if(rtn == HANDLE_RTN_SEND)
								{
									LOG_DEBUG("SessionRFID -> OnReadCb() -> dispatch() send message!");
								}

								if(rtn == HANDLE_RTN_CONTINUE)
								{
									// LOG_DEBUG 是有2行代码的宏，所以一定要写在花括号里
									LOG_DEBUG("SessionRFID -> OnReadCb() -> dispatch() return continue!");
								}

								if(rtn == HANDLE_RTN_STOP)
									cn->stop();
							}
							catch(...)
							{
								LOG_DEBUG("SessionRFID -> OnReadCb() -> dispatch() throw exception!");
								delete in;
								return;
							}
						}
						else
						{
							LOG_DEBUG("rfid parse message fail!");
							delete in;
							return;
						}
					}
					else
					{
						delete in;
						return;
					}
				}
				else
				{
					delete in;
					return;
				}
			}
			else
			{
				delete in;
				return;
			}

			delete in;
		}

		return;
	}
};// namespace session
