#ifndef GRAL_GB_TEST_SWITCH_H
#define GRAL_GB_TEST_SWITCH_H



/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

  /*! \brief Test the switched_vertex / switch_vertex functions
  
      \ingroup switchtests;
  */
  template<class GRID>
  void test_switch_vertex(GRID const& g)
  {
    typedef grid_types<GRID> gt;
    for(typename gt::EdgeIterator e(g); !e.IsDone(); ++e) {
      for(typename gt::VertexOnEdgeIterator ve(*e); !ve.IsDone(); ++ve) {
	typename gt::Vertex v1 = *ve;
	typename gt::Vertex v2 = g.switched_vertex(v1,*e);
	REQUIRE_ALWAYS(v1 != v2, "", 1);
	typename gt::Vertex v3 = g.switched_vertex(v2,*e);
	REQUIRE_ALWAYS(v3 == v1, "", 1);
      }
    }
  }

  /*! \brief Test the switched_edge / switch_edge functions
  
      \ingroup switchtests;
  */
  template<class GRID>
  void test_switch_edge(GRID const& g)
  {
    typedef grid_types<GRID> gt;
    for(typename gt::FaceIterator f(g); !f.IsDone(); ++f) {
      for(typename gt::EdgeOnFaceIterator ef(*f); !ef.IsDone(); ++ef) {
	for(typename gt::VertexOnEdgeIterator vef(*ef); !vef.IsDone(); ++vef) {
	  typename gt::Vertex v  = *vef;
	  typename gt::Edge   e1 = g.switched_edge(v,*ef,*f);
	  REQUIRE_ALWAYS(e1 != *ef, "", 1);
	  typename gt::Edge   e2 = g.switched_edge(v,e1,*f);
	  REQUIRE_ALWAYS(e2 == *ef, "", 1);
	}
      }
    }
  }


  /*! \brief Test the switched_facet / switch_facet functions
  
      \ingroup switchtests;
  */
  template<class GRID>
  void test_switch_facet(GRID const& g)
  {
    typedef grid_types<GRID> gt;
    for(typename gt::CellIterator c(g); !c.IsDone(); ++c) {
      for(typename gt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc) {
	for(typename gt::EdgeOnFacetIterator efc(*fc); !efc.IsDone(); ++efc) {
	  typename gt::Edge   e= *efc;
	  typename gt::Facet f1 = g.switched_facet(e,*fc,*c);
	  REQUIRE_ALWAYS(f1 != *fc, "", 1);
	  typename gt::Facet   f2 = g.switched_facet(e,f1,*c);
	  REQUIRE_ALWAYS(f2 == *fc, "", 1);
	}
      }
    }
  }


  /*! \brief Test the switched_cell / switch_cell functions
    
      \ingroup switchtests;
  */
  template<class GRID>
  void test_switch_cell(GRID const& g)
  {
    typedef grid_types<GRID> gt;
    for(typename gt::CellIterator c(g); !c.IsDone(); ++c) {
      for(typename gt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc) {
	typename gt::Cell c1 = g.switched_cell(*fc,*c);
	REQUIRE_ALWAYS(c1 != *c, "", 1);
	typename gt::Cell c2 = g.switched_cell(*fc,c1);
	REQUIRE_ALWAYS(c2 == *c, "", 1);
      }
    }
  }

} // namespace GrAL

#endif
