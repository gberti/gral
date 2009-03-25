#ifndef GRAL_GB_SEQUENCE_COUNTING_ITERATOR_H
#define GRAL_GB_SEQUENCE_COUNTING_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iterator>

namespace GrAL {

  /*! \defgroup counting_iterator_grp Counting output iterator
      \ingroup helperfunctors
  */

  /*! \brief A simple output iterator counting the number of increments
 
      \ingroup counting_iterator_grp
    
      This class models STL "OutputIterator"
      and can be used in STL algorithms when only the size of a result
      sequence is of interest.
      Because of the missing assignment operator, 
      <a href="http://www.boost.org/libs/iterator/doc/counting_iterator.html" target="parent"> boost::counting_iterator </a>
      cannot be used in these cases.

      \see Tested in \ref test-counting-iterator.C

      \b Example:
      \code
      int a[] = { 1, 2, 3 };
      int b[] = { 3, 4 };
      // 3 different ways of accessing the output size:
      assert( 2 == * std::set_difference(a+0,a+3,  b+0, b+2, GrAL::counter(0)));
      assert( 2 ==   std::set_difference(a+0,a+3,  b+0, b+2, GrAL::counter(0)).size() );
      assert( 2 ==   std::set_difference(a+0,a+3,  b+0, b+2, GrAL::counter(0)).count());
      \endcode
   */ 
  template<class I = int>
  class counting_output_iterator  {
    typedef counting_output_iterator<I> self;
    I cnt;

    struct proxy_type {
      I value;
      proxy_type(I v) : value(v) {}

      //! \brief formal assigment (without effect)
      template<class T>
      proxy_type& operator=(T const&) { return *this;}
      
      operator I() const { return value;}
    };
  public:
    typedef I                        value_type;
    typedef proxy_type               reference;
    typedef I*                       pointer;
    typedef ptrdiff_t                difference_type;
    typedef size_t                   size_type;
    typedef std::output_iterator_tag iterator_category;
  public:
    //! \brief Construction 
    counting_output_iterator(I start = 0) : cnt(start) {}
    /*! \brief Convertible to value_type, giving the number of increments.
     
    Assignment to the result is a no-op, but must be syntactically allowed,
    as this class is used as output iterator.
    */
    proxy_type operator*() const { return proxy_type(cnt);}
    //! \brief preincrement
    self& operator++() { ++cnt; return *this;} 
    //! \brief postincrement
    self& operator++(int) { self res = *this; ++cnt; return res;}
 
    //! \brief number of increments
    I size()  const { return cnt;}
    //! \brief number of increments
    I count() const { return cnt;}
 };

  /*! \brief Creator function for counting_output_iterator

      \ingroup counting_iterator_grp
   */
  template<class I> 
  inline counting_output_iterator<I> counter(I start) { return counting_output_iterator<I>(start);}

};

#endif
