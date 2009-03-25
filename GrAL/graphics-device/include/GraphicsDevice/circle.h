#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_CIRCLE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_CIRCLE_H


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


/*! \brief A geometric circle
    \ingroup geometricshape
    \todo This is an ellipse, rename!
    \see RCircle()
    \see REllipse()
 */
class geom_circle : public geom {
private:
  typedef geom_circle self;
  coord_type m; //< midpoint of the circle
  double r1,r2; //< Radii of ellipse in x/y directions
protected:
  void init(coord_type const& cc, double rr) {m=cc;r1=rr;r2=rr;}
  void init(coord_type const& cc, double rr1, double rr2) {m=cc;r1=rr1;r2=rr2;}
public:
  geom_circle(coord_type const& cc, double rr) {init(cc,rr);}
  geom_circle(coord_type const& cc, double rr1, double rr2) {init(cc,rr1,rr2);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_circle(*this);}

  coord_type const& midpoint() const {return m;}
  double radius1() const {return r1;}
  double radius2() const {return r2;}

};


/*!  \brief Creator function for geometric circle
    \ingroup geometricshape
 */
extern RenderableGeom RCircle(RenderableGeom::coord_type const& c1, double r);

/*! \brief Creator function for geometric ellipse
    \ingroup geometricshape
 */
extern RenderableGeom REllipse(RenderableGeom::coord_type const& c1, 
                               double r1, double r2);


} // namespace GrAL 

#endif
