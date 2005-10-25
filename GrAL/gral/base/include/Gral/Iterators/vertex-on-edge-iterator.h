#ifndef GRAL_GB_BASE_VERTEX_ON_EDGE_ITERATOR_H
#define GRAL_GB_BASE_VERTEX_ON_EDGE_ITERATOR_H

// $LICENSE


#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

  /*! \brief Generic VertexOnEdge Iterator

     \ingroup iterators
     
     \templateparams
     - GT
       - <tt> typedef Edge </tt>
       - <tt> vertex_handle() Edge::v1(), Edge::v2() </tt>

  */ 
template<class GRID, class GT = grid_types<GRID> >
class vertex_on_edge_iterator
{
  typedef vertex_on_edge_iterator<GRID,GT> self;
public:
  typedef GRID                grid_type;
  typedef typename GT::Vertex Vertex; 
  typedef typename GT::vertex_handle vertex_handle;
  typedef typename GT::Edge   Edge;

  typedef Edge   anchor_type;
  typedef Vertex value_type;
  typedef Vertex element_type;

private:
  Edge e;
  int  lv;
public:
  vertex_on_edge_iterator() {}
  explicit
  vertex_on_edge_iterator(Edge const& ee, int llv = 0) : e(ee), lv(llv) {} 

  self& operator++() { ++lv; return *this;}
  bool IsDone() const { return lv > 1;}
  Vertex operator*() const { return (lv == 0 ? e.V1() : e.V2());}
  vertex_handle handle() const { return (lv == 0 ? e.v1() : e.v2());}

  Edge      const& TheEdge  () const { return e;}
  Edge      const& TheAnchor() const { return e;}
  grid_type const& TheGrid  () const { return e.TheGrid();}

  bool operator==(self const& rhs) const { return (lv == rhs.lv);}
  bool operator!=(self const& rhs) const { return (lv != rhs.lv);}

  bool bound() const { return e.valid();}
  bool valid() const { return bound() && 0 <= lv && lv <= 1;}
};

} // namespace GrAL 

#endif
