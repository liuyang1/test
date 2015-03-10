#! /usr/bin/env bash
declare -A newmap
# name="liuyang"
newmap[name]="good"
newmap[gene]="better"

echo ${newmap[name]}
echo "with quote  " ${newmap["name"]}
echo "single quote" ${newmap['name']}

key="name"
echo "dynamic key:" ${newmap[$key]}

echo "missing->   " ${newmap[missing]}

if [ ${newmap[missing]+_} ]; then
    echo "found"
else
    echo "not found"
fi

echo "loop on key"
for K in "${!newmap[@]}"; do
    echo $K
done

echo "loop on key-value"
for K in "${!newmap[@]}"; do
    echo "$K -- ${newmap[$K]}"
done

echo "size: ${#newmap[@]}"
echo "unset key to clean key-value pair"

echo "indexing"
KEYS=(${!newmap[@]})
for (( Idx = 0 ; Idx < ${#newmap[@]} ; Idx++ )); do
    K=${KEYS[$Idx]}
    echo "$Idx $K ${newmap[$K]}"
done

unset newmap[name]
echo "keys containting keys, must use double or single quote to delete it"

echo "unset to clear map"
unset newmap


declare -A onemap
onemap=( ["cow"]="moo" ["woof"]="dog")
echo "keys:     " "${!onemap[@]}"
echo "key->value" "${onemap["cow"]}"


echo "fake substring style"
fakeMap=("cow:moo" "bird:chirp")
for animal in "${fakeMap[@]}"; do
    key="${animal%%:*}"
    val="${animal#*:}"
    echo "$key is $val"
done

echo "meta hask style"
hput() {
    eval hashPre"$1"='$2'
}
hget() {
    eval echo '${hashPre'"$1"'#hashPre}'
}
hput France Paris
echo "France -> " `hget France`
