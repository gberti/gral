#! /bin/bash

TMP=$1

cd ${TMP}
rm -rf GrAL
rm -rf gral
gzip -cd ${TMP}/gral-all.tar.gz | tar -xvf -
cd GrAL
gzip -cd ${TMP}/gral-doc.tar.gz | tar -xfv - 
gmake new
gmake install PREFIX=${TMP}/gral
gmake check DEVELOP=user PREFIX=${TMP}/gral
