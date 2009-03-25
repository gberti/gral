#ifndef NMWR_GB_GRAL_TEST_INCIDENCE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_INCIDENCE_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of incidence iterators 
*/

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-incidence-iterator.C"
#else

namespace GrAL {

/*! \brief Test a vertex-on-cell iterator
 
    Test a model of $GrAL VertexOnCellIterator

    \ingroup incidenceiteratortests
*/
  template<class E, class A, class G>
  bool test_incidence_iterator(G const& g, std::ostream & out);
  
  /*! \brief Test an incidence  iterator
    
  Test a model of $GrAL VertexOnCellIterator
  
  \ingroup incidenceiteratortests
  */
  template<class E, class A, class G, class GT>
  bool test_incidence_iterator(G const& g, std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
