#!/bin/sh
PROCESS=`ps -u $USER | grep task_jzt|grep -v run_task_jzt|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = task_jzt, pid = $i"
  kill -9 $i
done

PROCESS=`ps -u $USER | grep upload_jzt | grep -v run_upload_jzt|grep -v grep|grep -v PPID|awk '{ print $1}'`
for i in $PROCESS
do
  echo "Kill process : name = upload_jzt, pid = $i"
  kill -9 $i
done