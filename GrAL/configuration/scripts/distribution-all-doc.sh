#! /bin/bash


CVSREPO=/home/nmwr/CVS;
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
#MODULES="configuration configuration utilities"

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
 gmake predoc ALLROOT=${ALLROOT}
done;

for i in ${MODULES}
do
 cd ${ALLROOT}/$i;
 gmake postdoc ALLROOT=${ALLROOT} DOCROOT=../ 
done;


mkdir ${ALLROOT}/graldoc;
cd ${ALLROOT}/graldoc;
for i in ${MODULES}
do
  mkdir -p $i
  (cd ${ALLROOT}/$i; gmake copydoc DOCDEST=${ALLROOT}/graldoc/$i)
done;
cd ${ALLROOT};

tar cf gral-doc.tar ./graldoc
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
rm -rf ${ALLROOT};

