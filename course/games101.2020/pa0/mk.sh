set -e
out=build
[[ -d $out ]] || mkdir $out

pushd $out
cmake ..
make
./Transformation
popd
