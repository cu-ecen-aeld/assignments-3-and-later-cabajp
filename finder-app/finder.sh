#!/bin/sh
# Finder script for assignment 1 
# Author: Peter Cabaj

# Accepts the following runtime arguments: the first argument is a path to a directory on the
# filesystem, referred to below as filesdir; the second argument is a text string which will be
# searched within these files, referred to below as searchstr

# Exits with return value 1 error and print statements if any of the parameters above were not
# specified

# Exits with return value 1 error and print statements if filesdir does not represent a
# directory on the filesystem

# Prints a message "The number of files are X and the number of matching lines are Y" where X is
# the number of files in the directory and all subdirectories and Y is the number of matching
# lines found in respective files, where a matching line refers to a line which contains
# searchstr (and may also contain additional content).

set -e


filesdir=./
searchstr=test

if [ $# -lt  2 ]
then
	echo "missing input argument"
	exit 1
else
	filesdir=$1
	searchstr=$2
fi

if [ -d $filesdir ]
then
	#echo "file ${filesdir}, str ${searchstr}"
	echo "The number of files are $(find $filesdir -type f -exec grep -l $searchstr {} \; | wc -l) and the number of matching lines are $(find $filesdir -type f -exec grep $searchstr {} \; | wc -l)"
else
	echo "dir ko"
	exit 1 
fi


