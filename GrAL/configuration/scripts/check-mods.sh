#! /bin/bash

CVSPG=${HOME}/bin/cvscb
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;

cd ${HOME}/CVS-work
for i in ${MODULES}
do
 echo ">>>>>>  $i ";
 cd $i; 
 cvsstat -cvs ${CVSPG} -mods;
 cd ${HOME}/CVS-work;
done;

