set -e
out=build
[[ -d $out ]] || mkdir $out

pushd $out
cmake ..
make
./Rasterizer
popd
