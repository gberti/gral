#ifndef GRAL_GB_BASE_ARCHETYPE_0D_H
#define GRAL_GB_BASE_ARCHETYPE_0D_H

// $LICENSE_NEC

#include "Gral/Base/grid-function-vector.h"


class archetype_0D {
public:
  enum { dimension = 0 };
  
  struct VertexIterator {
    typedef archetype_0D   grid_type;
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

template<>
struct grid_types<archetype_0D> : public grid_types_base<archetype_0D> 
{
  typedef grid_dim_tag<0> dimension_tag;  
};

template<>
struct element_traits<archetype_0D::Vertex> 
  : public element_traits_vertex_base<archetype_0D>
{
  typedef element_traits_vertex_base<archetype_0D>  base;
  typedef consecutive_integer_tag<0>                       consecutive_tag;
  typedef base::hasher_type_elem_base                      hasher_type;
};


template<class T>
class grid_function<archetype_0D::Vertex, T> 
  : public grid_function_vector<archetype_0D::Vertex, T>
{
  typedef grid_function_vector<archetype_0D::Vertex, T> base;
  typedef archetype_0D                                  grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};


inline bool operator==(archetype_0D::VertexIterator const& lhs,
		       archetype_0D::VertexIterator const& rhs) 
{ return  lhs.handle() == rhs.handle();}
inline bool operator!=(archetype_0D::VertexIterator const& lhs,
		       archetype_0D::VertexIterator const& rhs) 
{ return  !(lhs == rhs);}



template<class V_CORR, class C_CORR>
void ConstructGrid0(archetype_0D      & dst,
		    archetype_0D const& src,
		    V_CORR       & v_corr,
		    C_CORR       & c_corr)
{
  v_corr[0] = 0;
  v_corr[1] = 1;
  c_corr[0] = 0;
  c_corr[1] = 1;
}

template<class PHI>
void ConstructGrid0(archetype_0D      & dst,
		    archetype_0D const& src,
		    PHI               & phi)
{
  typedef archetype_0D::Vertex Vertex;
  phi[Vertex(src,0)] = Vertex(dst,0);
  phi[Vertex(src,1)] = Vertex(dst,1);
}


#endif
