#!/usr/bin/env bash

cd libcommon
make
cd ../

cd libnet
make
cd ../

#! ================================

cd tolua
make
cd ../

cd libredis
make
cd ../

cd libmysql
make
cd ../

cd liblua
make
cd ../

cd rocket
make
cd ../

#! ================================


