#! /usr/bin/env bash
set -e errexit

dw_test() {
    DIR="test"
    FN="kdtree-testing.zip"

    [[ -d "$DIR" ]] || mkdir "$DIR"
    pushd "$DIR"
    wget http://coursera.cs.princeton.edu/algs4/testing/kdtree-testing.zip
    unzip "$FN"
    rm -rf "$FN"
    popd

    echo "download $FN"
}

dw_misc() {
    wget http://coursera.cs.princeton.edu/algs4/testing/kdtree/KdTreeVisualizer.java
    wget http://coursera.cs.princeton.edu/algs4/testing/kdtree/RangeSearchVisualizer.java
    wget http://coursera.cs.princeton.edu/algs4/testing/kdtree/NearestNeighborVisualizer.java
}

dw_test
dw_misc
