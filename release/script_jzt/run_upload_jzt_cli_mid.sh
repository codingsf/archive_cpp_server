#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep upload_jzt_mid|grep -v run_upload_jzt|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_jzt_mid, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_0 -l $HOME/code/output/log/upload_jzt_mid_0.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_0 -l $HOME/code/output/log/upload_jzt_mid_0.log &
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_1 -l $HOME/code/output/log/upload_jzt_mid_1.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_1 -l $HOME/code/output/log/upload_jzt_mid_1.log &
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_2 -l $HOME/code/output/log/upload_jzt_mid_2.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_2 -l $HOME/code/output/log/upload_jzt_mid_2.log &
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_3 -l $HOME/code/output/log/upload_jzt_mid_3.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_3 -l $HOME/code/output/log/upload_jzt_mid_3.log &
echo "$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_4 -l $HOME/code/output/log/upload_jzt_mid_4.log &"
$HOME/code/output/bin/upload_jzt -m 1 -r 6381 -p 219.139.241.228 -j 8887 -q msg_upload_mid_4 -l $HOME/code/output/log/upload_jzt_mid_4.log &