#!/usr/bin/env bash

cd hiredis
make
cd ../

cd LuaJIT
make
sudo make install
cd ../

cd luarapidjson
make
cd ../

cd snappy
make
cd ../

cd tolua
mkdir -p lib
mkdir -p bin
make clean
make
make lua
make tolua++
sudo make install
cd ../

cd crc32c
mkdir -p build
cd build
cmake ../
make
cd ../../

cd leveldb
mkdir -p build
cd build
cmake ../
make
cd ../../


