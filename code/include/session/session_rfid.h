/*
//
//	Last Modify Date: 2016-02-23
//	Author: zengpw
//	History:
//					2016-02-17 zengpw created
//	File Property: public
//	Future:
//					I. Session子类，用于封装 RFID报文网络层操作
//
//
*/

#ifndef _SESSION_SESSION_RFID_H_
#define _SESSION_SESSION_RFID_H_

#include "common/thread.h"
#include "common/process.h"
#include "common/net.h"
#include "common/log.h"
#include "handle_basic.h"
#include "session.h"

using namespace std;

namespace session
{
	class SessionRFID : public Session
	{
	public:
		explicit SessionRFID(const unsigned int session_type, const char* ip, unsigned int port, common::Thread* th, HandleBasic* hb):Session(session_type, ip, port, th, hb){}
	public:
		void startNoThread();
	public:
		static void* listenerCb(void* self);
		static void OnReadCb(struct bufferevent *bev, void *conn);
	};
};// namespace session

#endif