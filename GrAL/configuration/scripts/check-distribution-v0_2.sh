#! /bin/bash

TMP=${1-${HOME}/tmp}
GRAL=GrAL-0.2
GRALINSTALL=gral-v0.2
GCC95=/usr/local/bin/g++
cd ${TMP}
rm -rf $GRAL

gzip -cd ${TMP}/gral-src.tar.gz | tar -xvf -
cd $GRAL
gzip -cd ${TMP}/gral-doc.tar.gz | tar -xvf -
cp configuration/defs/mpi/success.defs configuration/defs/mpi/mpi.defs  
gmake new     DEVELOP=develop CCC=${GCC95} CCNAME=gcc
gmake install PREFIX=${TMP}/gral-v0.2
gmake test    DEVELOP=user PREFIX=${TMP}/${GRALINSTALL} TARGET=new CCC=${GCC95} CCNAME=gcc
gmake check   DEVELOP=user PREFIX=${TMP}/${GRALINSTALL} CCC=${GCC95} CCNAME=gcc
