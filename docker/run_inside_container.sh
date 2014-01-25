#!/bin/sh
set -xe

mkdir /qjson_build
cd /qjson_build
cmake -DCMAKE_BUILD_TYPE=DEBUG -DQJSON_BUILD_TESTS=yes /qjson
make
make test
