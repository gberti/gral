#! /bin/bash


cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
dopt=opt;
dbg=${1-$dopt};

cd ${cvswork}
for i in ${MODULES}
do
 cd $i;
 gmake new DEBUG=$dbg;
 gmake install DEBUG=$dbg;
 cd ${cvswork}
done;

