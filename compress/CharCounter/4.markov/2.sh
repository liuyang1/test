# for num in $(seq 0 256 7936); do
# for num in $(seq 0 256 1792); do
# for num in $(seq 2048 256 3840); do
# for num in $(seq 4096 256 5888); do
for num in $(seq 6144 256 7936); do
    # echo $num $((num + 255))
    bash 1.sh $num $((num + 255)) &
done

wait
