#! /bin/bash

source ${HOME}/.bashrc
echo `which convert`
echo `which perl`
perl -v;

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
scripts=${cvswork}/configuration/scripts;
MODULES=`$scripts/modules-v1_0.sh`;
EXCLUDED=`$scripts/excluded-v1_0.sh`;
TOPLEVELMODULES=`$scripts/modules-toplevel-v1_0.sh`;
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
cp gral/v1_0/index.dox gral/doc
cp gral/v1_0/Makefile.gral gral/Makefile
rm -rf gral/v1_0

for i in ${MODULES}
do
 cd ${GRALROOT}/$i;
 gmake predoc GRALROOT=${GRALROOT} DOXYGEN=$HOME/bin/doxygen
done;

for i in ${MODULES}
do
 cd ${GRALROOT}/$i;
 gmake postdoc GRALROOT=${GRALROOT} DOCROOT=../ DOXYGEN=$HOME/bin/doxygen
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


tar cf gral-doc.tar ./${DOC}
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
#rm -rf ${GRALROOT};






