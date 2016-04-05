#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER |grep rfid_upload|grep -v run_rfid_upload|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = rfid_upload, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/rfid_upload -r 6379 -q rfid_upload -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l $HOME/code/output/log/rfid_upload.log &"
$HOME/code/output/bin/rfid_upload -r 6379 -q rfid_upload -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l $HOME/code/output/log/rfid_upload.log &