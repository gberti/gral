#ifndef GRAL_GB_GEOMETRY_GEOMETRIC_SHAPES_H
#define GRAL_GB_GEOMETRY_GEOMETRIC_SHAPES_H

// $LICENCE_NEC

#include <string>

/*! \brief Elementary geometric shapes with point inclusion and intersection tests. 

   \ingroup geomintersections 
*/
namespace geometric_shapes {

/*
template<class GEOM1, class GEOM2>
class intersection_geometry {
  ref_ptr<GEOM1 const> g1;
  ref_ptr<GEOM2 const> g2;
public:
  
  bool inside(coord_type  const& x) const { return g1->inside(x) && g2->inside(x);}
  
  //template<class POLYTOPE>
  //bool intersects(POLYTOPE const& p) const
  //{ Not so easy ... g1->intersects(p) && g2->intersects(p) does not suffice!! }
  
};
*/


struct geom_base {
  enum intersection_result { inside, outside, intersection};
  
  static std::string name(intersection_result r) {
    return (r == inside ? "inside" 
	    : (r == outside ? "outside" 
	       : "intersections"));
  }
};

template<class COORD>
class halfspace : public geom_base {
public:
  typedef COORD                            coord_type;
  typedef point_traits<coord_type>         pt;
  typedef algebraic_primitives<coord_type> ap;
  typedef typename pt::component_type      scalar;
private:
  coord_type normal;
  scalar     d;
public:
  halfspace(coord_type const& n,
	    scalar dd)
    :  normal(n), d(dd) {}

  bool is_inside(coord_type const& x) const
  { return ap::dot(x, normal) <= d;}

  /*
  template<class POLYTOPE>
  bool intersects(POLYTOPE const& p) const;
  */


  template<class CELL, class GEOM, class GT> //  = grid_types<typename CELL::grid_type> >
  intersection_result intersection_check(CELL const& c, GEOM const& geom) const {
    typedef element_traits<CELL> et;
    typedef typename et::element_tag cell_tag;
    typedef typename GT::incidence<vertex_type_tag, typename et::element_tag>::type VertexOnCellIterator;
    int inside_cnt = 0;
    for(VertexOnCellIterator vc(c); ! vc.IsDone(); ++vc)
      inside_cnt += (inside(geom.coord(*vc)) ? 1 : 0);
    return(inside_cnt == 0 ? : outside (inside_cnt == c.NumOfVertices() ? inside : intersection));    
  }

  template<class CELL, class GEOM, class GT> // = grid_types<typename CELL::grid_type> >
  bool intersects(CELL const& c, GEOM const& geom) const 
  { return intersection_check<GT>(c,geom) == intersection;}
  template<class CELL, class GEOM, class GT> // = grid_types<typename CELL::grid_type> >
  bool is_inside(CELL const& c, GEOM const& geom) const 
  { return intersection_check<GT>(c,geom) == inside;}
  template<class CELL, class GEOM, class GT> //  = grid_types<typename CELL::grid_type> >
  bool is_outside(CELL const& c, GEOM const& geom) const 
  { return intersection_check<GT>(c,geom) == outside;}
};


} // namespace geometric_shapes

#endif
