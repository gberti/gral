#! /bin/bash


CVSREPO=/home/nmwr/CVS;
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/maintenance/scripts/modules.sh`;

cd ${HOME}/tmp;
rm -rf modules;
mkdir modules;
cd modules;
for i in ${MODULES}
do
 cvs -d ${CVSREPO} export -r HEAD $i;
done;
cp ${HOME}/CVS-work/maintenance/README .
cd ..;
tar cf gral-all.tar modules;
gzip -f gral-all.tar;
rm -rf modules;

