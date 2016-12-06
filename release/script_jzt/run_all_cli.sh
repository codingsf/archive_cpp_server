#!/bin/sh
. /etc/profile
. ~/.bash_profile

$HOME/release/script_jzt/run_task_jzt_cli.sh
$HOME/release/script_jzt/run_upload_jzt_cli_high.sh
$HOME/release/script_jzt/run_upload_jzt_cli_mid.sh
$HOME/release/script_jzt/run_upload_jzt_cli_low.sh