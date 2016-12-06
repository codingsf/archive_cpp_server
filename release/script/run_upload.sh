#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep upload|grep -v upload_jzt|grep -v run_upload|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = upload, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/upload -r 6379 -q msg_upload -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -j 192.168.1.88:8888 -l $HOME/code/output/log/upload.log &"
$HOME/code/output/bin/upload -r 6379 -q msg_upload -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -j 192.168.1.88:8888 -l $HOME/code/output/log/upload.log &