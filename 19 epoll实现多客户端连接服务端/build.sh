#!/bin/bash

if [ -f "client" ]; then
    rm -rf client
fi

if [ -f "server" ]; then
    rm -rf server
fi

if [ ! -d "debug" ]; then
  mkdir debug;
fi

cd debug
rm -rf *
cmake -DCMAKE_BUILD_TYPE=Debug ..;
make -j8;