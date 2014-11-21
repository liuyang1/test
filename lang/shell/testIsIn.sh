
is_in(){
    value=$1
    shift
    for var in $*; do
        echo "enum $var"
        [ $var = $value ] && return 0
    done
    return 1
}

ARCH_LIST=( x86 arm)
ARCH_LIST="x86 arm"
target_arch=arm

echo $target_arch
echo ${ARCH_LIST[@]}
if ! is_in $target_arch ${ARCH_LIST[@]} ; then
    echo "not"
else
    echo "yes"
fi
