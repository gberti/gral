#! /bin/bash

TAG=${1-HEAD}
#TAG=v0_2
cvswork=${CVSWORK-${HOME}/CVS-work}
scripts=$cvswork/configuration/scripts/


$scripts/distribution-v0_2-src.sh ${TAG}
$scripts/distribution-v0_2-doc.sh ${TAG}
$scripts/check-distribution-v0_2.sh 
$scripts/check-distribution-v0_2-doc.sh