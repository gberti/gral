#! /bin/bash


cd /home/nmwr/tmp
rm -rf GrAL
gzcat ${HOME}/tmp/gral-all.tar.gz | tar -xfv -
cd GrAL
gzcat ${HOME}/tmp/gral-doc.tar.gz | tar -xfv - 
gmake new
gmake install PREFIX=/home/nmwr/tmp/gral
gmake test DEVELOP=user PREFIX=/home/nmwr/tmp/gral
