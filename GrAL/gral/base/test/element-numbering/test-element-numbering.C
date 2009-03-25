

/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file 

    Test harness for element_numbering

*/

#include "Gral/Base/element-numbering.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include "Utility/pre-post-conditions.h"

template<class ELEM_NUM>
void check_element_numbering(ELEM_NUM const& num)
{
  typedef typename ELEM_NUM::element_type    element_type;
  typedef GrAL::element_traits<element_type> et;
  typename et::ElementIterator e(num.TheGrid());
  typename ELEM_NUM::value_type prev_num(num(*e));
  ++e;
  while(!e.IsDone()) {
    REQUIRE_ALWAYS( num(*e) == 1+prev_num, "num(*e)=" << num(*e) << " prev_num=" << prev_num,1);
    prev_num = num(*e);
    ++e;
  }
}


namespace GrAL {
  
  typedef grid_types<Complex2D> gt;
  template class element_numbering<gt::Vertex>;
  template class element_numbering<gt::Edge>;
  template class element_numbering<gt::Cell>;
  
}

int main() {

  using namespace GrAL;
  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;

  cartesian2d::CartesianGrid2D R(2,2);
  
  grid_type G;
  ConstructGrid0(G,R);

  element_numbering<gt::Vertex> vnum (G,0);
  element_numbering<gt::Edge>   ednum(G,0);
  element_numbering<gt::Cell>   cnum (G,0);

  check_element_numbering(vnum);
  check_element_numbering(ednum);
  check_element_numbering(cnum);
}
