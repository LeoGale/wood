#!/bin/sh
set -x 

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-../build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-../${BUILD_TYPE}}
BUILD_NO_EXAMPLES=${BUILD_NO_EXAMPLES:-0}

# CMAKE_INSTALL_PREFIX is used to determine 
# the root of where the files will be installe

mkdir -p $BUILD_DIR/$BUILD_TYPE \
    && cd $BUILD_DIR/$BUILD_TYPE \
    && cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        -DCMAKE_BUILD_NO_EXAMPLES=$BUILD_NO_EXMPALES \
        $SOURCE_DIR \
    && pwd \
    && make $*
