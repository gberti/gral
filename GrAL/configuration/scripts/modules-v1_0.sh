#! /bin/bash

# central place to collect the names of all modules
# of GRaL version 0.2

MODULES='configuration utilities external-control
         sequence geometry 
         graphics-device function-algebra
         gral gral/base gral/testing gral/tools gral/geometry gral/measurement 
         gral/complex2d  gral/complex3d gral/complexnd 
         gral/cartesian2d gral/cartesian3d gral/cartesiannd
         gral/testing gral/triang2d gral/triang3d 
         gral/cgns-io gral/gmv-io gral/geomview-io gral/vtk
         gral/partitioning gral/distributed gral/hierarchical   '


echo $MODULES;
