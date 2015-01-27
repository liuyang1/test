#! /usr/bin/env bash
test_func0() {
    echo "func0"
}

test_func1() {
    echo "func1"
}

func=func0
eval "test_$func"

echo "declare style"
if declare -f "test_$func" > /dev/null ; then
    echo "yes, it's a func."
else
    echo "not, it isn't a func."
fi

echo "type style"
if type -t "test_$func" > /dev/null ; then
    echo "yes, it's a func."
else
    echo "not, it isn't a func."
fi
