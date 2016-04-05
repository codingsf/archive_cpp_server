/*
//
//	Last Modify Date: 2016-01-13
//	Author: zengpw
//	History:
//					2015-10-12 zengpw created
//	File Property: public
//	Future:
//					I. 调用 python 接口
//
//
*/

#include "py_call.h"

namespace common
{
	PyObject* PyCall::Fun_requestAmap = NULL;
	PyObject* PyCall::Fun_requestAmapUseAddr = NULL;
	PyObject* PyCall::Fun_notifyJava = NULL;
	PyObject* PyCall::Fun_notifyJavaEnhance = NULL;

	bool PyCall::Initialize()
	{
		Py_Initialize();

		if(Py_IsInitialized() == 0)
		{
			LOG_DEBUG("python : python initialize fail!");
			return false;
		}

		PyObject* file = NULL;
		PyObject* module = NULL;
		PyObject* dict = NULL;

		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append('./')");

		file = Py_BuildValue("s", "py_http");

		module = PyImport_Import(file);
		if(!module)
		{
			LOG_DEBUG("python : load python script fail (or python script is not a correct file)!");
			return false;
		}

		dict = PyModule_GetDict(module);
		if(!dict)
		{
			LOG_DEBUG("python : load all function fail!");
			return false;
		}

		Fun_requestAmap = PyDict_GetItemString(dict, "requestAmap");
		if(!Fun_requestAmap || !PyCallable_Check(Fun_requestAmap))
		{
			LOG_DEBUG("python : load function requestAmap fail!");
			return false;
		}

		Fun_requestAmapUseAddr = PyDict_GetItemString(dict, "requestAmapUseAddr");
		if(!Fun_requestAmapUseAddr || !PyCallable_Check(Fun_requestAmapUseAddr))
		{
			LOG_DEBUG("python : load function requestAmapUseAddr fail!");
			return false;
		}

		Fun_notifyJava = PyDict_GetItemString(dict, "notifyJava");
		if(!Fun_notifyJava || !PyCallable_Check(Fun_notifyJava))
		{
			LOG_DEBUG("python : load function notifyJava fail!");
			return false;
		}

		Fun_notifyJavaEnhance = PyDict_GetItemString(dict, "notifyJavaEnhance");
		if(!Fun_notifyJavaEnhance || !PyCallable_Check(Fun_notifyJavaEnhance))
		{
			LOG_DEBUG("python : load function notifyJavaEnhance fail!");
			return false;
		}

		return true;
	}


	void PyCall::Finalize()
	{
		Py_Finalize();
	}


	bool PyCall::py_requestAmap(string& request, string& latitude, string& longitude)
	{
		PyObject* arg = NULL;
		PyObject* tp = NULL;
		PyObject* rtn = NULL;

		if(Fun_requestAmap == NULL)
		{
			LOG_DEBUG("python : function Fun_requestAmap is NULL!");
			return false;
		}

		arg = Py_BuildValue("s", request.c_str());

		tp = PyTuple_New(1);
		PyTuple_SetItem(tp, 0, arg);

		rtn = PyEval_CallObject(Fun_requestAmap, tp);
		if(!rtn)
		{
			LOG_DEBUG("python : call function requestAmap fail! Maybe net occur error.");
			return false;
		}

		const char* lati;
		const char* longi;

		if(!PyArg_ParseTuple(rtn, "ss", &longi, &lati))
		{
			LOG_DEBUG("python : parse function requestAmap return value fail!");
			return false;
		}
		else
		{
			latitude = lati;
			longitude = longi;
			return true;
		}

		return true;
	}


	bool PyCall::py_requestAmapUseAddr(string& request, string& address)
	{
		PyObject* arg = NULL;
		PyObject* tp = NULL;
		PyObject* rtn = NULL;

		if(Fun_requestAmapUseAddr == NULL)
		{
			LOG_DEBUG("python : function Fun_requestAmapUseAddr is NULL!");
			return false;
		}

		arg = Py_BuildValue("s", request.c_str());

		tp = PyTuple_New(1);
		PyTuple_SetItem(tp, 0, arg);

		rtn = PyEval_CallObject(Fun_requestAmapUseAddr, tp);
		if(!rtn)
		{
			LOG_DEBUG("python : call function requestAmapUseAddr fail! Maybe net occur error.");
			return false;
		}

		const char* addr;

		if(!PyArg_ParseTuple(rtn, "s", &addr))
		{
			LOG_DEBUG("python : parse function requestAmapUseAddr return value fail!");
			return false;
		}
		else
		{
			address = addr;
			return true;
		}

		return true;
	}


	bool PyCall::py_notifyJava(string& request, string& result)
	{
		PyObject* arg = NULL;
		PyObject* tp = NULL;
		PyObject* rtn = NULL;

		if(Fun_notifyJava == NULL)
		{
			LOG_DEBUG("python : function Fun_notifyJava is NULL!");
			return false;
		}

		arg = Py_BuildValue("s", request.c_str());

		tp = PyTuple_New(1);
		PyTuple_SetItem(tp, 0, arg);

		rtn = PyEval_CallObject(Fun_notifyJava, tp);
		if(!rtn)
		{
			LOG_DEBUG("python : call function notifyJava fail! Maybe net occur error.");
			return false;
		}

		const char* rt;

		if(!PyArg_ParseTuple(rtn, "s", &rt))
		{
			LOG_DEBUG("python : parse function notifyJava return value fail!");
			return false;
		}
		else
		{
			result = rt;
			return true;
		}

		return true;
	}


	bool PyCall::py_notifyJavaEnhance(string& request, string& result, string& code)
	{
		PyObject* arg = NULL;
		PyObject* tp = NULL;
		PyObject* rtn = NULL;

		code = "-1";

		if(Fun_notifyJavaEnhance == NULL)
		{
			LOG_DEBUG("python : function Fun_notifyJavaEnhance is NULL!");
			return false;
		}

		arg = Py_BuildValue("s", request.c_str());

		tp = PyTuple_New(1);
		PyTuple_SetItem(tp, 0, arg);

		rtn = PyEval_CallObject(Fun_notifyJavaEnhance, tp);
		if(!rtn)
		{
			LOG_DEBUG("python : call function notifyJavaEnhance fail! Maybe net occur error.");
			return false;
		}

		const char* rt;
		const char* cd;

		if(!PyArg_ParseTuple(rtn, "ss", &rt, &cd))
		{
			LOG_DEBUG("python : parse function notifyJavaEnhance return value fail!");
			return false;
		}
		else
		{
			result = rt;
			code = cd;
			return true;
		}

		return true;
	}
};// namespace common