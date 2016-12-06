#!/bin/sh
. /etc/profile
. ~/.bash_profile

$HOME/release/script/run_upload_high.sh
$HOME/release/script/run_upload_mid.sh
$HOME/release/script/run_upload_low.sh
