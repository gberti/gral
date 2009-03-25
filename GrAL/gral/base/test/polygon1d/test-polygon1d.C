

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
  Test code for polygon1d::polygon

*/

#include "Gral/Base/polygon.h"
#include "Gral/Test/all.h"

#include "Utility/pre-post-conditions.h"
#include "Geometry/coords.h"
#include <iostream>

template<class GRID1D>
void test_switch(GRID1D const& G, std::ostream& out)
{
  typedef GrAL::grid_types<GRID1D> gt;
  typename gt::Cell   c(* G.FirstCell()); 
  typename gt::Vertex v(* c.FirstVertex());
  typename gt::Vertex v_first = v;

  do{
    out << "  ->  ";
    out << "(v=" << v.handle() << " c=" << c.handle() << ") ";
    G.switch_vertex(v,c);
    out << "  ->  ";
    out << "(v=" << v.handle() << " c=" << c.handle() << ") ";
    G.switch_cell  (v,c);
  }
  while(v != v_first);
  out << std::endl;
}


int main() {
  using namespace GrAL;
  using namespace std;

  typedef grid_types<polygon1d::polygon> gt;
  typedef coordN<2> coord_type;
  typedef coord_type ct;

  polygon1d::polygon p(3);

  ct coords[3] = { ct(0,0), ct(1,0), ct(0,1) };
  polygon1d::geometry<coord_type> geom(p, coords, coords+sizeof(coords)/sizeof(coord_type));


  ostream* out = &cout;
  test_vertex_iterator(p, *out);
  test_cell_iterator  (p, *out);
  test_vertex_on_cell_iterator(p, *out);
  test_facet_on_cell_iterator (p, *out);

  test_vertex_on_vertex_iterator(p, *out);
  test_cell_on_vertex_iterator  (p, *out);

  test_grid_functions<gt::Vertex>(p, *out);
  test_grid_functions<gt::Cell>  (p, *out);

  test_archetypes(p, *out);


  test_sequence_iterator<gt::Vertex>(p, *out);
  test_sequence_iterator<gt::Cell>  (p, *out);

  test_incidence_iterator<gt::Vertex, gt::Cell  >(p, *out);
  test_incidence_iterator<gt::Cell,   gt::Vertex>(p, *out);

  checkgt<gt>(*out);

  test_switch(p, *out);

  {
    polygon1d::polygon p2;
    vertex_morphism<polygon1d::polygon, polygon1d::polygon> v_corr(p,p2);
    cell_morphism  <polygon1d::polygon, polygon1d::polygon> c_corr(p,p2);
    ConstructGrid0(p2,p, v_corr, c_corr);
    
    {
      gt::VertexIterator v_p  = p .FirstVertex();
      gt::VertexIterator v_p2 = p2.FirstVertex();
      for( ; !v_p.IsDone(); ++v_p, ++v_p2)
	REQUIRE_ALWAYS( (v_corr[v_p.handle()] == v_corr[v_p2.handle()]), "", 1);
    }
    {
      gt::CellIterator v_p  = p .FirstCell();
      gt::CellIterator v_p2 = p2.FirstCell();
      for( ; !v_p.IsDone(); ++v_p, ++v_p2)
	REQUIRE_ALWAYS( (c_corr[v_p.handle()] == c_corr[v_p2.handle()]), "", 1);
    }
  }

  {
    polygon1d::polygon p2;
    vertex_morphism<polygon1d::polygon, polygon1d::polygon> v_corr(p,p2);
    ConstructGrid0(p2,p, v_corr);
    
    {
      gt::VertexIterator v_p  = p .FirstVertex();
      gt::VertexIterator v_p2 = p2.FirstVertex();
      for( ; !v_p.IsDone(); ++v_p, ++v_p2)
	REQUIRE_ALWAYS( (v_corr[v_p.handle()] == v_corr[v_p2.handle()]), "", 1);
    }
  }

  {
    polygon1d::polygon p2;
    ConstructGrid0(p2,p);
  }

 

}
