#ifndef GRAL_BASE_GB_GRID_VTUPLE2D_H
#define GRAL_BASE_GB_GRID_VTUPLE2D_H



// $LICENSE


#include "Gral/Base/common-grid-basics.h"
#include "Config/compiler-config.h"

namespace GrAL {

template<class GRID> struct vtuple_2d;


template<class GRID>
bool operator< (vtuple_2d<GRID> const& ls, vtuple_2d<GRID> const& rs);
template<class GRID>
bool operator== (vtuple_2d<GRID> const& ls, vtuple_2d<GRID> const& rs);

/*! \brief [OBSOLETE] Vertex set of a facet with comparision and hashing
  \ingroup elements  
  
  This is to be replacet by vertex_set (which has to be optimized
  for the 2D case, especially the hash function)

  \see  \c hash<vtuple2d<GRID> >
 */
template<class GRID>
struct vtuple_2d {
    typedef grid_types<GRID>           gt;
    typedef GRID                       grid_type;
    typedef typename gt::vertex_handle vertex_handle;
    typedef typename gt::EdgeOnCellIterator EdgeOnCellIterator;
 
 //---- DATA ----
  vertex_handle v[2];
     
  //---------------- construction ---------------------

  vtuple_2d()  {}
  vtuple_2d(const vertex_handle& v1, const vertex_handle& v2) 
    { 
      if(v1 < v2) {
	v[0] = v1; v[1] = v2;
      }
      else {
	v[0] = v2; v[1] = v1;
      }
    }
    vtuple_2d(EdgeOnCellIterator const& f)
    { 
	v[0] = (*f).V1().handle();
	v[1] = (*f).V2().handle();
	if(v[0] > v[1]) {
	    int tmp = v[0]; v[0] = v[1]; v[1] = tmp;
	}
    }

  //---------------- comparision ----------------------

  friend bool operator<  <> (const vtuple_2d<GRID>& ls, 
			     const vtuple_2d<GRID>& rs);
  friend bool operator== <> (const vtuple_2d<GRID>& ls, 
			     const vtuple_2d<GRID>& rs);

  //-------- STL container interface --------------------

  typedef        vertex_handle  value_type;
  typedef        vertex_handle* iterator;
  typedef const  vertex_handle* const_iterator;

  const_iterator begin() const { return v;}
  const_iterator end()   const { return (v+2);}
  iterator       begin()       { return v;}
  iterator       end()         { return (v+2);}

};


template<class GRID>
inline
bool operator< (vtuple_2d<GRID> const& ls, vtuple_2d<GRID> const& rs)
    { return ( (ls.v[0] < rs.v[0]) 
		|| ( (ls.v[0] == rs.v[0]) && (ls.v[1] < rs.v[1]))); }

template<class GRID>
inline
bool operator== (vtuple_2d<GRID> const& ls, vtuple_2d<GRID> const& rs)
    { return (ls.v[0] == rs.v[0] && ls.v[1] == rs.v[1]); }

// helper function for calculate_neighbour_cells()
// get the vertices corresponding to a facet f

/*! \brief creator function for vtuple_2d<GRID> 
 
    \relates vtuple_2d
 */
template<class EdgeOnCellIterator>
inline vtuple_2d<typename EdgeOnCellIterator::grid_type> 
get_vertices(EdgeOnCellIterator const& f)
{
  typedef typename EdgeOnCellIterator::grid_type grid_type;
  typedef vtuple_2d<grid_type>         vtuple;

  return vtuple((*f).V1().handle(),(*f).V2().handle());
}

} // namespace GrAL 

template<class T> struct hash;

/*! \brief Specialization of STL hash<> for vtuple2d<GRID>

    \relates vtuple_2d<GRID>
 */
namespace STDEXT {
  template<class GRID>
    struct hash<GrAL::vtuple_2d<GRID> > {
      enum { p = 37 };
      size_t operator()(GrAL::vtuple_2d<GRID> const& vt) const { 
	return (p*vt.v[1] + (vt.v[0] % p));
      }
    };
}



#endif
