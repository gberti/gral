#! /bin/bash

source ${HOME}/.bashrc;
GCC95=/usr/local/bin/g++
CCC=${GCC95}
CCNAME=gcc
export MPICH_CCC=${CCC}
export MPICH_CCLINKER=${CCC}
export LD_LIBARY_PATH=${HOME}/lib/:${LD_LIBRARY_PATH}

cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
#MODULES=sequence
echo $cvswork

cd $cvswork
for i in ${MODULES}
do
 pwd;
 cd $i;
 gmake new CCC=${CCC} CCNAME=${CCNAME}  
 cd $cvswork
done;
for i in ${MODULES}
do
 cd $i;
 gmake test-check-clean TARGET=new CCC=${CCC} CCNAME=${CCNAME}
 cd $cvswork
done;

