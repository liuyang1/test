ag line ../baseline.txt | awk '{print $NF}' | awk '{s+=$1} END {print s}'
