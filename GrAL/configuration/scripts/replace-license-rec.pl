#!/bin/sh

cvswork=${CVSWORK-${HOME}/CVS-work}

find . -name "*.h" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;
find . -name "*.C" -exec ${cvswork}/configuration/scripts/replace-license.pl {} \;
