#! /bin/bash


cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules.sh`;

cd ${HOME}/CVS-work
for i in ${MODULES}
do
 echo $i;
 cd $i; 
 cvsstat -mods;
 cd ${HOME}/CVS-work;
done;

