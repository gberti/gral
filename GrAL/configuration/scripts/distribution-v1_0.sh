#! /bin/bash

TAG=${1-HEAD}
#TAG=v0_2
cvswork=${CVSWORK-${HOME}/CVS-work/GrAL}

$cvswork/configuration/scripts/distribution-v1_0-src.sh ${TAG}
$cvswork/configuration/scripts/distribution-v1_0-doc.sh ${TAG}
