/*
//
//	Last Modify Date: 2015-10-15
//	Author: zengpw
//	History:
//					2015-09-24 zengpw created
//	File Property: public
//	Future:
//					I. task 单元测试
//
//
*/

#ifndef _TASK_UT_HANDLE_H_
#define _TASK_UT_HANDLE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "common/log.h"
#include "protocol/putil.h"
#include "protocol/protocol_tlv.h"
#include "session/handle_basic.h"

using namespace std;

class TaskUtHandle : public session::HandleBasic
{
public:
	session::HANDLE_RTN conn(protocol::PacketOut* out);
	session::HANDLE_RTN cidDataGps_build(protocol::PacketOut* out);
	session::HANDLE_RTN cidDataGps(const protocol::PacketIn* in, protocol::PacketOut* out);
	session::HANDLE_RTN cidDataLbs_build(protocol::PacketOut* out);
	session::HANDLE_RTN cidDataLbs(const protocol::PacketIn* in, protocol::PacketOut* out);
};

#endif