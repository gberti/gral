#! /bin/bash

source ${HOME}/.bashrc
echo `which convert`
echo `which perl`
perl -v;

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules-v0_2.sh`;
EXCLUDED=`$cvswork/configuration/scripts/excluded-v0_2.sh`;
TOPLEVELMODULES=`$cvswork/configuration/scripts/modules-toplevel-v0_2.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;


TMP=${HOME}/tmp;
GRALROOT=${TMP}/modules;
DOC=doc
cd ${HOME}/tmp; 
rm -rf ${GRALROOT};
mkdir -p  ${GRALROOT};
cd  ${GRALROOT};

echo "####################################"
echo "Exporting modules ${TOPLEVELMODULES}"
for i in ${TOPLEVELMODULES}
do
 echo "##########  " $i "  #############"; 
 cvs -d ${CVSREPO} export -r ${TAG} $i;
done;

echo "####################################"
echo "Excluding modules ${EXCLUDED}"
for i in ${EXCLUDED}
do
 echo "##########  " $i "  #############"; 
 rm -rf $i
done;


mv configuration/defs/mpi/mpi.defs.template configuration/defs/mpi/mpi.defs
cp gral/v0_2/index.dox gral/doc
cp gral/v0_2/Makefile.gral gral/Makefile
rm -rf gral/v0_2

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






