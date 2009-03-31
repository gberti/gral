#ifndef GRAL_GB_BASE_ARCHETYPE_0D_H
#define GRAL_GB_BASE_ARCHETYPE_0D_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/element-handle.h"

namespace GrAL {

/*! \brief  Support for 0-dimensional archetypes

    \ingroup gridarchetypes

    These archetypes are formally required e.g. to end recursions.
*/
  namespace archetype_0D { 

  /*! \brief A 0-dimensional grid which can be used as archetype for regular 1-dimensional grids

  */
class archetype_t { 
  typedef archetype_t self;
public:
  enum { dim = 0 };
  typedef grid_dim_tag<0> dimension_tag;  
  unsigned dimension() const { return 0;}
  typedef self grid_type;
  typedef int  size_type;
  typedef grid_category_d<0> category; 

  struct VertexIterator {
    typedef archetype_t   grid_type;
    typedef VertexIterator self;

    typedef vertex_type_tag element_type_tag;
    typedef grid_type anchor_type;
    typedef self      value_type;

    grid_type    const*  g;
    int                  h;
  public:
    VertexIterator() : g(0) {}
    VertexIterator(grid_type const& gg, int hh = 0) : g(&gg), h(hh) {}
    
    self& operator++() { cv(); ++h; return *this;}
    self const& operator*() const { cv(); return *this;}
    bool IsDone() const { return h >= 2;}
    bool handle() const { return h;}

    grid_type const& TheGrid  () const { cb(); return *g;}
    grid_type const& TheAnchor() const { cb(); return *g;}

    bool bound() const { return g != 0;}
    bool valid() const { return (bound() && h < 2);}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  };
  typedef VertexIterator Vertex;
  typedef VertexIterator Cell;
  typedef VertexIterator CellIterator;
  typedef vertex_handle_int<self> vertex_handle;
  typedef cell_handle_int<self>   cell_handle;

  size_type NumOfVertices() const { return 2;}
  size_type NumOfCells()    const { return 2;}
  VertexIterator FirstVertex() const { return VertexIterator(*this);}
  CellIterator   FirstCell()   const { return CellIterator(*this);}
  VertexIterator EndVertex()   const { return VertexIterator(*this,2);}
  CellIterator   EndCell()     const { return CellIterator(*this,2);}
};


    struct grid_types_archetype_t : public grid_types_detail::grid_types_root  {

      typedef archetype_t                  grid_type;
      typedef grid_type::dimension_tag     dimension_tag;

      typedef grid_type::size_type          size_type;
      typedef grid_type::vertex_handle      vertex_handle;
      typedef grid_type::Vertex             Vertex;
      typedef grid_type::VertexIterator     VertexIterator;
      typedef grid_type::cell_handle        cell_handle;
      typedef grid_type::Cell               Cell;
      typedef grid_type::CellIterator       CellIterator;

    };


inline bool operator==(archetype_t::VertexIterator const& lhs,
		       archetype_t::VertexIterator const& rhs) 
{ return  lhs.handle() == rhs.handle();}
inline bool operator!=(archetype_t::VertexIterator const& lhs,
		       archetype_t::VertexIterator const& rhs) 
{ return  !(lhs == rhs);}



struct grid_types_mixin {
  typedef archetype_t           archetype_type;
  typedef archetype_type const* archetype_iterator;
  typedef int                   archetype_handle;

};

class grid_mixin_base  : public grid_types_mixin {
private:
  static archetype_t the_archetype[1];
public:
  /*! \name Archetype handling
   */
  /*@{*/ 
  static archetype_iterator BeginArchetype() { return the_archetype; }
  static archetype_iterator EndArchetype()   { return the_archetype+1; }
  static archetype_handle   handle(archetype_iterator it)  { return it - BeginArchetype();}
  static archetype_type const& Archetype(archetype_handle = 0) { return *BeginArchetype();} 
  static unsigned NumOfArchetypes() { return 1;}
  /*@}*/  
};


template<class GRID, class GT = grid_types<GRID> >
class grid_mixin : public grid_mixin_base {
public:
  static archetype_type const& ArchetypeOf (typename GT::Cell const&)  
    { return *BeginArchetype();}
  static archetype_type   const& ArchetypeOf (typename GT::cell_handle) 
    { return *BeginArchetype();}
  static archetype_handle        archetype_of(typename GT::cell_handle) 
    { return 0;}
  static archetype_handle        archetype_of(typename GT::Cell const&) 
    { return 0;}
};


} // namespace archetype_0D


template<>
struct grid_types<archetype_0D::archetype_t> 
  : public grid_types_base<archetype_0D::grid_types_archetype_t> 
{};


template<>
struct element_traits<archetype_0D::archetype_t::Vertex> 
  : public element_traits_vertex_base<archetype_0D::archetype_t>
{
  typedef element_traits_vertex_base<archetype_0D::archetype_t>  base;
  typedef consecutive_integer_tag<0>                       consecutive_tag;
  typedef base::hasher_type_elem_base                      hasher_type;
};


namespace archetype_0D {

#define gt grid_types<archetype_0D::archetype_t> 

  inline gt::VertexIterator gral_begin(archetype_0D::archetype_t a, gt::VertexIterator) { return a.FirstVertex();}
  inline gt::VertexIterator gral_end  (archetype_0D::archetype_t a, gt::VertexIterator) { return a.EndVertex();}
  inline gt::size_type      gral_size (archetype_0D::archetype_t a, gt::VertexIterator) { return a.NumOfVertices();}

#undef gt

}



template<class T>
class grid_function<archetype_0D::archetype_t::Vertex, T> 
  : public grid_function_vector<archetype_0D::archetype_t::Vertex, T>
{
  typedef grid_function_vector<archetype_0D::archetype_t::Vertex, T> base;
  typedef archetype_0D::archetype_t                                  grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};




template<class V_CORR, class C_CORR>
void ConstructGrid0(archetype_0D::archetype_t      & dst,
		    archetype_0D::archetype_t const& src,
		    V_CORR       & v_corr,
		    C_CORR       & c_corr)
{
  v_corr[0] = 0;
  v_corr[1] = 1;
  c_corr[0] = 0;
  c_corr[1] = 1;
}

/*
template<class PHI>
void ConstructGrid0(archetype_0D      & dst,
		    archetype_0D const& src,
		    PHI               & phi)
{
  typedef archetype_0D::Vertex Vertex;
  phi[Vertex(src,0)] = Vertex(dst,0);
  phi[Vertex(src,1)] = Vertex(dst,1);
}
*/

} // namespace GrAL

#endif
