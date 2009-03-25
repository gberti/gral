#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_SPHERE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_SPHERE_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

/*! \brief A geometric sphere
   
   \ingroup geometricshape
   \see RSphere()
 */
class geom_sphere : public geom {
private:
  typedef geom_sphere self;
  double     r; // radius
  coord_type m;  // midpoint
protected:
  void init(double rr, coord_type const& mm) {r=rr;m=mm;}
public:
  geom_sphere(double rr, coord_type const& mm) {init(rr,mm);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_sphere(*this);}

  double             radius()   const { return r;}
  coord_type const&  midpoint() const { return m;}
};


/*! \brief Creator function for geometric sphere

   \ingroup geometricshape
 */
extern RenderableGeom RSphere(double r,  RenderableGeom::coord_type const& m);

} // namespace GrAL 

#endif
