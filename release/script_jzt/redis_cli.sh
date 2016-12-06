#!/bin/bash
echo -e "\n redis-server process status: \n"
ps -ef | grep redis-server | grep -v grep
echo -e "\n choose redis:"
echo -e "\n\t 1) cache (redis-cli -h 127.0.0.1 -p 6381)"
echo -e "\n\t 2) send (redis-cli -h 127.0.0.1 -p 6380)"
echo -e "\n\t 3) dest_local (redis-cli -h 127.0.0.1 -p 6382)"
echo -e "\n\t 4) dest_jzt (redis-cli -h 219.139.241.228 -p 8887) \n"
read -p " input 1/2/3/4 :  " need noneed
echo -e "\n\t start $need"

if [ $need == "1" ]; then
	cd $HOME/release/redis
	echo -e "\n\t redis-cli -h 127.0.0.1 -p 6381 \n"
	redis-cli -h 127.0.0.1 -p 6381
fi

if [ $need == "2" ]; then
	cd $HOME/release/redis
	echo -e "\n\t redis-cli -h 127.0.0.1 -p 6380 \n"
	redis-cli -h 127.0.0.1 -p 6380
fi

if [ $need == "3" ]; then
	cd $HOME/release/redis
	echo -e "\n\t redis-cli -h 127.0.0.1 -p 6382 \n"
	redis-cli -h 127.0.0.1 -p 6382
fi

if [ $need == "4" ]; then
	cd $HOME/release/redis
	echo -e "\n\t redis-cli -h 219.139.241.228 -p 8887 \n"
	redis-cli -h 219.139.241.228 -p 8887
fi