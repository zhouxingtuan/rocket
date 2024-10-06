#!/usr/bin/env bash

mkdir -p build

for file in *.proto
do
	if test -f $file
	then
		#cd tool/
		#echo $file 是文件
		file_name=$(basename $file .proto)
		pb_file=${file_name}".lua"
		echo $pb_file
		lua tool/gen.lua $file 0 build
		#cd ../
		cp build/$pb_file ../script/pb/
	fi
	if test -d $file
	then
		echo $file 是目录
	fi
done



