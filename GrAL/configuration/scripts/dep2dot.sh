#! /bin/bash


cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}
MODULES=`$cvswork/configuration/scripts/modules.sh`;

cd ${cvswork}
echo "digraph Dependencies { "
echo "  concentrate=true"
for i in ${MODULES}
do
 deps=`cat $i/dependencies`;
 for dep in $deps
 do
  if [ $dep != "configuration" ] 
  then
  echo "  \"$i\" -> \"$dep\""
  fi
 done;
done;
echo "}"

