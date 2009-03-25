#ifndef NMWR_GB_GRAPHICSDEVICE_TRANSFORMATION_H
#define NMWR_GB_GRAPHICSDEVICE_TRANSFORMATION_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "FunctionAlgebra/function-algebra.h"
#include "Geometry/matrix.h"

namespace GrAL {

class RenderableGeom;

class Transformation: public RFunction {
public:
  Transformation() : RFunction(Identity3D) {}
  Transformation(const RFunction& F);
  Transformation(const Transformation& T);
  Transformation& operator= (const RFunction& F);
  Transformation& operator= (const Transformation& T);
  RenderableGeom  operator()(const RenderableGeom& G) const;
  coord_type      operator()(const coord_type&) const;
  Transformation  operator()(const Transformation& T) const;

  typedef matrix<4,4> matrix4_type;
  matrix4_type GetMat4() const;
};

} // namespace GrAL 

#endif
