#! /bin/bash
echo >> version
echo "----------------------------------------------------------------------" >> version
echo >> version
date >> version
echo $1 >> version
echo "" >> version
echo "" >> version
vim version +
