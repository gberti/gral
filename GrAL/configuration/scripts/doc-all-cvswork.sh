#! /bin/bash


cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;

source ${HOME}/.bashrc
cd $cvswork
make -f gral/Makefile.global doc


