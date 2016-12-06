#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep task|grep -v task_jzt|grep -v run_task|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = task, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/task -a 220.248.44.218 -h 21000 -i 30 -p 22001 -r 6379 -k 0 -x 1 -y 1 -z 1 -s 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l $HOME/code/output/log/task.log &"
$HOME/code/output/bin/task -a 220.248.44.218 -h 21000 -i 30 -p 22001 -r 6379 -k 0 -x 1 -y 1 -z 1 -s 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l $HOME/code/output/log/task.log &