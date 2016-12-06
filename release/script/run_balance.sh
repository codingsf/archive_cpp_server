#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep balance|grep -v run_balance|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = balance, pid = $i"
  kill -9 $i
done

echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/balance -h 21000 -p 22000 -r 6379 -k 0 -x 1 -y 1 -z 1 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l $HOME/code/output/log/balance.log &"
$HOME/code/output/bin/balance -h 21000 -p 22000 -r 6379 -k 0 -x 1 -y 1 -z 1 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -l $HOME/code/output/log/balance.log &