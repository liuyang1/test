# test cmake+Makefile

mkdir build
cd build
cmake ..
make

# test cmake + Ninja

mkdir build
cd build
cmake .. -G Ninja
ninja

