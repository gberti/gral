#include "Graphics/translators.h"
#include "Graphics/transformation.h"
#include "Graphics/renderable-object.h"
#include "Graphics/geom.h"



RenderableGeom::RenderableGeom() : g((geom*) 0), T(Identity3D), A() {} // g = empty_geom
RenderableGeom::RenderableGeom(geom* gg) : g(gg), T(Identity3D) {init();}
 
RenderableGeom::~RenderableGeom() {}

void RenderableGeom::init() {T=Identity3D;}
bool RenderableGeom::HasTrafo() const {return ! (RFunction(T) == Identity3D);}
bool RenderableGeom::HasAttr()  const {return (! A.IsEmpty());}

Transformation RenderableGeom::trafo() const { return T;}
Attribute      RenderableGeom::attr()  const { return A;}

void   RenderableGeom::write_geom_to(rendering_language& L) const
 {geometry()->write_geom_to(L);}

void RenderableGeom::Map(const Transformation& T1)
{ T = T1(T);}
/*
RenderableGeom& RenderableGeom::operator *=(const transformation& T1)
{
 if(T == 0)
   T = T1.copy();
 else 
   T->premultiply_with(T1); //  T  <==   T1 o T : x -> T1(T(x)) 
}
*/
