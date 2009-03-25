/*! \file */

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/wrap-elements.h"
#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Base/partial-grid-function-hash.h"
#include "Gral/Base/archetypes-from-base.h"

#include "Gral/Test/all.h"

#include <boost/static_assert.hpp>
#include <iostream>

namespace GrAL {

  template<class G, class GT>
  class wrap_grid;
  
  template<class G, class GT>
  struct grid_types_wg_base1 : 
    public archetypes_from_base_grid_types<GT>,
    public grid_types_detail::grid_types_root
  {
    typedef GT   bgt;
    typedef typename bgt::grid_type      base_grid_type;
    typedef wrap_grid<G,GT>              grid_type;
    typedef typename bgt::size_type      size_type;
    
    typedef typename bgt::dimension_tag  dimension_tag;
    
    typedef typename bgt::vertex_handle vertex_handle;
    typedef typename bgt::cell_handle   cell_handle;
    typedef wrapped_element<grid_type, typename bgt::Vertex, vertex_handle>  Vertex;
    typedef wrapped_element<grid_type, typename bgt::Cell,   cell_handle  >  Cell;
    
    //  typedef wrapped_element<grid_type, typename bgt::Face,   typename bgt::face_handle  >  Face;
    typedef wrapped_sequence_iterator<grid_type, Vertex, vertex_handle, 
				      typename bgt::VertexIterator>          VertexIterator;
    typedef wrapped_sequence_iterator<grid_type, Cell,   cell_handle, 
				      typename bgt::CellIterator>            CellIterator;

};


template<class G, class GT>
struct grid_types_wg_base : 
    public wrap_all_downward_incidence_iterator_types<grid_types_wg_base1<G,GT>, GT>
{
  typedef GT   bgt;
  typedef wrap_all_downward_incidence_iterator_types<grid_types_wg_base1<G,GT>, GT> base;
  using base::grid_type;
  typedef wrapped_sequence_iterator<typename base::grid_type, 
				    typename base::Edge,   
				    typename base::edge_handle, 
				    typename bgt::EdgeIterator>            EdgeIterator;
};

template<class G, class GT>
struct grid_types<wrap_grid<G,GT> > :
  public grid_types_base< grid_types_wg_base<G,GT> > 
{};


template<class G, class GT>
class wrap_grid 
{ 
public:
  typedef wrap_grid<G, GT>             self;
  typedef self                         grid_type;
  typedef cartesian2d::CartesianGrid2D base_grid_type;
  typedef grid_types<base_grid_type>   bgt;
  typedef grid_types<self>             gt;
  typedef typename base_grid_type::category category;
private:
  base_grid_type const* bg;
public:
  wrap_grid(base_grid_type const& bbg) : bg(&bbg) {}
  
  base_grid_type const& BaseGrid()  const { return *bg;}
  operator base_grid_type const& () const { return *bg;}

  typename gt::VertexIterator FirstVertex() const { return typename gt::VertexIterator(*this);}
  typename gt::CellIterator   FirstCell()   const { return typename gt::CellIterator  (*this);}
  //  typename gt::EdgeIterator   FirstEdge()   const { return typename gt::EdgeIterator  (*this);}
};

}

int main() {
  using namespace GrAL;
  using namespace std;

  typedef cartesian2d::CartesianGrid2D grid_type; 
  typedef grid_types<grid_type>        gt;
  typedef wrap_grid<grid_type, gt>     wrap_grid_type;
  typedef grid_types<wrap_grid_type>   wgt;

  grid_type      R(2,2);
  wrap_grid_type W(R);



  cout << "checkgt<wgt>\n";
  checkgt<wgt>(cout);

  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_element_category, 
			wgt::categories<wgt::Vertex>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_element_category, 
		       wgt::categories<wgt::Edge>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_element_category, 
			wgt::categories<wgt::Cell>::type>::value));
  BOOST_STATIC_ASSERT((1== is_base_or_same<grid_sequence_iterator_category,
		                           wgt::categories<wgt::VertexIterator>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_sequence_iterator_category,
		                           wgt::categories<wgt::EdgeIterator>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_sequence_iterator_category,
		                           wgt::categories<wgt::CellIterator>::type>::value));
  
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_incidence_iterator_category, 
					    wgt::categories<wgt::VertexOnEdgeIterator>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_incidence_iterator_category, 
			                    wgt::categories<wgt::VertexOnFaceIterator>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_incidence_iterator_category, 
					   wgt::categories<wgt::VertexOnFacetIterator>::type>::value));
  BOOST_STATIC_ASSERT( (1== is_base_or_same<grid_incidence_iterator_category, 
			                    wgt::categories<wgt::VertexOnCellIterator>::type>::value));

  BOOST_STATIC_ASSERT( (1== boost::is_same<wgt::VertexOnEdgeIterator,wgt::VertexOnFacetIterator>::value));
  


  wgt::Cell c(W,  * (W.BaseGrid().FirstCell()));
  

  wgt::VertexOnCellIterator vc(c);
  wgt::Vertex v(*vc);
  wgt::vertex_handle vh = vc.handle();
  vh = v.handle();
  wgt::Vertex v2(W, vh);
 
  wgt::CellIterator cc = W.FirstCell();
  wgt::CellIterator cc1(W);
  wgt::CellIterator cc2(W, R.FirstCell());


  BOOST_STATIC_ASSERT( (1== boost::is_same<wgt::CellIterator, GrAL::iterator<wgt::Cell, wgt::grid_type>::type>::value));
 

  test_sequence_iterator <wgt::Vertex>           (W, cout);
  test_sequence_iterator <wgt::Edge>             (W, cout);
  test_sequence_iterator <wgt::Cell>             (W, cout);
  test_incidence_iterator<wgt::Vertex, wgt::Edge>(W, cout);
  test_incidence_iterator<wgt::Vertex, wgt::Cell>(W, cout);
  test_incidence_iterator<wgt::Edge,   wgt::Cell>(W, cout);
  // test_incidence_iterator<wgt::Cell,   wgt::Cell>(W, cout);
    
  GrAL::partial_grid_function<wgt::Edge, int> pge(W);
  GrAL::grid_function<wgt::Edge, int>         tge(W,0);
  wgt::EdgeOnCellIterator ec(*cc);
  tge[*ec] = 1;

}
