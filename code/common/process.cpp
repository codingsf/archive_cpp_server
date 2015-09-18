/*
//
//	Last Modify Date: 2015-09-14
//	Author: zengpw
//	History:
//					2015-08-27 zengpw created
//	File Property: public
//	Future:
//					I. 进程
//
//
*/

#include "process.h"

namespace common
{
	Config::Config(int argc, char* argv[], string key)
	{
		mapConfig.clear();

		StringSet SetString;
		common::split(key, ":", SetString);

		int opt;
		while((opt = getopt(argc, argv, key.c_str())) != -1)
		{
			char c[2] = {(char)opt, '\0'};
			string s(c, 1);
			StringSet::iterator iter = SetString.find(s);

			if(iter != SetString.end())
			{
				mapConfig[*iter] = string(optarg);
			}
		}

		for(iterConfig = mapConfig.begin(); iterConfig != mapConfig.end(); ++iterConfig)
		{
			printf("\n execute arg : %s = %s \n", iterConfig->first.c_str(), iterConfig->second.c_str());
		}
	}


	bool Config::exist(char argv)
	{
		char c[2] = {argv, '\0'};
		if(mapConfig.find(c) != mapConfig.end())
			return true;
		else
			return false;
	}


	string Config::get(char argv)
	{
		char c[2] = {argv, '\0'};
		return mapConfig.find(c)->second;
	}
};// namespace common
