
// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/renderable-geom.h"

#include "Geometry/algebra.h"

typedef Transformation::coord_type coord_type;

Transformation::Transformation(const RFunction& F) 
  : RFunction(F) {}

Transformation::Transformation(const Transformation& T) 
  : RFunction(T) {}

Transformation& Transformation::operator= (const RFunction& F)
{
  if(this != &F)
    RFunction::operator=(F);
  return (*this);
}

Transformation& Transformation::operator= (const Transformation& T)
{
  if(this != &T)
    RFunction::operator=(T);
  return (*this);
}

RenderableGeom  Transformation::operator()(const RenderableGeom& G) const
{
  RenderableGeom G1(G);
  G1.Map(*this); 
  return G1;
}

coord_type Transformation::operator()(const coord_type& p) const
{return (RFunction::operator()(p));}

Transformation Transformation::operator()(const Transformation& T) const
{return (RFunction::operator()(T));}

mat4 Transformation::GetMat4() const
{
  Transformation const& T(*this);
  REQUIRE(((T.dDef() == 3) && (T.dIm() == 3)),
	  "GetMat4: Transformation is not affine or not R^3 -> R^3",1);
  const coord_type zero(0.0,0.0,0.0);
  const coord_type e1(1.0,0.0,0.0);
  const coord_type e2(0.0,1.0,0.0);
  const coord_type e3(0.0,0.0,1.0);
  
  mat4 m;
  coord_type t0(T(zero));
  coord_type a1(T(e1)-t0), a2(T(e2)-t0), a3(T(e3)-t0), a4(t0);
  for(int row = 0; row <=2; row++){
    m[row][0] = a1[row+1];
    m[row][1] = a2[row+1];
    m[row][2] = a3[row+1];
    m[row][3] = a4[row+1];
  }
  m[3][0]=m[3][1]=m[3][2]=0.0;
  m[3][3] = 1.0;
 
 return m;
}
