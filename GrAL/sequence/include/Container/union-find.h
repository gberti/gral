#ifndef GRAL_GB_SEQUENCE_UNION_FIND_H
#define GRAL_GB_SEQUENCE_UNION_FIND_H

// $LICENSE_NEC_2005

#include "Container/bijective-mapping.h"
#include "Container/partial-mapping.h"

namespace GrAL {

  /*! \brief union_find data structure

  \templateparams
    - T: element type of sets

    Tested in \ref test-union-find.C
  */
  template<class T>
  class union_find {
    mutable bijective_mapping<T,T,identity_if_undefined> labels;
    partial_mapping<T,int>                       sz;
  public:
    typedef typename partial_mapping<T,int>::const_iterator const_iterator;
    typedef typename partial_mapping<T,int>::size_type      size_type;

    //! Iterator over explicitely defined sets
    const_iterator begin() const { return sz.begin();}
    const_iterator end()   const { return sz.end();}
    size_type      size()  const { return sz.size();}

    union_find() : sz(1) {}

    T operator()(T const& t) const { return find(t);}
    T find(T const& t) const { 
      if(t != labels(t))
	labels[t] = find(labels(t));
      return labels(t);
    }
    void join(T const& t1, T const& t2) {
      link(find(t1), find(t2));
    }
    int size(T const& t) { return sz(find(t)); }

  private:
    void link(T const& t1, T const& t2) {
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
  };

} // namespace GrAL

#endif
