#! /bin/sh 

(cd test;
 perl -p -e "s|\\\\\\n| |g;" Makefile | grep SUB | perl -p -e "s/[ ]+/\\n/g;" | grep -v SUB | grep -v '=' | sort > /tmp/mk.txt;
 ls . | sort | grep -v Makefile | grep -v CVS > /tmp/dir.txt;
 diff /tmp/mk.txt /tmp/dir.txt | grep ">")

 
