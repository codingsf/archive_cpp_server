#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep task_jzt|grep -v run_task_jzt|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = task_jzt, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/task_jzt -m 1 -p 30000 -r 6381 -k 1 -x 1 -y 5 -z 5 -s 6380 -l $HOME/code/output/log/task_jzt.log &"
$HOME/code/output/bin/task_jzt -m 1 -p 30000 -r 6381 -k 1 -x 1 -y 5 -z 5 -s 6380 -l $HOME/code/output/log/task_jzt.log &