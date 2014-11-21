LOGFILE=log

for(( n=100;n<=1000;n+=100))
do
    echo $n
    echo $n >> $LOGFILE
    /usr/bin/time -ao $LOGFILE -p python hello.py $n
    echo >> $LOGFILE
done
