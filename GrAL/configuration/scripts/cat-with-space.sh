#! /bin/sh

for i in $@
do
    cat $i | perl -p -e "s/\n//g"
    echo -n " "
done