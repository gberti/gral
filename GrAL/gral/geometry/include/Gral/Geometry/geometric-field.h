#ifndef GRAL_GB_GEOMETRY_GEOMETRIC_FIELD_H
#define GRAL_GB_GEOMETRY_GEOMETRIC_FIELD_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometry/point-locator.h"


#include "Utility/ref-ptr.h"

namespace GrAL {

  /*! \brief A function over the geometric domain of a mesh
   
     \ingroup interpolation

     \templateparams
       - \c GEOM is a model of $GrAL VertexGridGeometry
       - \c F    is a model of $GrAL TotalGridFunction on vertices
       - \c I is an interpolation type, for instance  \c linear_cube or \c linear_simplex 
  */
template<class GEOM, class F, class I, class GT = grid_types<typename GEOM::grid_type> >
class geometric_field {
public:
  typedef GEOM geom_type;
  typedef F    function_type;
  typedef I    interpolator_type;
  typedef typename geom_type::coord_type coord_type;
  typedef typename GT::grid_type         grid_type;
  typedef typename GT::Cell              Cell;
  typedef typename GT::Vertex            Vertex;
  typedef typename function_type::value_type result_type;
  typedef point_locator<grid_type, geom_type> point_locator_type;

  typedef typename point_locator_type::location_result_type loc_result_type;

private:

  ref_ptr<geom_type const>     the_geom;
  ref_ptr<function_type const> f;
  // could be shared by many instances ...
  ref_ptr<point_locator_type>  loc;
  result_type outside_value; 
  bool        outside_value_set;
public:
  geometric_field() : outside_value_set(false)
  {
    loc.make_shared(new point_locator_type());
  }
  geometric_field(geom_type const& geo,
		  function_type const& ff) : the_geom(geo), f(ff), outside_value_set(false)
  {
    loc.make_shared(new point_locator_type(TheGrid(), TheGeometry()));
    loc->init();
  }
  void init(ref_ptr<geom_type     const> geo, 
	    ref_ptr<function_type const> ff)
  {
    the_geom = geo;
    f        = ff;
    loc->init(TheGrid(), TheGeometry());
  }

  void init(geom_type     const& geo,
	    function_type const& ff) { init(ref_ptr<geom_type const>(geo), ref_ptr<function_type const> (ff));}


  result_type operator()(coord_type X) const 
  { 
    loc_result_type res = loc->locate(X);
    if(outside_value_set && res.tag() != loc_result_type::inside_tag)
      return outside_value;
    if(res.tag() == loc_result_type::outside_tag)
      std::cout << " field(" << X << "): No cell found!" << std::endl; 
    Cell c = loc->locate(X).TheCell();
    REQUIRE_ALWAYS(c.valid(), " X=" << X << " t=" << res.tag() << " c=" << c.handle() << "," << & (c.TheGrid()),1);
    return (*interpolator(c))(X);
  }
  result_type operator()(Vertex v) const { return (*f)(v);}
  result_type operator()(coord_type X, Cell c) const { return (*interpolator(c))(X);}

  temporary<interpolator_type> interpolator(Cell c) const
  { 
    // could pass adequate mapping of c's archetype to interpolator
    // which has been pre-calculated by interpolator_type
    // The exact type of interpolator must also depend on the archetype!
    // this is a Problem on hybrid grids
    return temporary<interpolator_type>(interpolator_type(*the_geom, *f, c));
  }


  void set_outside_value(result_type v) { outside_value = v; outside_value_set = true;}
  void unset_outside_valu() { outside_value_set = false;}

  bool inside(coord_type X) const { return loc->locate(X).tag() == loc_result_type::inside_tag;}

  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}
  ref_ptr<grid_type const> TheGrid()     const { return ref_ptr<grid_type const>(the_geom->TheGrid());}
  ref_ptr<point_locator_type const> TheLocator() const { return loc;}
  ref_ptr<point_locator_type >      TheLocator()       { return loc;}
};

} // namespace GrAL 

#endif
