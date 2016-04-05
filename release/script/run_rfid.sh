#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep rfid|grep -v rfid_upload|grep -v run_rfid|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = rfid, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/rfid -p 22501 -r 6379 -q 1 -l $HOME/code/output/log/rfid.log &"
$HOME/code/output/bin/rfid -p 22501 -r 6379 -q 1 -l $HOME/code/output/log/rfid.log &