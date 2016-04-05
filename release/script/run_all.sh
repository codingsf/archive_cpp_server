#!/bin/sh
. /etc/profile
. ~/.bash_profile

$HOME/release/script/run_balance.sh
$HOME/release/script/run_task.sh
$HOME/release/script/run_upload.sh
$HOME/release/script/run_sending.sh
$HOME/release/script/run_timing.sh
$HOME/release/script/run_rfid.sh
$HOME/release/script/run_rfid_upload.sh