#ifndef GRAL_GB_SEQUENCE_UNION_FIND_H
#define GRAL_GB_SEQUENCE_UNION_FIND_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/bijective-mapping.h"
#include "Container/partial-mapping.h"

namespace GrAL {

  /*! \brief union_find data structure

   This is an implementation of the well-know
  <a href="http://en.wikipedia.org/wiki/Disjoint-set_data_structure">union-find</a> data structure.
  The implementation uses a hash table to store sets.

  \note The \c union operation is called \c join (as union is a reserved word).
  \note The standard make_set(x) operation is implicit: Each value of type \c T not yet encountered is
   regarded a singleton.

  \templateparams
    - T: element type of sets

    Tested in \ref test-union-find.C
  */
  template<class T>
  class union_find {
    mutable bijective_mapping<T,T,identity_if_undefined> labels;
    partial_mapping<T,int>                               sz;
  public:
    typedef typename partial_mapping<T,int>::const_iterator const_iterator;
    typedef typename partial_mapping<T,int>::size_type      size_type;

    //! Iterator over \e explicitely defined sets
    const_iterator begin() const { return sz.begin();}
    const_iterator end()   const { return sz.end();}
    size_type      size()  const { return sz.size();}

    union_find() : sz(1) {}

    //! \brief Equivalent to find(t)
    T operator()(T const& t) const { return find(t);}

    /*! \brief Find the set (representant) of \c t
     */
    T find(T const& t) const { 
      if(t != labels(t))
	labels[t] = find(labels(t));
      return labels(t);
    }
    /*! \brief Join the sets of \c t1 and \c t2

       This is called union in the standard nomenclature.
     */
    void join(T const& t1, T const& t2) {
      link(find(t1), find(t2));
    }

    /*! \brief Return the size of the set of \c t
     */
    int size(T const& t) const { return sz(find(t)); }

  private:
    void link(T const& t1, T const& t2) {
      if(t1 != t2) {
	if(sz(t1) < sz(t2)) {
	  labels[t1] = t2;
	  sz[t2] += sz[t1];
	  sz.undefine(t1);
	}
	else {
	  labels[t2] = t1;
	  sz[t1] += sz[t2];
	  sz.undefine(t2);
	}
      }
    }
  };

} // namespace GrAL

#endif
