#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules-v0_2.sh`;
CVSREPO=`cat $cvswork/configuration/CVS/Root`;

ALLROOT=GrAL-0.2
cd ${HOME}/tmp;
rm -rf ${ALLROOT};
mkdir ${ALLROOT};
cd ${ALLROOT};
for i in ${MODULES}
do
 cvs -d ${CVSREPO} export -r ${TAG} $i;
done;
mv gral/v0_2/README  .
mv gral/LICENSE      .
mv gral/v0_2/MODULES .
mv gral/v0_2/INSTALL .
mv gral/NEWS         .
mv gral/Makefile.global ./Makefile

rm -rf configuration/scripts
rm -f  gral/TODO.html

# assume docs will be installed under ./doc
mv gral/index-global.html ./index.html 
perl -p -i -e 's|\$GRALDOC|doc|g' ./index.html


# insert license text in all sources
find . -name "*.h" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;
find . -name "*.C" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;


cd ..;
tar cf gral-src.tar ${ALLROOT};
gzip -f gral-src.tar;
rm -rf ${ALLROOT};
