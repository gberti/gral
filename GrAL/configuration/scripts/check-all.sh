#! /bin/bash


CVSREPO=/home/nmwr/CVS;
cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
CHECKDIR=check-modules;

cd ${HOME}/tmp;
rm -rf ${CHECKDIR};
mkdir ${CHECKDIR};
cd ${CHECKDIR}
for i in ${MODULES}
do
 cvs -d ${CVSREPO} export -r HEAD $i;
 echo $i
 done;
for i in ${MODULES}
do
 cd $i;
 gmake clean  GRALROOT=${HOME}/tmp/${CHECKDIR}
 gmake new    GRALROOT=${HOME}/tmp/${CHECKDIR}
 gmake test   GRALROOT=${HOME}/tmp/${CHECKDIR}  TARGET=new-check-clean
 gmake clean  GRALROOT=${HOME}/tmp/${CHECKDIR} 
 cd ${HOME}/tmp/${CHECKDIR};
done;
cd ..;
#rm -rf ${CHECKDIR};

