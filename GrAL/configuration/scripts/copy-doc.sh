#! /bin/bash


cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
#MODULES="configuration configuration utilities"

DOCDEST=$1

for i in ${MODULES}
do
  #echo "mkdir ${DOCDEST}/$i";
  mkdir -p ${DOCDEST}/$i
  (cd ${cvswork}/$i; gmake copydoc DOCDEST=${DOCDEST}/$i)
done;

