#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}

$cvswork/configuration/scripts/distribution-src.sh ${TAG}
$cvswork/configuration/scripts/distribution-doc.sh ${TAG}
