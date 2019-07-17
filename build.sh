#!/bin/bash 
## script to build all targets 

cd third_party
./configure
cd -

cmake CMakeLists.txt
if [ $? -eq 0 ]; then
    echo 'there was an error with cmake, exiting'
fi 

make all
