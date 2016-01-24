#! /usr/bin/env bash
# Read from the file file.txt and output the tenth line to stdout.
awkSln() {
    awk '{if (NR=="10") {print $0}}' file.txt
}
awk2Sln() {
    awk 'NR==10' file.txt
}

shSln() {
    lineno=0
    while true; do
        lineno=$((lineno+1))
        read -r line || break
        if [[ "$lineno" == "10" ]]; then
            echo "$line"
            break
        fi
    done
}

htSln() {
    head -n 10 file.txt | tail -n 10
}

sedSln() {
    sed -n 10p file.txt
}
