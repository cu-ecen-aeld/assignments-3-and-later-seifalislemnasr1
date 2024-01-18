#!/bin/bash

if [ $# -lt 2 ] 
then 
   echo "No arguments provided"
   exit 1
fi

dir=$(dirname $1)
file=$(basename $1)
if [ -d $dir ] 
then
   cd $dir
   echo $2 >> file
else
    sudo mkdir -p $dir 
    cd $dir
    echo $2 > $file
fi

