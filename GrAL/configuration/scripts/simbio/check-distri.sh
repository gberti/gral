#! /bin/bash

TMP=${1-${HOME}/tmp}
GRAL=GrAL-for-Simbio
GRALINSTALL=gral-for-simbio
GCC95=/usr/bin/g++
cd ${TMP}
rm -rf $GRAL ${GRALINSTALL}

gzip -cd ${TMP}/gral-for-simbio.tar.gz | tar -xvf -
cd $GRAL
gmake new     DEVELOP=develop CCC=${GCC95} CCNAME=gcc
gmake install PREFIX=${TMP}/${GRALINSTALL}
gmake test    DEVELOP=user PREFIX=${TMP}/${GRALINSTALL} TARGET=new CCC=${GCC95} CCNAME=gcc
gmake check   DEVELOP=user PREFIX=${TMP}/${GRALINSTALL} CCC=${GCC95} CCNAME=gcc
