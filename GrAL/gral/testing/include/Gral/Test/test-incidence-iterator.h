#ifndef NMWR_GB_GRAL_TEST_INCIDENCE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_INCIDENCE_ITERATOR_H

// $LICENSE

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
