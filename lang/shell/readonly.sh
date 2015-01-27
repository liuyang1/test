readonly a="A"
echo "a=$a is readonly var, cannot re-assignment"
a="B"

readonly array=("a" "b")
echo "array=${array[*]} is readonly array, cannot re-assignment"
array=("1" "2")

echo "WARN: but script can continue to run"
