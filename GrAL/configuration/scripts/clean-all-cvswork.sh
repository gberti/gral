#! /bin/bash

cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
dopt=opt;
dbg=${1-dopt};

cd $cvswork;
for i in ${MODULES}
do
 cd $i;
 gmake realclean;
 cd ${HOME}/CVS-work/GrAL;
done;

