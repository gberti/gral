#ifndef NMWR_GB_RESTRICTED_GRID_FUNCTION
#define NMWR_GB_RESTRICTED_GRID_FUNCTION



// $LICENSE


#include "Gral/Base/grid-functions.h"

//----------------------------------------------------------------
//
// The class template 
//  restricted_grid_function<E,T,E_Iter>
// provides a mask for a grid_function<E,T>, 
// restricting it to the range of an element-iterator of type E_iter,
// for example the VertexIterator of a subrange.
// There is no copying involved, this class has reference semantics
// with respect to the underlying grid-function.
//
//----------------------------------------------------------------



//----------------------------------------------------------------
//
// restr_gf_iterator<E,T,E_iter>:
//
// Wrapper class for E_iter type: this has value-type T 
// (instead of E which is the value-type of E_iter).
// This class is used in restricted_grid_function to implement the
// container-level (STL-compliant) iterators over the stored items.
//
//----------------------------------------------------------------

template<class E, class T, class E_iter>
class restr_gf_iterator {
  typedef restr_gf_iterator<E,T,E_iter> self;
public:
  //------------------ construction -------------------

  restr_gf_iterator() {}
  restr_gf_iterator(E_iter start,  grid_function<E,T>& GF) 
    : it(start), gf(&GF) {}

  //------------------ iteration --- -------------------

  typedef T value_type;

  self& operator++()    { ++it; return *this;}
  //  self  operator++(int) { self tmp(*this); ++(*this); return tmp;}
  T&    operator*() const  { return (*gf)[*it];}
  //T& operator*() const  { return (*gf)[it.GlobalNumber()];} // more eff. than (*gf)[*it]
  // not yet defined for arbitrary iterators!
  //  bool IsDone()  const  { return it.IsDone();}
  //  bool IsDone()  const  { return it == stop;}

  //------------------ comparision ---------------------

  friend bool operator== <> (const self& ls, const self& rs);
  friend bool operator<  <> (const self& ls, const self& rs);
private:
  //------- DATA ---------
  E_iter it;
  grid_function<E,T>* gf;
};


template<class E, class T, class E_iter>
inline bool operator==(restr_gf_iterator<E,T,E_iter> const& ls,
		       restr_gf_iterator<E,T,E_iter> const& rs)
 { return (ls.it == rs.it);}

template<class E, class T, class E_iter>
inline bool operator< (restr_gf_iterator<E,T,E_iter> const& ls,
		       restr_gf_iterator<E,T,E_iter> const& rs)
 { return (ls.it <  rs.it);}

//----------------------------------------------------------------


// restrict a grid function to the range [b,e)  of an iterator
// (see above)
template<class E, class T, class E_iter>
class restricted_grid_function {
public:
  typedef unsigned size_type;
  restricted_grid_function(grid_function<E,T>& GF,
			   E_iter b, E_iter e, // [b,e)
			   size_type s)  // s = distance(b,e)
    : gf(&GF), start(b), stop(e), sz(s) {}

  typedef restr_gf_iterator<E,T,E_iter> iterator;
  //  class const_iterator;

  iterator begin() const { return iterator(start,*gf); } // !! non-const iterator !!
  iterator end()   const { return iterator(stop ,*gf); }
  size_type size() const { return sz;}
private:
  grid_function<E,T>* gf;
  E_iter start;
  E_iter stop;
  size_type sz;
};


template<class E, class T, class ERange>
inline 
restricted_grid_function<E,T, typename ERange::const_iterator>
Restriction(grid_function<E,T>& gf, const ERange& R)
{ 
  typedef typename ERange::const_iterator e_iter;  
  //typedef typename ERange::ElementIterator e_iter;

  return restricted_grid_function<E,T,e_iter>(gf,R.begin(),R.end(),R.size());
}

#endif
