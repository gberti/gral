#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
TOPLEVELMODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;


TMP=${HOME}/tmp;
GRALROOT=${TMP}/modules;
DOC=doc
cd ${HOME}/tmp; 
rm -rf ${GRALROOT};
mkdir -p  ${GRALROOT};
cd  ${GRALROOT};
for i in ${TOPLEVELMODULES}
do
 echo $i; 
 cvs -d ${CVSREPO} export -r ${TAG} $i;
done;

for i in ${MODULES}
do
 cd ${GRALROOT}/$i;
 gmake predoc GRALROOT=${GRALROOT}
done;

for i in ${MODULES}
do
 cd ${GRALROOT}/$i;
 gmake postdoc GRALROOT=${GRALROOT} DOCROOT=../ 
done;


mkdir ${GRALROOT}/${DOC};
cd ${GRALROOT}/${DOC};
for i in ${MODULES}
do
  mkdir -p $i
  (cd ${GRALROOT}/$i; gmake copydoc DOCDEST=${GRALROOT}/${DOC}/$i)
done;
cd ${GRALROOT};


tar cf gral-doc.tar ./${DOC}
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
#rm -rf ${GRALROOT};

