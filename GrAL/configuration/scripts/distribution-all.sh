#! /bin/bash


CVSREPO=/home/nmwr/CVS;
cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules.sh`;

cd ${HOME}/tmp;
rm -rf modules;
mkdir modules;
cd modules;
for i in ${MODULES}
do
 cvs -d ${CVSREPO} export -r HEAD $i;
done;
mv gral/README .
mv gral/LICENSE .
mv gral/MODULES .
mv gral/INSTALL .
mv gral/Makefile.global ./Makefile

find . -name "*.h" -exec ${cvsroot}/configuration/replace-license.pl {} \;
find . -name "*.C" -exec ${cvsroot}/configuration/replace-license.pl {} \;


cd ..;
tar cf gral-all.tar modules;
gzip -f gral-all.tar;
rm -rf modules;

