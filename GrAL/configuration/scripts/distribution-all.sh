#! /bin/bash

cvswork=${CVSWORK-${HOME}/CVS-work}

$cvswork/configuration/scripts/distribution-src.sh
$cvswork/configuration/scripts/distribution-doc.sh
