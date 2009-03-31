#ifndef NMWR_GB_GRAL_TEST_SEQUENCE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_SEQUENCE_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
