#! /bin/bash

source ${HOME}/.bashrc

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/simbio/modules.sh`;
EXCLUDED=`$cvswork/configuration/scripts/simbio/excluded.sh`;
TOPLEVELMODULES=`$cvswork/configuration/scripts/simbio/modules-toplevel.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;

GRALROOT=GrAL-for-Simbio

cd ${HOME}/tmp;
rm -rf ${GRALROOT};
mkdir -p ${GRALROOT};
cd ${GRALROOT};
echo `pwd`;

for i in ${TOPLEVELMODULES}
do
 cvs -d ${CVSREPO} export -r ${TAG} $i;
done;
for i in ${EXCLUDED}
do
 rm -rf $i
done;
mv gral/v0_2/README  .
mv gral/v0_2/LICENSE .
mv gral/v0_2/MODULES .
mv gral/v0_2/INSTALL .
mv gral/NEWS         .
mv gral/v0_2/Makefile ./Makefile
#cp gral/v0_2/Makefile.gral gral/Makefile
mv gral/v0_2/index.dox gral/doc

for i in ${EXCLUDED}
do
 s=`basename $i`;
 echo $s;
 echo perl -p -i -e "s|$s||g"  gral/Makefile
 perl -p -i -e "s|$s||g"  gral/Makefile
done

perl -p -i -e "s|(SUBDIRS[ ]*=[ ]*)|\1 function-algebra graphics-device |" Makefile

#perl -i -p -e "s/WITHMPI=true/WITHMPI=false/" gral/distributed/mk.config
#mv configuration/defs/mpi/mpi.defs.template configuration/defs/mpi/mpi.defs

rm -rf configuration/scripts
rm -f  gral/TODO.html
rm -rf gral/v0_1 gral/v0_2


# assume docs will be installed under ./doc
mv gral/index-global.html ./index.html 
perl -p -i -e 's|\$GRALDOC|doc|g' ./index.html


# insert license text in all sources
find . -name "*.h" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;
find . -name "*.C" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;


cd ..;
tar cf gral-for-simbio.tar ${GRALROOT};
gzip -f gral-for-simbio.tar;
#rm -rf ${GRALROOT};

