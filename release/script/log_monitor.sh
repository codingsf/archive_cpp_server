#!/bin/sh
. /etc/profile
. ~/.bash_profile

cd $HOME/code/output

#example 2016-06-1X 2X
YEAR=`date +%Y`
MONTH=`date +%m`
DAY=`date +%d`

TIME=`echo $DAY | cut -c1-1`

if [ "$TIME" = "0" ]
then
	if [ "$MONTH" = "01" ]
	then
		YEAR=`expr $YEAR - 1 `
		MONTH=12
	else
		MONTH=`expr $MONTH - 1 `
		MONTH="0"$MONTH
	fi

	TIME=3
else
	TIME=`expr $TIME - 1 `
fi

#example 2016-06-1X 2X
RECORD_TIME=$YEAR"-"$MONTH"-"$TIME
RECORD_FOLDER="log_"$RECORD_TIME"X"

#example 2016-06
if [ "$TIME" = "3" ]
then
	RECORD_TIME=$YEAR"-"$MONTH
	RECORD_FOLDER="log_"$RECORD_TIME
fi

mkdir $RECORD_FOLDER

#move log file
cd $HOME/code/output/log
mv *$RECORD_TIME* ../$RECORD_FOLDER
