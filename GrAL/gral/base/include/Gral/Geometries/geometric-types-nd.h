#ifndef GRAL_BASE_GB_GEOMETRIC_TEMPLATE_TYPES_H
#define GRAL_BASE_GB_GEOMETRIC_TEMPLATE_TYPES_H

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-grid.h"

#include "Geometry/intersections.h"

#include <vector>

namespace detail {


template<class GEOM, class GT, class E, int DIM>
class polytope_aux_dim : public  polytope_aux_dim<GEOM,GT,E,DIM-1> 
{
public:
  polytope_aux_dim() {}
  polytope_aux_dim(GEOM const&         geo, E const& e) :  polytope_aux_dim<GEOM,GT,E,DIM-1> (geo,e) {}
  polytope_aux_dim(ref_ptr<GEOM const> geo, E const& e) :  polytope_aux_dim<GEOM,GT,E,DIM-1> (geo,e) {}
};


template<class GEOM, class GT, class E>
class polytope_aux_dim<GEOM,GT,E,0> : public element_grid<E> 
{
private:
  typedef element_grid<E> base;
public:
  typedef GEOM  geom_type;
  typedef typename GEOM::coord_type coord_type;
  typedef E                 element_type;

private:
  ref_ptr<geom_type const> geom;
public:
  polytope_aux_dim() {}
  polytope_aux_dim(geom_type const&         geo, element_type const& ee) : base(ee), geom(geo) {}
  polytope_aux_dim(ref_ptr<geom_type const> geo, element_type const& ee) : base(ee), geom(geo) {}

  ref_ptr<geom_type const> TheGeom() const { return geom;}
  coord_type operator()(typename base::Vertex const& v) const { return TheGeom()->coord(v);}
};



template<class GEOM, class GT, class E>
class polytope_aux_dim<GEOM,GT,E,1> : public polytope_aux_dim<GEOM,GT,E,0> {
  typedef polytope_aux_dim<GEOM,GT,E,0> base;
public:
  polytope_aux_dim() {}
  polytope_aux_dim(GEOM const&         geo, E const& ee) : base(geo,ee) {}
  polytope_aux_dim(ref_ptr<GEOM const> geo, E const& ee) : base(geo,ee) {}

  typename base::coord_type direction(typename base::Edge ed) const 
  { 
    typename base::VertexOnEdgeIterator v1(ed); // .FirstVertex();
    typename base::VertexOnEdgeIterator v2 = v1; ++v2;
    return TheGeom()->coord(*v1)  - TheGeom()->coord(*v2);
    //return TheGeom()->coord(ed.V1()) - TheGeom()->coord(ed.V2());
  }
  std::vector<typename base::coord_type> directions(typename base::Edge ed) const 
  { return std::vector<typename base::coord_type>(1,direction(ed));}
};


template<class GEOM, class GT, class E>
class polytope_aux_dim<GEOM,GT,E,2> : public polytope_aux_dim<GEOM,GT,E,1> {
  typedef polytope_aux_dim<GEOM,GT,E,1> base;
public:
  polytope_aux_dim() {}
  polytope_aux_dim(GEOM const&         geo, E const& ee) : base(geo,ee) {}
  polytope_aux_dim(ref_ptr<GEOM const> geo, E const& ee) : base(geo,ee) {}

  using base::directions;

  std::vector<typename base::coord_type> directions(typename base::Face f)  const { 
    std::vector<typename base::coord_type> res(2);
    // typename base::EdgeIterator e = f.FirstEdge();
    res[0] = direction(*f.FirstEdge());
    //FIXME: Not OK: could be parallel, e.g. cartesiannd::grid<N>!
    res[1] = direction(*(++(f.FirstEdge())));
    return res;
  }

};




/*! \brief Mapping of combinatorial grid elements to geometric entities

   \todo Polytopes should give access to the underlying grid, i.e. the archetype
    of the anchor element.
*/
template<class GEOM, class GT, class E>
class polytope_aux : public polytope_aux_dim<GEOM,GT,E, element_traits<E>::dim> // element_dimension_tag::dim>
{

  typedef polytope_aux_dim<GEOM,GT,E, element_traits<E>::dim> // element_dimension_tag::dim>  
                            base;
 public:
  typedef GEOM              geom_type;
  typedef GT                gt;
  typedef element_traits<E> et;
  typedef E                 element_type;
  typedef typename GEOM::coord_type coord_type;

  polytope_aux() {}
  polytope_aux(geom_type const&         geo, element_type const& ee) : base(geo,ee) {}
  polytope_aux(ref_ptr<geom_type const> geo, element_type const& ee) : base(geo,ee) {}

  // must use specialization for runtime-fixed dimension
  //enum {  space_dim = geom_type::space_dim, dim = et::dimension_tag::dim };
  int space_dimension() const { return TheGeom()->space_dimension();}
};



// directions


template<class GEOM, class GT, class E, int DIM>
class polytope_directions_aux : public polytope_directions_aux<GEOM,GT,E,DIM-1> 
{ 
  typedef polytope_directions_aux<GEOM,GT,E,DIM-1> base;
  typedef polytope_aux<GEOM, GT, E>                object_type;
 public:
  polytope_directions_aux(object_type const& obj) : base(obj) 
    {
      /*
      for(ot::sequence_iterator<DIM>::type elem = obj.FirstElement(DIM); ! elem.IsDone(); ++elem)
	dirs[DIM].push_back(obj.directions(*elem));
      */
    }
};


template<class GEOM, class GT, class E>
class polytope_directions_aux<GEOM,GT,E,0> 
  : public polytope_directions_base<polytope_aux<GEOM, GT, E> >
{
public:
  typedef polytope_aux<GEOM, GT, E>     object_type;
  typedef polytope_directions_base<object_type> base;
  polytope_directions_aux(object_type const& obj) : base(obj) {}
};


template<class GEOM, class GT, class E>
class polytope_directions_aux<GEOM,GT, E,1>  : public polytope_directions_aux<GEOM,GT,E,0>
{
public:
  typedef polytope_aux<GEOM, GT, E>      object_type;
  typedef polytope_directions_aux<GEOM,GT,E,0>   base;
  typedef object_type                            ot;

  polytope_directions_aux(object_type const& obj) : base(obj) 
    {
      for(typename ot::EdgeIterator e = obj.FirstEdge(); ! e.IsDone(); ++e) {
	dirs[1].push_back(obj.directions(*e));
      }
    }
};

template<class GEOM, class GT, class E>
class polytope_directions_aux<GEOM,GT, E,2>  : public polytope_directions_aux<GEOM,GT,E,1>
{
public:
  typedef polytope_aux<GEOM, GT, E>      object_type;
  typedef polytope_directions_aux<GEOM,GT,E,1>   base;
  typedef object_type                            ot;

  polytope_directions_aux(object_type const& obj) : base(obj) 
    {
      for(typename ot::FaceIterator f = obj.FirstFace(); ! f.IsDone(); ++f) {
	dirs[2].push_back(obj.directions(*f));
      }
    }
};


} // namespace detail



/*

template<class GEOM, class GT = grid_types<typename GEOM::grid_type> >
struct geometric_grid_types {
  
  template<class E>
  struct polytope //: public detail::polytope_aux<GEOM, GT, E>
  {
    typedef detail::polytope_aux<GEOM, GT, E> type;
    polytope() {}
    polytope(GEOM const&         geom, E const& e) : detail::polytope_aux<GEOM, GT, E>(geom,e) {}
    polytope(ref_ptr<GEOM const> geom, E const& e) : detail::polytope_aux<GEOM, GT, E>(geom,e) {}
  };
};
*/


/*! \brief Map combinatorial grid elements to geometric entities (polytopes)
    
    The name polytope is justified only for linear geometries!
*/

template<class GEOM, class E, class GT = grid_types<typename GEOM::grid_type> >
class polytope : public detail::polytope_aux<GEOM, GT, E>
{
public:
    polytope() {}
    polytope(GEOM const&         geom, E const& e) : detail::polytope_aux<GEOM, GT, E>(geom,e) {}
    polytope(ref_ptr<GEOM const> geom, E const& e) : detail::polytope_aux<GEOM, GT, E>(geom,e) {}
};


template<class OBJ>
class polytope_directions;




template<class GEOM, class GT, class E>
class polytope_directions<polytope<GEOM,E,GT> > // detail::polytope_aux<GEOM, GT, E> > 
  : public  detail::polytope_directions_aux<GEOM,GT, E, element_traits<E>::dim> //element_dimension_tag::dim>
{
  //typedef detail::polytope_aux<GEOM, GT, E>     object_type;
  typedef polytope<GEOM, E, GT>     object_type;
  typedef object_type               ot;
  typedef detail::polytope_directions_aux<GEOM,GT, E, element_traits<E>::dim> // element_dimension_tag::dim>
                                                base;
  typedef typename base::coord_type coord_type;
  typedef algebraic_primitives<coord_type> ap;
  typedef typename base::dir_sequence dir_sequence;
public:

  polytope_directions(object_type const& obj) : base(obj) 
    {
      // if the polytope obj is not full dimensional, we need to add more directions:
      // We extrude it into some independent direction to make it  full-dimensional
      if(obj.dimension() > 0 && obj.space_dimension() > obj.dimension()) {
	if(obj.space_dimension() == 1+obj.dimension()) {
	  //   add 1 normal dir. n to edge dirs
	  dirs[1].push_back(ap::basis_completion(dirs[obj.dimension()][0]));
	  if(obj.space_dimension() > 2) {
	    unsigned oldsz = dirs[2].size();
	    dirs[2].resize(dirs[2].size() + dirs[1].size()); 
	    for(unsigned d = 0; d < dirs[1].size(); ++d) {
	      // add each pair (n,d) to face dirs
	      dir_sequence face(2);
	      dirs[2][d+oldsz] = face;//dir_sequence(2);
	      dirs[2][d+oldsz][0] = dirs[1].back()[0];
	      dirs[2][d+oldsz][1] = dirs[1][d][0];
	    }
	    }
	}
	else if (obj.space_dimension() == 2+obj.dimension()) {
	  //   add 2 normal dirs n1, n2 to edge dirs
	  typename base::dir_sequence n = ap::basis_completion(dirs[obj.dimension()][0]);
	  dirs[1].push_back(dir_sequence(1,n[0]));
	  dirs[1].push_back(dir_sequence(1,n[1]));
	  //   add dirs (n1,n2) and (ni,ej) for i = 1,2 and every edge direction ej to face dirs
	  dirs[2].push_back(n);
	  for(unsigned i = 0; i <= 1; ++i)
	    for(unsigned j = 0; j < dirs[1].size()-2;  ++j) {
	      dirs[2].push_back(dir_sequence(2));
	      dirs[2].back()[0]=n[i];
	      dirs[2].back()[1]=dirs[1][i][0];
	    }
	}
      }
    }
};


#endif
