#! /bin/sh

#rm -rf ${HOME}/tmp/checkdoc
cd ${HOME}/tmp
mkdir checkdoc
cd checkdoc
zcat ${HOME}/tmp/gral-src.tar.gz | tar -xvf -
cd GrAL-0.2
zcat ${HOME}/tmp/gral-doc.tar.gz | tar -xvf -

${HOME}/bin/checkbot --url file://localhost/home/berti/tmp/checkdoc/GrAL-0.2/index.html \
         --match localhost dimitri --exclude  --timeout 10 
