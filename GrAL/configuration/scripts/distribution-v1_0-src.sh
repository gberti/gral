#! /bin/bash

source ${HOME}/.bashrc

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules-v1_0.sh`;
EXCLUDED=`$cvswork/configuration/scripts/excluded-v1_0.sh`;
TOPLEVELMODULES=`$cvswork/configuration/scripts/modules-toplevel-v1_0.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;

GRALROOT=GrAL-1.0
cd ${HOME}/tmp;
rm -rf ${GRALROOT};
mkdir ${GRALROOT};
cd ${GRALROOT};
for i in ${TOPLEVELMODULES}
do
 cvs -d ${CVSREPO} export -r ${TAG} $i;
done;
for i in ${EXCLUDED}
do
 rm -rf $i
done;
mv gral/v1_0/README  .
mv gral/v1_0/LICENSE .
mv gral/v1_0/MODULES .
mv gral/v1_0/INSTALL .
mv gral/v1_0/NEWS    .
mv gral/v1_0/Makefile ./Makefile
cp gral/v1_0/Makefile.gral gral/Makefile
mv gral/v1_0/index.dox gral/doc

perl -i -p -e "s/WITHMPI=true/WITHMPI=false/" gral/distributed/mk.config
mv configuration/defs/mpi/mpi.defs.template configuration/defs/mpi/mpi.defs

rm -rf configuration/scripts
rm -f  gral/TODO.html
rm -rf gral/v0_1 gral/v0_2 gral/v1_0


# assume docs will be installed under ./doc
mv gral/index-global.html ./index.html 
perl -p -i -e 's|\$GRALDOC|doc|g' ./index.html


# insert license text in all sources
find . -name "*.h" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;
find . -name "*.C" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;


cd ..;
tar hcf gral-src.tar ${GRALROOT};
gzip -f gral-src.tar;
rm -rf ${GRALROOT};

