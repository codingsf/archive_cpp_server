/*
//
//	Last Modify Date: 2016-07-28
//	Author: zengpw
//	History:
//					2015-08-12 zengpw created
//	File Property: public
//	Future:
//					I. 部分工具
//
//
*/

#include "util.h"

namespace common
{
	//字符串拆分
	void split(const string& in, const string delimiter, StringSet& out)
	{
		string::size_type cur = 0;
		string::size_type next = 0;

		next = in.find(delimiter, cur);

		while(next != std::string::npos)
		{
			out.insert(in.substr(cur, next - cur));
			cur = next + 1;
			next = in.find(delimiter, cur);
		}

		out.insert(in.substr(cur));
	}

	// 16进制打印
	char hex2char(int hex)
	{
		char c_res = ' ';
		// 0~9转换成对应字符,转为'0'~'9'
		if(hex >= 0 && hex <= 9)
		{
			c_res = char(hex + '0');
		}
		// 10~15十六进制为a~f,转换为'a'~'f'
		else if(hex >= 10 && hex <= 15)
		{
			c_res = char(hex - 10 + 'A');
		}

		return c_res;
	}

	string printByHex(const char* message, const int length)
	{
		string out;

		for(int i = 0; i < length; i++)
		{
			int k = (unsigned char)*(message + i)/ 16;
			out += hex2char(k);
			k = (unsigned char)*(message + i)% 16;
			out += hex2char(k);
			out += " ";
		}

		// LOG_DEBUG(out);

		return out;
	}

	string printByHex(const unsigned char* message, const int length)
	{
		string out;

		for(int i = 0; i < length; i++)
		{
			int k = (unsigned char)*(message + i)/ 16;
			out += hex2char(k);
			k = (unsigned char)*(message + i)% 16;
			out += hex2char(k);
			out += " ";
		}

		// LOG_DEBUG(out);

		return out;
	}

	int hexStr2int(const unsigned char* message, const int length)
	{
		int out = 0;

		for(int i = 0; i < length; i++)
		{
			//每循环一次，原值进位 + 1
			out = out*16;

			//原值转换为十进制
			unsigned char k = *(message + i);

			if(k >= '0' && k <= '9')
			{
				out += int(k - '0');
			}

			if(k >= 'a' && k <= 'f')
			{
				out += int(k - 'a' + 10);
			}

			if(k >= 'A' && k <= 'F')
			{
				out += int(k - 'A' + 10);
			}
		}

		return out;
	}

	//进制转换
	string int2binary(const int i, const int reverse)
	{
		std::bitset<std::numeric_limits<int>::digits> bs(i);
		std::string str;

		if(reverse)
		{
			/* cut data on first character '1' */
			str = bs.to_string().substr(bs.to_string().find('1'));
			std::replace(str.begin(),str.end(),'0','X');
			std::replace(str.begin(),str.end(),'1','0');
			std::replace(str.begin(),str.end(),'X','1');
			str = str.substr(str.find('1'));
		}
		else
		{
			/* cut data on first character '1' */
			str = bs.to_string().substr(bs.to_string().find('1'));
		}

		return str;
	}

	string uchar2binary(const unsigned char i)
	{
		std::bitset<std::numeric_limits<unsigned char>::digits> bs(i);
		std::string str;
		str = bs.to_string();

		return str;
	}

	int binary2int(const string& str)
	{
		std::bitset<std::numeric_limits<int>::digits> bs(str);

		return (int)bs.to_ulong();
	}

	//生成当前时间
	string buildDateTime()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);

		struct tm* pst;
		pst = localtime(&tv.tv_sec);

		string sdt;

		char buf[36];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%04d%02d%02d%02d%02d%02d", pst->tm_year + 1900, pst->tm_mon + 1, pst->tm_mday, pst->tm_hour, pst->tm_min, pst->tm_sec);

		stringstream ss;

		ss << buf;
		sdt = ss.str();
		ss.str("");
		ss.clear();

		return sdt;
	}

	//生成当前时间（秒数）
	long long buildDateTimeSecond()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);

		return tv.tv_sec;
	}

	//时间字符串处理
	bool dateTimeFilter(string& time14)
	{
		if(time14.find("-") != std::string::npos)
			return true;

		if(time14.substr(0, 3) != "201")
			return true;

		return false;
	}

	//取模
	string getMod(const string& str, const int mod)
	{
		int i;
		stringstream ss;

		string sub = str.substr(str.length() - 1);

		ss << sub;
		ss >> i;
		ss.str("");
		ss.clear();

		i = i%mod;
		ss << i;
		sub = ss.str();
		ss.str("");
		ss.clear();

		return sub;
	}

	char* mix(const unsigned char* origin, int len, char* base64)
	{
		int i;
		int j;
		unsigned char c;

		// const char* index = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		const char* index = "WXYZabcderstfgnopqRSTUVhijklmvwxyz012JKLMN89+/OPQABCDEFGHIu34567";

		for (i = 0, j = 0; i < len; i += 3)
		{
		    c = (origin[i] >> 2);
		    c &= (unsigned char)0x3F;
		    base64[j++] = index[(int)c];

		    c = ((unsigned char)(origin[i] << 4)) & ((unsigned char)0x30);
		    if (i + 1 >= len)
		    {
		        base64[j++] = index[(int)c];
		        base64[j++] = '=';
		        base64[j++] = '=';
		        break;
		    }
		    c |= ((unsigned char)(origin[i+1] >> 4)) & ((unsigned char) 0x0F);
		    base64[j++] = index[(int)c];

		    c = ((unsigned char)(origin[i+1] << 2)) & ((unsigned char)0x3C);
		    if (i + 2 >= len)
		    {
		        base64[j++] = index[(int)c];
		        base64[j++] = '=';
		        break;
		    }
		    c |= ((unsigned char)(origin[i+2] >> 6)) & ((unsigned char) 0x03);
		    base64[j++] = index[(int)c];

		    c = ((unsigned char)origin[i+2]) & ((unsigned char)0x3F);
		    base64[j++] = index[(int)c];
		}

		base64[j] = '\0';

		return base64;
	}

	string mix(const string& origin)
	{
		stringstream ss;
		char base64[256] = {'\0'};

		common::mix((unsigned char*)origin.c_str(), origin.length(), base64);
		ss << base64;

		return ss.str();
	}

	unsigned char* unmix(const char* base64, unsigned char* origin)
	{
		int i;
		int j;
		unsigned char k;
		unsigned char b[4];

		// const char* index = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		const char* index = "WXYZabcderstfgnopqRSTUVhijklmvwxyz012JKLMN89+/OPQABCDEFGHIu34567";

		for (i = 0, j = 0; base64[i] != '\0'; i += 4)
		{
		    memset(b, 0xFF, sizeof(b));
		    for (k = 0; k < 64; k++)
		    {
		        if (index[k] == base64[i])
		            b[0]= k;
		    }
		    for (k = 0; k < 64; k++)
		    {
		        if (index[k] == base64[i+1])
		            b[1]= k;
		    }
		    for (k = 0; k < 64; k++)
		    {
		        if (index[k] == base64[i+2])
		            b[2]= k;
		    }
		    for (k = 0; k < 64; k++)
		    {
		        if (index[k] == base64[i+3])
		            b[3]= k;
		    }

		    origin[j++] = ((unsigned char)(((unsigned char)(b[0] << 2))&0xFC)) | ((unsigned char)((unsigned char)(b[1]>>4)&0x03));

		    if ( base64[i+2] == '=' )
		        break;

		    origin[j++] = ((unsigned char)(((unsigned char)(b[1] << 4))&0xF0)) | ((unsigned char)((unsigned char)(b[2]>>2)&0x0F));

		    if ( base64[i+3] == '=' )
		        break;

		    origin[j++] = ((unsigned char)(((unsigned char)(b[2] << 6))&0xF0)) | ((unsigned char)(b[3]&0x3F));
		}

		return origin;
	}

	string unmix(const string& base64)
	{
		stringstream ss;
		unsigned char origin[256] = {'\0'};

		common::unmix(base64.c_str(), origin);
		ss << origin;

		return ss.str();
	}
};// namespace common
