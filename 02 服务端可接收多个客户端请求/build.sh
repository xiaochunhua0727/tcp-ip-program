#!/bin/bash

if [ -f "client" ]; then
    rm -rf client
fi

if [ -f "server" ]; then
    rm -rf server
fi

if [ ! -d "build" ]; then
  mkdir build;
fi

cd build
rm -rf *
cmake -DCMAKE_BUILD_TYPE=Debug ..;
make -j8;