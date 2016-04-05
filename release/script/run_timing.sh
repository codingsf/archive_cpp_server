#!/bin/sh
. /etc/profile
. ~/.bash_profile

PROCESS=`ps -u $USER | grep timing|grep -v run_timing|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = timing, pid = $i"
  kill -9 $i
done
echo "cd $HOME/code/output/bin"
cd $HOME/code/output/bin
echo "$HOME/code/output/bin/timing -r 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -j 192.168.1.88:8888 -l $HOME/code/output/log/timing.log &"
$HOME/code/output/bin/timing -r 6380 -d iF5LlcJOkCePiF5LlcJOkCeQfSgWpDAoT2gf -j 192.168.1.88:8888 -l $HOME/code/output/log/timing.log &