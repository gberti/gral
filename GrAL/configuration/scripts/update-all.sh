#! /bin/bash

cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/maintenance/scripts/modules.sh`;


cd ${HOME}/CVS-work;
for i in ${MODULES}
do
 cd $i;
 echo ">>>>>>>>>> Updating $i:"
 cvs update;
 cd ${cvswork}
done;

