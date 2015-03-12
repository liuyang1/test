date0=$(date +"%s")
sleep 4
date1=$(date +"%s")
rddate=$(($date1-$date0))
if [[ "$rddate" -gt 3 ]]; then
    ddate=$(date -u -d @"$rddate" +'%-Mm %-Ss')
else
    ddate="$rddate"s
fi
echo "$ddate"
