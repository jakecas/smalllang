#!/bin/bash
DIR="$1"

test -f ./main.out || (g++ main.cpp -o main.out)

for f in $DIR/*
do
	if [ ${f: -4} != ".xml" ]
	then
	  printf "$f:\n"
    if !(./main.out "$f")
    then printf "Oops... Next File."
    fi
    echo
	fi
done
