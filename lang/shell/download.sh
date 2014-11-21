#!/bin/bash
#  下载图片视频的脚本

year=2011
YEAR=$year

for(( month=1;month<=12;month++ ))
do
    MONTH=`printf "%02d" $month`
    mkdir ./$MONTH

    for(( ind1=1;ind1<=150;ind1++ ))
    do
        IND1=`printf "%03d" $ind1`
        mkdir ./$MONTH/$IND1

        for(( ind2=1;ind2<=40;ind2++ ))
        do
            IND2=`printf "%02d" $ind2`
            url=http://tu1.mm8mm8.com/mm8/tupai/$YEAR$MONTH/$IND1/$ind2.jpg
            wget -T 5 -t 3 $url -O ./$MONTH/$IND1/$IND2.jpg
            res=$?
            COUNT=$IND2
            if [ $res -eq 8 ]
            then
                break
            fi
        done
        rm ./$MONTH/$IND1/$COUNT.jpg
    done
    for file in ./$MONTH/*/
    do
        rmdir $file
    done
done

for file in ./*/
do
    rmdir $file
done
