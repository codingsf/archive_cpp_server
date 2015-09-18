#!/bin/bash
echo -e "\n redis-server process status: \n"
ps -ef | grep redis-server | grep -v grep
echo -e "\n choose redis:"
echo -e "\n\t 1) redis-cli -h 127.0.0.1 -p 6379"
echo -e "\n\t 2) redis-cli -h 127.0.0.1 -p 6380"
echo -e "\n\t 3) redis-cli -h 127.0.0.1 -p 6381 \n"
read -p " input 1/2/3 :  " need noneed
echo -e "\n\t start $need"

if [ $need == "1" ]; then
	cd ../redis/
	echo -e "\n\t redis-cli -h 127.0.0.1 -p 6379 \n"
	redis-cli -h 127.0.0.1 -p 6379
fi

if [ $need == "2" ]; then
	cd ../redis/
	echo -e "\n\t redis-cli -h 127.0.0.1 -p 6380 \n"
	redis-cli -h 127.0.0.1 -p 6380
fi

if [ $need == "3" ]; then
	cd ../redis/
	echo -e "\n\t redis-cli -h 127.0.0.1 -p 6381 \n"
	redis-cli -h 127.0.0.1 -p 6381
fi