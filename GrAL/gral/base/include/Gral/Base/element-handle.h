#ifndef GRAL_GB_BASE_ELEMENT_HANDLE_H
#define GRAL_GB_BASE_ELEMENT_HANDLE_H

// $LICENSE 

#include <iostream>
#include "Container/my-hash-map.h"

namespace GrAL {

template<class GRID, class element_tag>  //, class GT = grid_types<GRID> >
struct element_handle_int 
{
  typedef element_handle_int<GRID,element_tag> self;
  typedef GRID grid_type;
  int h;
  element_handle_int() {}
  element_handle_int(int hh) : h(hh) {}

  operator int()  const { return h;}
  // operator       int&()       { return h;}
  self& operator=(int hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(int d) { h+=d; return *this;}
  self& operator-=(int d) { h-=d; return *this;}

};

template<class GRID, class element_tag>
inline
std::istream& operator>>(std::istream& in, element_handle_int<GRID, element_tag> & h)
{ return (in >> h.h); }

} // namespace GrAL

namespace STDEXT {

  template<class T> struct hash;

  template<class GRID, class element_tag>
    struct  hash<GrAL::element_handle_int<GRID, element_tag> > { 
      typedef GrAL::element_handle_int<GRID, element_tag> key_type;
      typedef GrAL::element_handle_int<GRID, element_tag> argument_type;
      typedef size_t                                result_type;
      size_t operator()(GrAL::element_handle_int<GRID, element_tag> h) const { return h.h;}
    };

} // namespace ::STDEXT


namespace GrAL {
/*! \brief Integer based vertex handle

     \ingroup elements
    The purpose of this class is to provide a unique
    type for a grid's vertex handle, in order to overload
    function for different handle types (impossible if
    handles are just typedefs for int).
 */
   
template<class GRID>  //, class GT = grid_types<GRID> >
struct vertex_handle_int 
{
  typedef vertex_handle_int<GRID> self;
  typedef GRID grid_type;
  int h;
  vertex_handle_int() {}
  vertex_handle_int(int hh) : h(hh) {}

  operator  int()  const { return h;}
  // operator       int&()       { return h;}
  self& operator=(int hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(int d) { h+=d; return *this;}
  self& operator-=(int d) { h-=d; return *this;}

};


/*! \brief Integer based edge handle

     \ingroup elements
    The purpose of this class is to provide a unique
    type for a grid's edge handle, in order to overload
    function for different handle types (impossible if
    handles are just typedefs for int).
 */

template<class GRID>  //, class GT = grid_types<GRID> >
struct edge_handle_int 
{
  typedef edge_handle_int<GRID> self;
  typedef GRID grid_type;
  int h;
  edge_handle_int() {}
  edge_handle_int(int hh) : h(hh) {}

  operator int()  const { return h;}
  // operator       int&()       { return h;}
  self& operator=(int hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(int d) { h+=d; return *this;}
  self& operator-=(int d) { h-=d; return *this;}
};

/*! \brief Integer based facet handle

     \ingroup elements
    The purpose of this class is to provide a unique
    type for a grid's facet handle, in order to overload
    function for different handle types (impossible if
    handles are just typedefs for int).
 */

template<class GRID>  //, class GT = grid_types<GRID> >
struct facet_handle_int 
{
  typedef facet_handle_int<GRID> self;
  typedef GRID grid_type;
  int h;
  facet_handle_int() {}
  facet_handle_int(int hh) : h(hh) {}

  operator int()  const { return h;}
  // operator       int&()       { return h;}
  self& operator=(int hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(int d) { h+=d; return *this;}
  self& operator-=(int d) { h-=d; return *this;}
};


/*! \brief Integer based cell handle

  \ingroup elements

  The purpose of this class is to provide a unique
  type for a grid's cell handle, in order to overload
  function for different handle types (impossible if
  handles are just typedefs for int).
*/

template<class GRID>  //, class GT = grid_types<GRID> >
struct cell_handle_int 
{
  typedef cell_handle_int<GRID> self;
  typedef GRID grid_type;
  int h;
  cell_handle_int() {}
  cell_handle_int(int hh) : h(hh) {}

  operator int()  const { return h;}
  //  operator       int&()       { return h;}
  self& operator=(int hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(int d) { h+=d; return *this;}
  self& operator-=(int d) { h-=d; return *this;}
};


template<class GRID>
inline std::istream& operator>>(std::istream& in, vertex_handle_int<GRID> & h)
{ return (in >> h.h); }

template<class GRID>
inline std::istream& operator>>(std::istream& in, edge_handle_int<GRID> & h)
{ return (in >> h.h); }

template<class GRID>
inline std::istream& operator>>(std::istream& in, facet_handle_int<GRID> & h)
{ return (in >> h.h); }

template<class GRID>
inline std::istream& operator>>(std::istream& in, cell_handle_int<GRID> & h)
{ return (in >> h.h); }

} // namespace GrAL 

namespace STDEXT  {
  template<class T> struct hash;

  template<class GRID>
    struct  hash<GrAL::vertex_handle_int<GRID> > { 
      typedef GrAL::vertex_handle_int<GRID> key_type;
      typedef GrAL::vertex_handle_int<GRID> argument_type;
      typedef size_t                  result_type;
      size_t operator()(GrAL::vertex_handle_int<GRID> const& h) const { return h.h;}
    };
  template<class GRID>
    struct  hash<GrAL::edge_handle_int<GRID> > { 
      typedef GrAL::edge_handle_int<GRID> key_type;
      typedef GrAL::edge_handle_int<GRID> argument_type;
      typedef size_t                result_type;
      size_t operator()(GrAL::edge_handle_int<GRID> h) const { return h.h;}
    };
  template<class GRID>
    struct  hash<GrAL::facet_handle_int<GRID> > { 
      typedef GrAL::facet_handle_int<GRID> key_type;
      typedef GrAL::facet_handle_int<GRID> argument_type;
      typedef size_t                 result_type;
      size_t operator()(GrAL::facet_handle_int<GRID> h) const { return h.h;}
    };
  template<class GRID>
    struct  hash<GrAL::cell_handle_int<GRID> > { 
      typedef GrAL::cell_handle_int<GRID> key_type;
      typedef GrAL::cell_handle_int<GRID> argument_type;
      typedef size_t                result_type;
      size_t operator()(GrAL::cell_handle_int<GRID> h) const { return h.h;}
    };
}


#endif





