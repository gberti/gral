#! /bin/bash


cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/maintenance/scripts/modules.sh`;
#MODULES=sequence
cd $cvswork
for i in ${MODULES}
do
 cd $i;
 gmake clean 
 gmake new   
 gmake test TARGET=new-check-clean
 gmake clean 
 cd $cvswork
done;

