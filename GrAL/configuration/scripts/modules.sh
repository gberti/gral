#! /bin/bash

# central place to collect the names of all modules
# on which the collective scripts are going to be executed

MODULES='configuration utilities external-control sequence geometry 
         function-algebra graphics-device
         gral gral/base gral/complex2d gral/distributed gral/partitioning
         gral/cartesian2d gral/cartesian3d gral/complex3d gral/gmv-io
         gral/testing gral/triang2d gral/geomview-io gral/measurement';

echo $MODULES;
