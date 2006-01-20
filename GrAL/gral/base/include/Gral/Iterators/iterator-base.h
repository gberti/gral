#ifndef GRAL_GB_BASE_ITERATORS_ITERATOR_BASE_H
#define GRAL_GB_BASE_ITERATORS_ITERATOR_BASE_H

// $LICENSE_NEC_2006

#include "Gral/Base/common-grid-basics.h"
#include "Utility/ref-ptr.h"

namespace GrAL {

  /*! \brief Base class for sequence iterators to help standard conformance

      \ingroup iterators
   
  */
  template<class GT, class ETAG> class sequence_iter_base  {
  public:
    typedef GT                               gt;
    typedef std::forward_iterator_tag        iterator_category;
    typedef typename gt::size_type           size_type;
    // This must be a signed type! Normally, gt::size_type is also signed,
    // but we could check.
    typedef typename gt::size_type           difference_type;
    typedef typename element<gt,ETAG>::type  value_type;
    typedef typename sequence_iterator<gt,ETAG>::type iterator;
    typedef value_type                       reference;
    typedef value_type*                      pointer;

    temporary<value_type>  operator->() const { return temporary<value_type>(static_cast<iterator const&>(*this).operator*());}
  };

  /*! \brief Base class for incidence iterators to help standard conformance
  
      \ingroup iterators
 
      Usage:
      \code
      #include "Gral/Iterators/iterator-base.h"

      class my_grid;
      class my_vertex_on_cell_iter;
      
      grid_types_my_grid { 
        typedef  my_vertex_on_cell_iter VertexOnCellIterator;
        ...
      };
      

      class my_vertex_on_cell_iter 
        : public incidence_iter_base<grid_types_my_grid,
                                     vertex_type_tag,
                                     cell_type_tag>
      { ... };
      \endcode
  */
  template<class GT, class ETAG, class ATAG> class incidence_iter_base  {
  public:
    typedef GT                               gt;
    typedef std::forward_iterator_tag        iterator_category;
    typedef typename gt::size_type           size_type;
    // This must be a signed type! Normally, gt::size_type is also signed,
    // but we could check.
    typedef typename gt::size_type           difference_type;
    typedef typename element<gt,ETAG>::type  value_type;
    typedef typename incidence_iterator<gt,ETAG,ATAG>::type iterator;
    typedef value_type                       reference;
    typedef value_type*                      pointer;

    temporary<value_type>  operator->() const { return temporary<value_type>(static_cast<iterator const&>(*this).operator*());}
  };




} // namespace GrAL

#endif
