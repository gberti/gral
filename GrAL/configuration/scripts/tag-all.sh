#! /bin/bash

TAG=$1;
cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;


cd ${HOME}/CVS-work/GrAL;
for i in ${MODULES}
do
 cd $i;
 echo ">>>>>>>>>> Tagging $i:"
 cvs tag ${TAG};
 cd ${cvswork}
done;

