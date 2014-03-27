pure_server<br />
程序说明：用C++实现的 server 库，目的是10万并发^_^，运行在 linux 平台，会自带一个示例程序<br />
程序版本： 1.1<br />
最后修改：2014年3月27日 19:22:39<br />
author：zengpw<br />
mail：zengpw@foxmail.com;    zengpw1226@gmail.com<br />

已经实现的功能：<br />
Ⅰ.基于TCP IPV4协议的socket通信；<br />
Ⅱ.采用select的服务端；<br />
Ⅲ.采用stl list的任务队列；<br />
Ⅳ.固定大小的线程池；<br />

未来功能展望：<br />
Ⅰ.采用epoll的服务端；<br />
Ⅱ.动态线程池；<br />
Ⅲ.对XML支持（即是支持对XML序列化及反序列化）；<br />
Ⅳ.将接口提供到ps_api这一层<br />

进度的规划：<br />
Ⅰ.采用的是季度规划，即以3个月为一个开发周期；<br />

依赖的第三方软件：<br />
Ⅰ. 编译采用 cmake ，版本要求不低于 2.6<br />
Ⅱ. 由于采用了epoll这个系统调用，所以要求linux内核版本高于2.6.19 <br />
Ⅲ. 采用第3方代码 tinyxml2 来提供XML序列化底层支持（源码位于 ./ps_3rd 目录中）<br />

