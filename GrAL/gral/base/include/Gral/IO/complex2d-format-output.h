#ifndef NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_OUTPUT_H
#define NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_OUTPUT_H

// $LICENSE

// STD
#include <iostream>
#include <fstream>
#include <string>


// GrAL Base
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/vertex-iterator-int.h"
#include "Gral/Base/cell-iterator-int.h"

#include "Gral/IO/write-complex2d.h"




/*! \brief Output adapter for Complex2D file format
    \ingroup complex2dformat

    \see Module \ref complex2dformat
 */


class OstreamComplex2DFmt {
  typedef OstreamComplex2DFmt self;
protected:
  std::ostream * out;
  bool      owned;
  int       offset;

  int nv, nc;
private:
  void copy(self const& rhs);

public: 
  OstreamComplex2DFmt();
  OstreamComplex2DFmt(std::ostream& ot, int off = 0);
  OstreamComplex2DFmt(std::string const& nm, int off = 0);

  OstreamComplex2DFmt(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  ~OstreamComplex2DFmt();

  void init(std::string const& nm);

  std::ostream& Out() { return *out;}

  int Offset() const { return offset;}

  typedef int                       vertex_handle;
  typedef int                       cell_handle;
  typedef int_vertex_iterator<self> Vertex;
  typedef int_vertex_iterator<self> VertexIterator;
  typedef int_cell_iterator<self>   Cell;
  typedef int_cell_iterator<self>   CellIterator;

  void set_nv(int n) { nv = n;}
  void set_nc(int n) { nc = n;}

  int NumOfVertices() const { return nv;}
  int NumOfCells()    const { return nc;}
  Cell   cell  (int c) const { return Cell  (*this,c);}
  Vertex vertex(int v) const { return Vertex(*this,v);}
};


/*! \brief ConstructGrid overload for OstreamComplex2DFmt
  \ingroup complex2dformat
  \relates OstreamComplex2DFmt
  
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID>
void ConstructGrid0(OstreamComplex2DFmt& Out, 
		    GRID const& G)
{
  Out.set_nv(G.NumOfVertices());
  Out.set_nc(G.NumOfCells());
  write_complex2d(G, Out.Out(), Out.Offset());
}


template<class GRID, class VCORR, class CCORR>
void ConstructGrid0(OstreamComplex2DFmt& Out, 
		    GRID const& G,
                    VCORR & G2Out_v,
                    CCORR & G2Out_c)
{
  Out.set_nv(G.NumOfVertices());
  Out.set_nc(G.NumOfCells());
  write_complex2d(G, Out.Out(), Out.Offset(), G2Out_v, G2Out_c);
}



/*! \brief ConstructGrid overload for OstreamComplex2DFmt
  \ingroup complex2dformat
  \relates OstreamComplex2DFmt
  
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID, class GEOM>
void ConstructGrid(OstreamComplex2DFmt& Out, 
                   GRID const& G, GEOM const& Geom)
{
  Out.set_nv(G.NumOfVertices());
  Out.set_nc(G.NumOfCells());
  write_complex2d(G, Geom, Out.Out(), Out.Offset());
}

/*! \brief ConstructGrid overload for OstreamComplex2DFmt
  \ingroup complex2dformat
  \relates OstreamComplex2DFmt
  
  Dummy output geometry variable.
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID, class GEOM>
void ConstructGrid(OstreamComplex2DFmt& Out, 
                   OstreamComplex2DFmt&, // dummy output geom
                   GRID const& G, GEOM const& Geom)
{
  Out.set_nv(G.NumOfVertices());
  Out.set_nc(G.NumOfCells());
  write_complex2d(G, Geom, Out.Out(), Out.Offset());
}

template<class T>
class grid_function<int_cell_iterator<OstreamComplex2DFmt>,T> 
  : public grid_function_vector<int_cell_iterator<OstreamComplex2DFmt>,T>
{
  typedef grid_function_vector<int_cell_iterator<OstreamComplex2DFmt>,T> base;
public: 
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& tt) : base(gg,tt) {}
};

/*!  \brief Specialization of  \c grid_types  for OstreamComplex2DFmt
    \ingroup complex2dformat


    \see Module \ref complex2dformat
    \see Module \ref traits (\c grid_types)
    \see OstreamComplex2DFmt

*/

template<>
struct grid_types<OstreamComplex2DFmt>
{
  typedef  OstreamComplex2DFmt             grid_type;
  typedef  grid_dim_tag<2>                 dimension_tag;

  typedef  grid_type::Vertex               Vertex;
  typedef  grid_type::Cell                 Cell;
  typedef  grid_type::VertexIterator       VertexIterator;
  typedef  grid_type::CellIterator         CellIterator;
  typedef  grid_type::vertex_handle        vertex_handle;
  typedef  grid_type::cell_handle          cell_handle;
};


template<>
struct element_traits<int_vertex_iterator<OstreamComplex2DFmt> >
  : public element_traits_vertex_base<OstreamComplex2DFmt> 
{
  struct hasher_type {
    unsigned operator()(int_vertex_iterator<OstreamComplex2DFmt> const& v) const
    { return unsigned(v.handle());}
  };
};

template<>
struct element_traits<int_cell_iterator<OstreamComplex2DFmt> >
  : public element_traits_cell_base<OstreamComplex2DFmt> 
{
  struct hasher_type {
    unsigned operator()(int_cell_iterator<OstreamComplex2DFmt> const& c) const
    { return unsigned(c.handle());}
  };
};


#endif
