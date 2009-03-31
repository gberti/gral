#! /bin/bash

DOCDEST=$1
shift
MODULES="$@"

GRALWORK=${HOME}/Sources/GrAL
MODULES_DEFAULT=`$GRALWORK/configuration/scripts/modules.sh`;

#MODULES="configuration configuration utilities"

echo $MODULES
for i in ${MODULES}
do
  #echo "mkdir ${DOCDEST}/$i";
  mkdir -p ${DOCDEST}/$i
  (cd ${GRALWORK}/$i; gmake copydoc DOCDEST=${DOCDEST}/$i)
done;

