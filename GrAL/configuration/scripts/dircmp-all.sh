#! /bin/bash

cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
otherdir=$1
MODULES=`$cvswork/configuration/scripts/modules.sh`;

cd $cvswork;
for i in ${MODULES}
do
 echo ">>>> Comparing $cvswork/$i and $otherdir/$i"
 dircmp -w 150 -s $i $otherdir/$i \
        | grep -v CVS | grep -v '~' | grep -v doxygen | more -s
done;

