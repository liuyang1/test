# ./stat ../data/enwik8 0
beg=$1
end=$2
echo > markov.4.$beg.txt
for num in $(seq "$beg" "$end"); do
    echo "$num / $beg-$end"
    ./stat enwik8 $num >> markov.4.$beg.txt
done
