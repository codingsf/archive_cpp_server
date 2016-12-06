#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -ef | grep upload_mid |grep -v upload_jzt| grep -v run_upload|grep -v grep|grep -v PPID|awk '{ print $2}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_mid, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_0 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_0.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_0 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_0.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_1 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_1.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_1 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_1.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_2 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_2.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_2 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_2.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_3 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_3.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_3 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_3.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_4 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_4.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_4 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_4.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_5 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_5.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_5 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_5.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_6 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_6.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_6 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_6.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_7 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_7.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_7 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_7.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_8 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_8.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_8 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_8.log &
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_9 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_9.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload_mid_9 -d iF5LlcJOkB51lFv+kVI9f1WAgTXoT2gf -j 192.168.1.88:80 -l $HOME/code/output/log/upload_mid_9.log &