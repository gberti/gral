#! /bin/sh

cvswork=${CVSWORK-${HOME}/CVS-work}
MODULES=`$cvswork/configuration/scripts/modules-toplevel.sh`;


cd ${HOME}/CVS-work;
for i in ${MODULES}
do
 cd $i;
 echo ">>>>>>>>>> Updating $i:"
 cvs update -d;
 cd ${cvswork}
done;

