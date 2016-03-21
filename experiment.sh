#!/bin/bash

file_name=$1

for i in `seq 0 $2`;
do
   ./imoavmp $file_name $i
done

