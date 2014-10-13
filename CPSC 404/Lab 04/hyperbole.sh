#!/bin/bash
for i in `seq  1 7`
do
  for j in `seq 1 7`
  do
    tmp=`echo "(e($i+$j)+e(-($i+$j)))/2" | bc -l`
    printf "$tmp "
  done
  printf "\n"
done
