#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules.sh`;
TOPLEVELMODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;


TMP=${HOME}/tmp;
ALLROOT=${TMP}/modules;
DOC=doc
cd ${HOME}/tmp; 
rm -rf ${ALLROOT};
mkdir -p  ${ALLROOT};
cd  ${ALLROOT};
for i in ${TOPLEVELMODULES}
do
 echo $i; 
 cvs -d ${CVSREPO} export -r ${TAG} $i;
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


mkdir ${ALLROOT}/${DOC};
cd ${ALLROOT}/${DOC};
for i in ${MODULES}
do
  mkdir -p $i
  (cd ${ALLROOT}/$i; gmake copydoc DOCDEST=${ALLROOT}/${DOC}/$i)
done;
cd ${ALLROOT};


tar cf gral-doc.tar ./${DOC}
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
#rm -rf ${ALLROOT};

