#! /bin/sh

cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;


cd ${HOME}/CVS-work/GrAL;
for i in ${MODULES}
do
 cd $i;
 echo ">>>>>>>>>> Updating $i:"
 cvs update -d;
 cd ${cvswork}
done;

