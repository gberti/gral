#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_CUBE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_CUBE_H


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

#include "Utility/pre-post-conditions.h"

namespace GrAL {

/*! \brief A geometric (non-linear) cube
   
   \ingroup geometricshape
   \see RCube()
 */
class geom_cube : public geom {
private:
  typedef geom_cube self;

  coord_type  c[8];
protected:
  void init(double aa);
  void init(coord_type const& cc1,coord_type const& cc2,
	    coord_type const& cc3,coord_type const& cc4,
	    coord_type const& cc5,coord_type const& cc6,
	    coord_type const& cc7,coord_type const& cc8)
  {
    c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;c[4]=cc5;c[5]=cc6;c[6]=cc7;c[7]=cc8;
  }
public:
  geom_cube(double aa) {init(aa);}
  geom_cube(coord_type const& cc1,
            coord_type const& cc2,
            coord_type const& cc3,
            coord_type const& cc4,
	    coord_type const& cc5,
            coord_type const& cc6,
            coord_type const& cc7,
            coord_type const& cc8) 
  { init(cc1,cc2,cc3,cc4,cc5,cc6,cc7,cc8); }
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_cube(*this);}

  coord_type const&  corner(int i) const 
  {
    REQUIRE( ( i >= 1 && i <= 8),
	     "geom_cube::corner called with i= "<< i << " ! ",1);
    return c[i-1];
  }
};

/*! \brief Creator function for geometric cube
    \ingroup geometricshape
 */
extern RenderableGeom RCube(RenderableGeom::coord_type const& c1, 
                            RenderableGeom::coord_type const& c2, 
                            RenderableGeom::coord_type const& c3, 
                            RenderableGeom::coord_type const& c4, 
                            RenderableGeom::coord_type const& c5, 
                            RenderableGeom::coord_type const& c6, 
                            RenderableGeom::coord_type const& c7, 
                            RenderableGeom::coord_type const& c8);

/*! \brief Creator function for geometric cube, centered at 0, with edge length a
    \ingroup geometricshape
 */
extern RenderableGeom RCube(double a);

/*
extern RenderableGeom RCube(RenderableGeom::coord_type p000,
			    RenderableGeom::coord_type p111)
*/

} // namespace GrAL 


#endif
