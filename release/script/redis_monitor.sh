#!/bin/bash
. /etc/profile
. ~/.bash_profile

PROCESS_NUM=`ps -u $USER | grep redis-server | grep -v grep | grep -v PPID | wc -l`

if [ "$PROCESS_NUM" != "2" ]
then
	$HOME/release/script/redis_sending.sh
	$HOME/release/script/redis_upload.sh
fi