#ifndef NMWR_GB_RESTRICTED_GRID_FUNCTION_ADAPTER_H
#define NMWR_GB_RESTRICTED_GRID_FUNCTION_ADAPTER_H



// $LICENSE


#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

/*! \defgroup restrictedgridfunctions Restrictions of grid functions
   \ingroup gridfunctions  
*/

template<class GF, class E_iter>
class restr_gf_iterator_adapter;

template<class GF, class E_iter>
inline bool operator==(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs);
template<class GF, class E_iter>
inline bool operator!=(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs);
template<class GF, class E_iter>
inline bool operator< (restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs);



//----------------------------------------------------------------
//----------------------------------------------------------------

/*! \internal
    \brief Wrapper class for E_iter type: Has value-type of GF   instead of E_iter
    \ingroup restrictedgridfunctions
 
   This class is used in restricted_grid_function_adapter to implement the
  container-level (STL-compliant) iterators over the stored items.
*/

template<class GF, class E_iter>
class restr_gf_iterator_adapter {
  typedef restr_gf_iterator_adapter<GF,E_iter> self;
public:
  //------------------ construction -------------------

  restr_gf_iterator_adapter() {}
  restr_gf_iterator_adapter(E_iter start,  GF& f) 
    : it(start), gf(&f) {}

  //------------------ iteration --- -------------------

  typedef typename GF::value_type value_type;

  self& operator++()    { ++it; return *this;}
  value_type&    operator*() const  { return (*gf)[*it];}
  //T& operator*() const  { return (*gf)[it.GlobalNumber()];} // more eff. than (*gf)[*it]
  // not yet defined for arbitrary iterators!
  //  bool IsDone()  const  { return it.IsDone();}
  //  bool IsDone()  const  { return it == stop;}

  //------------------ comparision ---------------------

  friend bool operator== <> (const self& ls, const self& rs);
  friend bool operator!= <> (const self& ls, const self& rs);
  friend bool operator<  <> (const self& ls, const self& rs);
private:
  //------- DATA ---------
  E_iter it;
  GF *   gf;
};


template<class GF, class E_iter>
inline bool operator==(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs)
 { return (ls.it == rs.it);}

template<class GF, class E_iter>
inline bool operator!=(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs)
 { return (ls.it != rs.it);}

template<class GF, class E_iter>
inline bool operator< (restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs)
 { return (ls.it <  rs.it);}


//----------------------------------------------------------------
/*! \brief Restricts a grid function of type GF to the range of an 
    element-iterator
   \ingroup restrictedgridfunctions  
 
   For example, a grid function may be the ElementIterator of a subrange.
   There is no copying involved, this class has reference semantics
   with respect to the underlying grid function.
*/
//----------------------------------------------------------------


template<class GF, class E_iter>
class restricted_grid_function_adapter {
public:
  typedef unsigned size_type;
  restricted_grid_function_adapter(GF& f,
				   E_iter b, E_iter e, // [b,e)
				   size_type s)  // s = distance(b,e)
    : gf(&f), start(b), stop(e), sz(s) {}

  typedef restr_gf_iterator_adapter<GF,E_iter> iterator;
  //  class const_iterator;

  iterator begin() const { return iterator(start,*gf); } // !! non-const iterator !!
  iterator end()   const { return iterator(stop ,*gf); }
  size_type size() const { return sz;}
private:
  GF*       gf;
  E_iter    start;
  E_iter    stop;
  size_type sz;
};

/*! \brief Creator function for restricted_grid_function_adapter.

   \ingroup restrictedgridfunctions  
*/

template<class GF, class ERange>
inline 
restricted_grid_function_adapter<GF, typename ERange::const_iterator>
Restriction(GF& gf, const ERange& R)
{ 
  typedef typename ERange::const_iterator e_iter;
  return restricted_grid_function_adapter<GF,e_iter>(gf,R.begin(),R.end(),R.size());
}

} // namespace GrAL 

#endif
