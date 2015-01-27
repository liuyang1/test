func() {
    echo "this is func"
}
if type -t func > /dev/null; then
    eval "func"
else
    echo "not func"
fi

if declare -f "func" > /dev/null; then
    echo "declare func"
fi
