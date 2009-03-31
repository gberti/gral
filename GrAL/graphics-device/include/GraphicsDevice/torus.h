#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_TORUS_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_TORUS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

/*! \brief Geometric torus
  \ingroup geometricshape
  \see RTorus()
  \todo style parameter really belongs to rendering_language
*/
class geom_torus : public geom {
private:
  typedef geom_torus self;
  double r1;  //< major radius
  double r2;  //< minor radius
  char style; //< bezier or not ?
protected:
  void init(double rr1, double rr2, char ssty) {r1=rr1;r2=rr2;style=ssty;}
public:
  geom_torus(double rr1, double rr2, char ssty) {init(rr1,rr2,ssty);}
  virtual self * clone() const { return new self(*this); }

  virtual void  write_geom_to(rendering_language& L) const;

  double major_radius() const {return r1;}
  double minor_radius() const {return r2;}
};


/*! \brief Creator function for geometric torus
    \ingroup geometricshape
    \param style 'n' => no bezier approximation
 */
extern RenderableGeom RTorus(double major,
                             double minor, 
                             char style='n');


} // namespace GrAL 

#endif
