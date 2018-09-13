string="hello.world"
# string="a"
readarray -td. arr <<< "$string.";declare -p arr;
echo $arr

printf '%s\n' "${arr[@]}"

printf '%s\n' "${arr[*]}"

printf '%s\n' "${arr[2]}"
printf '%s\n' "${arr[1]}"
printf '%s\n' "${arr[0]}"
