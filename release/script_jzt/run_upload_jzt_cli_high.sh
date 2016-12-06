#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep upload_jzt_high|grep -v run_upload_jzt|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_jzt_high, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_high -l $HOME/code/output/log/upload_jzt_high.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_high -l $HOME/code/output/log/upload_jzt_high.log &