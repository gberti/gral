#! /bin/bash

CVSPG=cvs #${HOME}/bin/cvscb
cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;

cd ${HOME}/CVS-work/GrAL
for i in ${MODULES}
do
 echo ">>>>>>  $i ";
 cd $i; 
 cvsstat -cvs ${CVSPG} -mods;
 cd ${HOME}/CVS-work/GrAL;
done;

