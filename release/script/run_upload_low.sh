#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -ef | grep upload_low |grep -v upload_jzt| grep -v run_upload|grep -v grep|grep -v PPID|awk '{ print $2}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_low, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_0 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_0.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_0 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_0.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_1 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_1.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_1 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_1.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_2 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_2.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_2 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_2.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_3 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_3.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_3 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_3.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_4 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_4.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_4 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_4.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_5 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_5.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_5 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_5.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_6 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_6.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_6 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_6.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_7 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_7.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_7 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_7.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_8 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_8.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_8 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_8.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_9 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_9.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_low_9 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_low_9.log &