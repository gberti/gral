#ifndef NMWR_GB_GRAL_TEST_FACET_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACET_ITERATOR_H

// $LICENSE

/*! \file  

    Test of facet iterators (models of $GrAL FacetIterator).

*/

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-facet-iterator.C"

#else

namespace GrAL {

/*! \brief Test of facet iterators

    Models of $GrAL FacetIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G>
bool test_facet_iterator(G const& g, ::std::ostream & out);

/*! \brief Test of facet iterators

    Models of $GrAL FacetIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT>
bool test_facet_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
