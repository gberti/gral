#ifndef GRAL_GB_BASE_ARCHETYPE_0D_H
#define GRAL_GB_BASE_ARCHETYPE_0D_H

// $LICENSE_NEC

#include "Gral/Base/grid-function-vector.h"

namespace archetype_0D {

/*! \brief  A 0-dimensional grid which can be used as archetype for regular 1-dimensional grids

*/

class archetype_t {
public:
  enum { dim = 0 };

  unsigned dimension() const { return 0;}
  
  struct VertexIterator {
    typedef archetype_t   grid_type;
    typedef VertexIterator self;

    grid_type    const*  g;
    int                  h;
  public:
    VertexIterator() : g(0) {}
    VertexIterator(grid_type const& gg, int hh = 0) : g(&gg), h(hh) {}
    
    self& operator++() { cv(); ++h; return *this;}
    self const& operator*() const { cv(); return *this;}
    bool IsDone() const { return h >= 2;}
    bool handle() const { return h;}

    bool bound() const { return g != 0;}
    bool valid() const { return (bound() && h < 2);}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  };
  typedef VertexIterator Vertex;
  typedef VertexIterator Cell;
  typedef VertexIterator CellIterator;
  typedef int vertex_handle;
  typedef int cell_handle;

  unsigned NumOfVertices() const { return 2;}
  unsigned NumOfCells()    const { return 2;}
  VertexIterator FirstVertex() const { return VertexIterator(*this);}
  CellIterator   FirstCell()   const { return CellIterator(*this);}
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
struct grid_types<archetype_0D::archetype_t> : public grid_types_base<archetype_0D::archetype_t> 
{
  typedef grid_dim_tag<0> dimension_tag;  
};

template<>
struct element_traits<archetype_0D::archetype_t::Vertex> 
  : public element_traits_vertex_base<archetype_0D::archetype_t>
{
  typedef element_traits_vertex_base<archetype_0D::archetype_t>  base;
  typedef consecutive_integer_tag<0>                       consecutive_tag;
  typedef base::hasher_type_elem_base                      hasher_type;
};


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



#endif
