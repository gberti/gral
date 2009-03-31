/*! \file test-simple-graphics.C
    Some tests and examples for using the GraphicsDevice framework.

 */

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/oogl.h"
#include "GraphicsDevice/graphics-device.h"
#include "GraphicsDevice/all-geom.h"

#include <fstream>

namespace GrAL {

inline DevManip1<Transformation> 
BT(const Transformation& T) { return BeginTransformation(T);}

inline void 
ET(rendering_language& L) { L.end_transformation();}


inline DevManip1<Attribute> 
BA(const Attribute& A) { return BeginAttribute(A);}

inline void 
EA(rendering_language& L) { L.end_attribute();}

}

int main() {
  using namespace GrAL;

  std::ofstream out("simple.out");
  GraphicsDevice dev(OOGLDevice(out));
  point zero(0,0,0), e1(1,0,0), e2(0,1,0), e3(0,0,1);
  RenderableGeom Q1 = RQuadrilateral(e1,e1+e2, e2, zero);
  RenderableGeom T1 = RTriangle(e3, e1+e3, e2+e3);
  Transformation T(Translate(point(2,0,0)));

  Transformation RX(Rotation3D(e1, M_PI/2));
  Transformation RY(Rotation3D(e2, M_PI/2));
  Transformation RZ(Rotation3D(e3, M_PI/2));
  Transformation S(Scale3D(0.25));

  Transformation CenterQuad(Translate(point(-0.5,-0.5,0)));
  Transformation RotQuad(RZ);

  RenderableGeom F = RFileGeom("cube.off");

  dev << BeginGroup("quads") 
      << Q1
      << BA(Attribute::red())   << BT(T)  << Q1 << ET << EA
      << BA(Attribute::blue())  << BT(RX) << Q1 << ET << EA
      << BA(Attribute::green()) << BT(RY) << Q1 << ET << EA
      << BA(Attribute::black()) << BT(RZ) << Q1 << ET << EA
      << EndGroup
      << BeginGroup("triangle")
      << T1
      << EndGroup
      << BeginGroup("cube")
      << BT(T(S)) << F << ET
      << EndGroup
      << BeginGroup("quad T o Rot")
      << BT(CenterQuad(RotQuad))
      << BA(Attribute::magenta()) <<  Q1 << EA
      << ET
      << EndGroup
      << BeginGroup("quad Rot o T")
      << BT(RotQuad(CenterQuad))
      << BA(Attribute::yellow()) <<  Q1 << EA
      << ET
      << EndGroup
      << BeginGroup("Origin")
      << RSphere(0.1, point(0,0,0))
      << EndGroup;
  out.close();

  return 0;

}

