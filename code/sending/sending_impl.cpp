/*
//
//	Last Modify Date: 2016-01-13
//	Author: zengpw
//	History:
//					2015-10-28 zengpw created
//	File Property: private
//	Future:
//					I. 实现 ICE 的接口
//
//
*/

#include "sending_impl.h"

void SendingImpl::setRedis(common::Redis* rc)
{
	this->rc = rc;
}

::sending::server::Rtn SendingImpl::buildRtn(int e, string str)
{
	::sending::server::Rtn r;

	r.e = e;
	r.str = str;

	LOG_DEBUG("build : r.e = " << r.e << ", r.str = " << r.str);

	return r;
}

::sending::server::Rtn SendingImpl::push(const ::sending::server::Command& cmd, const ::Ice::Current& c)
{
	session::RedisPublic rp;

	LOG_DEBUG("receive : cmd.id = " << cmd.id << ", cmd.type = " << cmd.type);

	string hint = "no hint";
	int rtn = rp.public_insert_command(rc, cmd.id, cmd.type, "2592000", hint);

	return buildRtn(rtn, hint);
}