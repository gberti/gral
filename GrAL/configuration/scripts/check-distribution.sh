#! /bin/bash

TMP=${1-${HOME}/tmp}

cd ${TMP}
rm -rf GrAL
rm -rf gral
gzip -cd ${TMP}/gral-src.tar.gz | tar -xvf -
cd GrAL
gzip -cd ${TMP}/gral-doc.tar.gz | tar -xvf - 
gmake new
gmake install PREFIX=${TMP}/gral
gmake check DEVELOP=user PREFIX=${TMP}/gral
