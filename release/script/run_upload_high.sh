#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -ef | grep upload_high |grep -v upload_jzt| grep -v run_upload|grep -v grep|grep -v PPID|awk '{ print $2}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_high, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_high -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_high.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_high -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_high.log &