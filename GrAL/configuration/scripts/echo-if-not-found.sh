#! /bin/bash


egrep $1 $2 > /dev/null || echo "$2: $1 not found";