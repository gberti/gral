#! /bin/bash

# execute a command for all modules
# example: do-all-cvswork.sh 'gmake clean'

cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;

cd ${cvswork}
for i in ${MODULES}
do
 cd $i;
 echo ">>> Module $i"; 
 echo "Executing $1";
 echo "#! /bin/bash" > tmp.sh;
 echo "PATH=${PATH}:${cvswork}/configuration/scripts/;" >> tmp.sh;
 echo "$1" >> tmp.sh;
 # cat tmp.sh;
 chmod ugo+x tmp.sh;
 ./tmp.sh;
 rm tmp.sh;
 echo "";
 cd ${cvswork}
done;

