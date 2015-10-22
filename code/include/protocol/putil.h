/*
//
//	Last Modify Date: 2015-10-21
//	Author: zengpw
//	History:
//					2015-08-14 zengpw created
//	File Property: public
//	Future:
//					I. 协议工具
//
//
*/

#ifndef _PROTOCOL_PUTIL_H_
#define _PROTOCOL_PUTIL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include "common/log.h"

using namespace std;

namespace protocol
{
	unsigned short createCRC(const char *message, unsigned int len);
	string createKey();
};// namespace protocol

#endif