#ifndef GRAL_BASE_GB_GRID_FACET_HANDLE_H
#define GRAL_BASE_GB_GRID_FACET_HANDLE_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "compiler-config.h"

#include <iostream.h>
#include <stdlib.h>

//----------------------------------------------------------------
// template<class CHandle>
// struct facet_handle;
//
// A generic facet handle defined by a pair (c,lf) where
// c is a cell handle, lf is an integer giving the local
// number of the facet on the cell c: lf \in [0, Cell(c).NumOfFacets()]
//
// On construction, one has to take care that only one of two 
// possible handles for an interior facet is ever created.
// Else two handle denoting the same facet could compare not equal.
// This generally requires access to cell neighbors, i.e. a grid 
// with CellOnCellIterator defined.
//
//----------------------------------------------------------------



template<class CHandle>
struct facet_handle {
  CHandle c;  // cell
  int     lf; // 0 <= lf < NumOfFacets(c) : local position in cell

  facet_handle() {}
  facet_handle(CHandle cc, int llf) : c(cc), lf(llf) {}

  int local_facet() const { return lf;}
};


template<class CHandle>
inline
bool operator== (facet_handle<CHandle> const& ls, facet_handle<CHandle> const& rs)
   { return ((ls.c == rs.c) && (ls.lf == rs.lf)); }

template<class CHandle>
inline
bool operator<  (facet_handle<CHandle> const& ls, facet_handle<CHandle> const& rs)
  { return ((ls.c < rs.c)  || ((ls.c == rs.c) &&  (ls.lf < rs.lf))); }

template<class CHandle>
inline
ostream& operator<< ( ostream& out, facet_handle<CHandle> const& e)
 { return (out << e.c << ' ' << e.lf);}

template<class CHandle>
inline
istream& operator>> ( istream& in,  facet_handle<CHandle>     & e)
 { return (in >> e.c >> e.lf);}

inline size_t hash_facet_handle(facet_handle<int> const& h)
{ return (8*h.c + h.lf);}

template<class T> class hash;
struct hash<facet_handle<int> > {
public:
  typedef facet_handle<int>     argument_type;
  typedef size_t                result_type;

  // hash function -- the factor 8 is heuristic, but it makes at
  // least sure that for the common cell types in 2D and 3D no two
  // different handles map to the same value 
  size_t operator()(const facet_handle<int>& h) const
    { return (8*h.c + h.lf);}
};

#endif
