#ifndef GRAL_GB_GEOMETRY_GEOMETRIC_FIELD_H
#define GRAL_GB_GEOMETRY_GEOMETRIC_FIELD_H

// $LICENSE_NEC_2003

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometry/point-locator.h"


#include "Utility/ref-ptr.h"

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
private:
  ref_ptr<geom_type const>     the_geom;
  ref_ptr<function_type const> f;
  // could be shared by many instances ...
  ref_ptr<point_locator_type>  loc;
public:
  geometric_field() {}
  geometric_field(geom_type const& geo,
		  function_type const& ff) : the_geom(geo), f(ff)
  {
    loc = ref_ptr<point_locator_type>(new point_locator_type(TheGrid(), TheGeometry()));
    loc->init();
  }

  result_type operator()(coord_type X) const 
  { 
    Cell c = loc->locate(X).TheCell();
    // std::cout << "X=" << X << " in cell " << c.handle() << std::endl;
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

  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}
  ref_ptr<grid_type const> TheGrid()     const { return ref_ptr<grid_type const>(the_geom->TheGrid());}
};



#endif
