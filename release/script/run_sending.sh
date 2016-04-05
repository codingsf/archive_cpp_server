#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep sending|grep -v run_sending|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = sending, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/sending -r 6380 -p 7777 -l $HOME/code/output/log/sending.log &"
$HOME/code/output/bin/sending -r 6380 -p 7777 -l $HOME/code/output/log/sending.log &