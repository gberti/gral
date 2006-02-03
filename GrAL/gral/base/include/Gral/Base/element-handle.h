#ifndef GRAL_GB_BASE_ELEMENT_HANDLE_H
#define GRAL_GB_BASE_ELEMENT_HANDLE_H

// $LICENSE 

#include <iostream>
#include "Container/my-hash-map.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

template<class GRID, class element_tag, class GT = grid_types<GRID> >
struct element_handle_int 
{
  typedef grid_element_handle_category_d<etag2dim<GT, element_tag>::value> category;

  typedef element_handle_int<GRID,element_tag,GT> self;
  typedef GRID                   grid_type;
  typedef typename GT::size_type size_type;

  size_type h;
  element_handle_int() {}
  element_handle_int(size_type hh) : h(hh) {}

  operator size_type()  const { return h;}
  // operator       size_type&()       { return h;}
  self& operator=(size_type hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(size_type d) { h+=d; return *this;}
  self& operator-=(size_type d) { h-=d; return *this;}

};

  template<class GRID, class element_tag, class GT>
inline
  std::istream& operator>>(std::istream& in, element_handle_int<GRID, element_tag, GT> & h)
{ return (in >> h.h); }

} // namespace GrAL

namespace STDEXT {

  template<class T> struct hash;

  template<class GRID, class element_tag, class GT>
  struct  hash<GrAL::element_handle_int<GRID, element_tag,GT> > { 
    typedef GrAL::element_handle_int<GRID, element_tag,GT> key_type;
    typedef GrAL::element_handle_int<GRID, element_tag,GT> argument_type;
    typedef size_t                                        result_type;
      size_t operator()(GrAL::element_handle_int<GRID, element_tag,GT> h) const { return h.h;}
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
   
  template<class GRID,class GT = grid_types<GRID> >
struct vertex_handle_int 
{
  typedef grid_vertex_handle_category category;
  typedef vertex_handle_int<GRID,GT> self;
  typedef GRID                    grid_type;
  typedef typename GT::size_type  size_type;
  
  size_type h;
  vertex_handle_int() {}
  vertex_handle_int(size_type hh) : h(hh) {}

  operator  size_type()  const { return h;}
  // operator      size_type&()       { return h;}
  self& operator=(size_type hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(size_type d) { h+=d; return *this;}
  self& operator-=(size_type d) { h-=d; return *this;}

};


/*! \brief Integer based edge handle

     \ingroup elements
    The purpose of this class is to provide a unique
    type for a grid's edge handle, in order to overload
    function for different handle types (impossible if
    handles are just typedefs for int).
 */

template<class GRID, class GT = grid_types<GRID> >
struct edge_handle_int 
{
  typedef grid_edge_handle_category category;
  typedef edge_handle_int<GRID,GT> self;
  typedef GRID grid_type;
  typedef typename GT::size_type  size_type;


  size_type h;
  edge_handle_int() {}
  edge_handle_int(size_type hh) : h(hh) {}

  operator size_type()  const { return h;}
  // operator       size_type&()       { return h;}
  self& operator=(size_type hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(size_type d) { h+=d; return *this;}
  self& operator-=(size_type d) { h-=d; return *this;}
};

/*! \brief Integer based face handle

     \ingroup elements
    The purpose of this class is to provide a unique
    type for a grid's face handle, in order to overload
    function for different handle types (impossible if
    handles are just typedefs for int).
 */

template<class GRID, class GT = grid_types<GRID> >
struct face_handle_int 
{
  typedef grid_face_handle_category category;
  typedef face_handle_int<GRID,GT> self;
  typedef GRID grid_type;
  typedef typename GT::size_type  size_type;


  size_type h;
  face_handle_int() {}
  face_handle_int(size_type hh) : h(hh) {}

  operator size_type()  const { return h;}
  // operator       size_type&()       { return h;}
  self& operator=(size_type hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(size_type d) { h+=d; return *this;}
  self& operator-=(size_type d) { h-=d; return *this;}
};

/*! \brief Integer based facet handle

     \ingroup elements
    The purpose of this class is to provide a unique
    type for a grid's facet handle, in order to overload
    function for different handle types (impossible if
    handles are just typedefs for int).
 */

template<class GRID, class GT = grid_types<GRID> >
struct facet_handle_int 
{
  typedef grid_facet_handle_category category;
  typedef facet_handle_int<GRID,GT> self;
  typedef GRID grid_type;
  typedef typename GT::size_type  size_type;


  size_type h;
  facet_handle_int() {}
  facet_handle_int(size_type hh) : h(hh) {}

  operator size_type()  const { return h;}
  // operator       size_type&()       { return h;}
  self& operator=(size_type hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(size_type d) { h+=d; return *this;}
  self& operator-=(size_type d) { h-=d; return *this;}
};


/*! \brief Integer based cell handle

  \ingroup elements

  The purpose of this class is to provide a unique
  type for a grid's cell handle, in order to overload
  function for different handle types (impossible if
  handles are just typedefs for int).
*/

template<class GRID, class GT = grid_types<GRID> >
struct cell_handle_int 
{
  typedef grid_cell_handle_category category;
  typedef cell_handle_int<GRID,GT>   self;
  typedef GRID                    grid_type;
  typedef typename GT::size_type  size_type;


  size_type h;
  cell_handle_int() {}
  cell_handle_int(size_type hh) : h(hh) {}

  operator size_type()  const { return h;}
  //  operator       size_type&()       { return h;}
  self& operator=(size_type hh) { h = hh; return *this;}
  self& operator++() { ++h; return *this;}
  self& operator+=(size_type d) { h+=d; return *this;}
  self& operator-=(size_type d) { h-=d; return *this;}
};


template<class GRID, class GT>
inline std::istream& operator>>(std::istream& in, vertex_handle_int<GRID,GT> & h)
{ return (in >> h.h); }

template<class GRID, class GT>
inline std::istream& operator>>(std::istream& in, edge_handle_int<GRID,GT> & h)
{ return (in >> h.h); }

template<class GRID, class GT>
inline std::istream& operator>>(std::istream& in, face_handle_int<GRID,GT> & h)
{ return (in >> h.h); }

template<class GRID, class GT>
inline std::istream& operator>>(std::istream& in, facet_handle_int<GRID,GT> & h)
{ return (in >> h.h); }

template<class GRID, class GT>
inline std::istream& operator>>(std::istream& in, cell_handle_int<GRID,GT> & h)
{ return (in >> h.h); }

} // namespace GrAL 

namespace STDEXT  {
  template<class T> struct hash;

  template<class GRID, class GT>
    struct  hash<GrAL::vertex_handle_int<GRID,GT> > { 
      typedef GrAL::vertex_handle_int<GRID,GT> key_type;
      typedef GrAL::vertex_handle_int<GRID,GT> argument_type;
      typedef size_t                  result_type;
      size_t operator()(GrAL::vertex_handle_int<GRID,GT> const& h) const { return h.h;}
    };
  template<class GRID, class GT>
    struct  hash<GrAL::edge_handle_int<GRID,GT> > { 
      typedef GrAL::edge_handle_int<GRID,GT> key_type;
      typedef GrAL::edge_handle_int<GRID,GT> argument_type;
      typedef size_t                result_type;
      size_t operator()(GrAL::edge_handle_int<GRID,GT> h) const { return h.h;}
    };
  template<class GRID, class GT>
    struct  hash<GrAL::face_handle_int<GRID,GT> > { 
      typedef GrAL::face_handle_int<GRID,GT> key_type;
      typedef GrAL::face_handle_int<GRID,GT> argument_type;
      typedef size_t                 result_type;
      size_t operator()(GrAL::face_handle_int<GRID,GT> h) const { return h.h;}
    };
  template<class GRID, class GT>
    struct  hash<GrAL::facet_handle_int<GRID,GT> > { 
      typedef GrAL::facet_handle_int<GRID,GT> key_type;
      typedef GrAL::facet_handle_int<GRID,GT> argument_type;
      typedef size_t                 result_type;
      size_t operator()(GrAL::facet_handle_int<GRID,GT> h) const { return h.h;}
    };
  template<class GRID, class GT>
    struct  hash<GrAL::cell_handle_int<GRID,GT> > { 
      typedef GrAL::cell_handle_int<GRID,GT> key_type;
      typedef GrAL::cell_handle_int<GRID,GT> argument_type;
      typedef size_t                result_type;
      size_t operator()(GrAL::cell_handle_int<GRID,GT> h) const { return h.h;}
    };
}


#endif





