#!/bin/bash

if [ $# -lt 2 ]
then 
   echo "No arguments provided"
   exit 1
fi
if [ ! -d $1 ]
then 
   exit 1
fi   
cd $1
X=$(find . -type f| wc -l)

Y=$(grep -r $2 * | wc -l)
 
 
echo "The number of files are $X and the number of matching lines are $Y"

