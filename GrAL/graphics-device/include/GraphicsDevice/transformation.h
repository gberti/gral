#ifndef NMWR_GB_TRANSFORMATION_H
#define NMWR_GB_TRANSFORMATION_H

#include "RFunction/RFunction.h"

class RenderableGeom;

class Transformation: public RFunction {
public:
  Transformation() : RFunction(Identity3D) {}
  Transformation(const RFunction& F);
  Transformation(const Transformation& T);
  Transformation& operator= (const RFunction& F);
  Transformation& operator= (const Transformation& T);
  RenderableGeom  operator()(const RenderableGeom& G) const;
  point           operator()(const point&) const;
  Transformation  operator()(const Transformation& T) const;
};

class mat4;
extern mat4 GetMat4(const Transformation& T);

#include "Work-around/default-operators.h"
MAKE_DEFAULT_OPS(Transformation,"Transformation");
#undef MAKE_DEFAULT_OPS

#endif
