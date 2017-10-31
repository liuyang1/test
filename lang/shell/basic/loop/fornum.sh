#! /usr/bin/env bash
for i in 1 2; do
    echo $i
done

# bash 3.0+
for i in {1..3}; do
    echo $i
done

# bash 4.0+
for i in {1..5..2}; do
    echo $i
done

echo "use seq command"
for i in $(seq 1 2 5); do
    echo $i
done

echo "fix width (zero pading)"
for i in $(seq -w 0 5 10); do
    echo $i
done

echo "arith in bash"
for (( i=1; i<=5; i+=2 )); do
    echo $i
done

echo "infinite loops"
for (( ; ; )); do
    echo "infinite loops"
    sleep 1
done
