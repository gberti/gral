#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
scripts=${cvswork}/configuration/scripts;
MODULES=`$scripts/modules.sh`;
TOPLEVELMODULES=`$scripts/modules-toplevel.sh`;
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

mv configuration/defs/mpi/mpi.defs.template configuration/defs/mpi/mpi.defs

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

cd ${GRALROOT};
find ./gral/base/doc -name "*.html" -exec $scripts/reduce-underscores.pl {} \; 

mkdir ${GRALROOT}/${DOC};
cd ${GRALROOT}/${DOC};
for i in ${MODULES}
do
  mkdir -p $i
  (cd ${GRALROOT}/$i; gmake copydoc DOCDEST=${GRALROOT}/${DOC}/$i)
done;
cd ${GRALROOT};

cp ${GRALROOT}/gral/index-global.html ${GRALROOT}/${DOC}/index.html

tar cf gral-doc.tar ./${DOC}
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
#rm -rf ${GRALROOT};

