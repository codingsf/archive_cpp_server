#! /usr/local/bin python3
import urllib.request
import json

def requestAmap(req):
	send = urllib.request.Request(req)
	try:
		receive = urllib.request.urlopen(send)
		origin = receive.read().decode("utf-8")
		receive.close()
		content = json.loads(origin)
		j_type = content["result"]["type"]
		print("py_http.py : " + j_type)
		#增加对返回值的判断，如果没有定位结果，则返回全0
		if j_type == "0":
			return ("000000000000", "000000000000")
		j_location = content["result"]["location"]
		print("py_http.py : " + j_location)
		L = j_location.split(',')
		T = (L[0], L[1])
		return T
	#网络不正常，则抛出错误给 C++再次处理
	except urllib.error.HTTPError as e:
		print(e.code)
		print(e.read().decode("utf8"))
		raise
	#如果出现不可控的异常，则让 C++处理
	except:
		print("unknown exception")
		raise

def requestAmapUseAddr(req):
	send = urllib.request.Request(req)
	try:
		receive = urllib.request.urlopen(send)
		origin = receive.read().decode("utf-8")
		receive.close()
		content = json.loads(origin)
		location = "".join(content["regeocode"]["addressComponent"]["province"])
		location = location[0:] + "".join(content["regeocode"]["addressComponent"]["city"])
		location = location[0:] + "".join(content["regeocode"]["addressComponent"]["district"])
		location = location[0:] + "".join(content["regeocode"]["addressComponent"]["streetNumber"]["street"])
		location = location[0:] + "".join(content["regeocode"]["addressComponent"]["streetNumber"]["number"])
		print("py_http.py : " + location)
		T = (location[0:],)
		return T
	#网络不正常，则抛出错误给 C++再次处理
	except urllib.error.HTTPError as e:
		print(e.code)
		print(e.read().decode("utf8"))
		raise
	#如果是其它的问题（比如查询无结果），则返回全0
	except:
		return ("",)

def notifyJava(req):
	send = urllib.request.Request(req)
	try:
		receive = urllib.request.urlopen(send)
		origin = receive.read().decode("utf-8")
		receive.close()
		content = json.loads(origin)
		result = content["code"]
		print("py_http.py : " + result)
		T = (result[0:],)
		return T
	#网络不正常，则抛出错误给 C++ ，以便 C++ 重新处理
	except urllib.error.HTTPError as e:
		print(e.code)
		print(e.read().decode("utf8"))
		raise
	#如果是其它的问题（比如查询无结果），则返回false
	except:
		return ("notifyJava error",)

def notifyJavaEnhance(req):
	send = urllib.request.Request(req)
	try:
		receive = urllib.request.urlopen(send)
		origin = receive.read().decode("utf-8")
		receive.close()
		content = json.loads(origin)
		result = content["results"]
		if result:
			results = "true"
		else:
			results = "false"
		code = content["code"]
		print("py_http.py : results = " + results)
		print("py_http.py : code = " + code)
		T = (results[0:], code[0:])
		return T
	#网络不正常，则抛出错误给 C++ ，以便 C++ 重新处理
	except urllib.error.HTTPError as e:
		print(e.code)
		print(e.read().decode("utf8"))
		raise
	#如果是其它的问题（比如查询无结果），则返回false
	except:
		return ("notifyJavaEnhance error", "-1")

# if __name__ == '__main__':
	# requestAmap("http://apilocate.amap.com/position?accesstype=0&imei=866104021101384&cdma=0&bts=460,0,6338,62755,-66&nearbts=460,0,6338,62755,-66|460,0,6338,62755,-66&output=json&key=fe0d26ade7f0dc72e2d2efa4d10fc259")
	# requestAmap("http://apilocate.amap.com/position?accesstype=0&imei=866104021101384&cdma=0&bts=460,0,25013,10,-83&output=json&key=fe0d26ade7f0dc72e2d2efa4d10fc259")
	# requestAmapUseAddr("http://restapi.amap.com/v3/geocode/regeo?location=116.355,39.9876&extensions=all&output=json&key=44e606a9c5a628e9a8bd42226249f84c")
	# notifyJava("http://192.168.1.88:8888/dplus/corp/linkman/sendMsg.jspa?deviceId=201506060228&msgType=1&msgValue=2&regionType=1")
	# notifyJava("http://222.73.208.89:80/dplus/corp/linkman/sendMsg.jspa?deviceId=201506060228&msgType=1&msgValue=2&regionType=1")
	# notifyJavaEnhance("http://192.168.1.88:8888/dplus/system/isInFence.jspa?deviceId=201506040020&lng=121602637000&lat=031204421900")