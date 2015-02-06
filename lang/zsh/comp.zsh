_hexo_completion() {
    if (( CURRENT == 2 )); then
        compadd generate server clean
    fi
}

hexo() {
    echo "$1"
    echo "this is test"
}
compdef _hexo_completion hexo
