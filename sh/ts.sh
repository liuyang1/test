#! /usr/bin/env bash
HOST="210.75.225.77"
USER="dsp"
PASS="portalpic"
LCD="/var/www/html"

lftp -c "set ftp:list-options -a;		
		open ftp://$USER:$PASS@$HOST;
		lcd $LCD;
		mput *.ts"
