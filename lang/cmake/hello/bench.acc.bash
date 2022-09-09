cmd=$1

case $cmd in
    ninja)
        rm -rf build_gn
        mkdir build_gn
        pushd build_gn
        cmake .. -G Ninja
        time ninja
        popd
        ;;
    make|*)
        rm -rf build_mk
        mkdir build_mk
        pushd build_mk
        cmake ..
        time make
        popd
        ;;
esac
