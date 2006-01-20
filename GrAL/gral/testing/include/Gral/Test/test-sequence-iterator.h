#ifndef NMWR_GB_GRAL_TEST_SEQUENCE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_SEQUENCE_ITERATOR_H

// $LICENSE

/*! \file  

    Test of sequence iterators 

*/

#include <iostream>

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-sequence-iterator.C"
#else

namespace GrAL {

  /*! \brief Test of sequence iterators
  
  \ingroup sequenceiteratortest
  */
  template<class E,class G>
  bool test_sequence_iterator(G const& g, std::ostream & out);

  /*! \brief Test of sequence iterators
  
  \ingroup sequenceiteratortest
  */
  template<class E,class G, class GT>
  bool test_sequence_iterator(G const& g, std::ostream & out, GT);

} // namespace GrAL 

#endif



#endif
