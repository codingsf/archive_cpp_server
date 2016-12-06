#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep upload_jzt|grep -v run_upload_jzt|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_jzt, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 127.0.0.1 -j 6382 -q msg_upload -l $HOME/code/output/log/upload_jzt.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 127.0.0.1 -j 6382 -q msg_upload -l $HOME/code/output/log/upload_jzt.log &