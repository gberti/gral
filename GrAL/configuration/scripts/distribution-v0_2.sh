#! /bin/bash

TAG=${1-HEAD}
cvswork=${CVSWORK-${HOME}/CVS-work}

$cvswork/configuration/scripts/distribution-v0_2-src.sh ${TAG}
$cvswork/configuration/scripts/distribution-v0_2-doc.sh ${TAG}
