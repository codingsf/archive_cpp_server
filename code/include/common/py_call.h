/*
//
//	Last Modify Date: 2015-12-04
//	Author: zengpw
//	History:
//					2015-10-12 zengpw created
//	File Property: public
//	Future:
//					I. 调用 python 接口
//
//
*/

#ifndef _COMMON_PY_CALL_H_
#define _COMMON_PY_CALL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "log.h"
#include "util.h"
#include "Python.h"

using namespace std;

namespace common
{
	class PyCall
	{
	public:
		static PyObject* Fun_requestAmap;
		static PyObject* Fun_requestAmapUseAddr;
		static PyObject* Fun_notifyJava;
	public:
		static bool Initialize();
		static void Finalize();
	public:
		bool py_requestAmap(string& request, string& latitude, string& longitude);
		bool py_requestAmapUseAddr(string& request, string& address);
		bool py_notifyJava(string& request, string& result);
	};
};// namespace common

#endif
