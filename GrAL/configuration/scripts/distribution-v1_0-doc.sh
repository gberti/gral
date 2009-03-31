#! /bin/bash

source ${HOME}/.bashrc
echo `which convert`
echo `which perl`
perl -v;

TAG=${1-HEAD}
GRALWORK=${HOME}/Sources/GrAL
scripts=${GRALWORK}/configuration/scripts;
MODULES=`$scripts/modules-v1_0.sh`;
EXCLUDED=`$scripts/excluded-v1_0.sh`;
TOPLEVELMODULES=`$scripts/modules-toplevel-v1_0.sh`;
CVSREPO=guntram@cvs.berlios.de/cvsroot/gral


TMP=${HOME}/tmp;
GRALROOT=${TMP}/GrAL;
DOC=doc
cd ${HOME}/tmp; 
rm -rf ${GRALROOT};

#cd  ${GRALROOT};

cvs -d $CVSREPO export -r ${TAG} GrAL
cd $GRALROOT

echo "####################################"
echo "Excluding modules ${EXCLUDED}"
for i in ${EXCLUDED}
do
 echo "##########  " $i "  #############"; 
 rm -rf $i
done;

cp configuration/defs/local.defs.template configuration/defs/local.defs
mv configuration/defs/mpi/mpi.defs.template configuration/defs/mpi/mpi.defs
cp gral/v1_0/index.dox gral/doc
cp gral/v1_0/Makefile.gral gral/Makefile
rm -rf gral/v1_0

make init
make predoc  GRALROOT=${GRALROOT} DOXYGEN=$HOME/bin/doxygen

for i in ${MODULES}
do
 cd ${GRALROOT}/$i;
 gmake postdoc GRALROOT=${GRALROOT} DOCROOT=../ DOXYGEN=$HOME/bin/doxygen
done;

mkdir ${GRALROOT}/${DOC};
cd ${GRALROOT}/${DOC};
for i in ${MODULES}
do
  mkdir -p $i
  (cd ${GRALROOT}/$i; gmake copydoc DOCDEST=${GRALROOT}/${DOC}/$i)
done;
cd ${GRALROOT};


tar hcf gral-doc.tar ./${DOC}
gzip -f gral-doc.tar;
mv gral-doc.tar.gz ..;
cd ..;
#rm -rf ${GRALROOT};






