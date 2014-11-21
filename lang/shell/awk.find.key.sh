function extract()
{
    filename=$1
    key=$2
    awk -v KEY="$key" '{if ($2==KEY) {print $3}}' $filename | awk -F "[()]" '{print $2}'
}

RET=$(extract ~/test.h START)
echo $RET
