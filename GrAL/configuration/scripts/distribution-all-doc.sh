#! /bin/bash


CVSREPO=/home/nmwr/CVS;
cvswork=${CVSWORK-${HOME}/CVS-work}
#MODULES=`$cvswork/configuration/scripts/modules.sh`;
MODULES="configuration configuration utilities"

TMP=${HOME}/tmp;
ALLROOT=${TMP}/modules;
cd ${HOME}/tmp;
rm -rf ${ALLROOT};
mkdir -p  ${ALLROOT};
cd  ${ALLROOT};
for i in ${MODULES}
do
 echo $i; 
 cvs -d ${CVSREPO} export -r HEAD $i;
done;

for i in ${MODULES}
do
 cd ${ALLROOT}/$i;
 gmake doxy1 ALLROOT=${ALLROOT}
done;

for i in ${MODULES}
do
 cd ${ALLROOT}/$i;
 gmake installdox ALLROOT=${ALLROOT} DOCROOT=../ 
done;


mkdir ${ALLROOT}/Docs;
cd ${ALLROOT}/Docs;
for i in ${MODULES}
do
  mkdir -p $i
  cp ${ALLROOT}/$i/doxygen/html/* $i
done;
cd ${ALLROOT};

tar cf gral-doc.tar ${ALLROOT}/Docs
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
rm -rf ${ALLROOT};

