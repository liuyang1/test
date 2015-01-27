s=a_b_c_d
echo "entire string"
echo "$s"

echo "from head, delte shortest matched substr"
echo "${s#*_}"
echo "from head, delete longest matched substr"
echo "${s##*_}"

echo "from tail, delete shortest matched substr"
echo "${s%_*}"
echo "from tail, delete longest matched substr"
echo "${s%%_*}"
