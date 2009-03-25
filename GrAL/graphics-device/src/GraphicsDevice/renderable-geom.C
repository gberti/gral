

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/renderable-geom.h"
#include "GraphicsDevice/geom.h"

#include "FunctionAlgebra/function-algebra.h"

namespace GrAL {

RenderableGeom::RenderableGeom() : g((geom*) 0), T(Identity3D), has_trafo(false) {} // g = empty_geom
RenderableGeom::RenderableGeom(geom* gg) : g(gg), T(Identity3D), has_trafo(false) {init();}
 
RenderableGeom::~RenderableGeom() {}

void RenderableGeom::init() { T=Identity3D; has_trafo = false;}
bool RenderableGeom::HasTrafo() const { return has_trafo;}
bool RenderableGeom::HasAttr()  const { return (! A.IsEmpty());}

Transformation RenderableGeom::trafo() const { return T;}
Attribute      RenderableGeom::attr()  const { return A;}

void   RenderableGeom::write_geom_to(rendering_language& L) const
 {geometry()->write_geom_to(L);}

void RenderableGeom::Map(const Transformation& T1)
{ 
  T = T1(T);
  has_trafo = true;
}

} // namespace GrAL 
