
// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

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

Transformation::matrix4_type
Transformation::GetMat4() const
{
  Transformation const& T(*this);
  REQUIRE(((T.dDef() == 3) && (T.dIm() == 3)),
	  "GetMat4: Transformation is not affine or not R^3 -> R^3",1);
  const coord_type zero(0.0,0.0,0.0);
  const coord_type e1(1.0,0.0,0.0);
  const coord_type e2(0.0,1.0,0.0);
  const coord_type e3(0.0,0.0,1.0);
  
  matrix4_type m;
  coord_type t0(T(zero));
  coord_type a1(T(e1)-t0), a2(T(e2)-t0), a3(T(e3)-t0), a4(t0);
  for(int row = 1; row <=3; row++){
    m(row,1) = a1[row];
    m(row,2) = a2[row];
    m(row,3) = a3[row];
    m(row,4) = a4[row];
  }
  m(4,1)=m(4,2)=m(4,3)=0.0;
  m(4,4) = 1.0;
 
 return m;
}

} // namespace GrAL 
