#!/bin/sh
# writer script for assignment 1 
# Author: Peter Cabaj


if [ $# -lt  2 ]
then
	echo "missing input argument"
	exit 1
else
	file=$1
	str=$2
	dir=${file%/*}
	echo "File to write ${file}, string ${str}, dir ${dir}"
fi

if [ ! -d $dir ]
then
	mkdir -p $dir	
fi	

echo $str > $file

if [ ! -e $file ]
then
	echo "${file} could not be created"
	exit 1 
fi	
