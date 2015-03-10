plainFunc() {
    echo "before assign in func $a"
    a="a"
    echo "after assign in func $a"
}
f() {
    echo "before assign in func $a"
    local a="a"
    echo "after assign in func $a"
}
a="A"
echo "origin $a"
plainFunc
echo "after func $a"
echo "change external VAR without LOCAL decartor"

echo "----"
a="A"
echo "origin $a"
f
echo "after func $a"
echo "NOT change external VAR with LOCAL decartor"
