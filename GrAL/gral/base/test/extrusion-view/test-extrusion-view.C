
// $LICENSE_NEC


/*! \file
   
     Test for extrusion_view
 */
#include "Gral/Views/extrusion-view.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format-input.h"

#include "Gral/IO/gmv-format-output3d.h"

#include <string>

template class extrusion_view::grid<Complex2D>;


int main() {
  using namespace std;

    typedef grid_types<extrusion_view::grid<Complex2D> > exgt;
    typedef coordN<3> ext_coord_type;
    typedef constant<exgt::Vertex, ext_coord_type> mapping_type;
    mapping_type f(ext_coord_type(0,0,1));

  {
    Complex2D G;
    stored_geometry_complex2D GeomG(G);
    
    string grid_in = "grid.complex2d";
    IstreamComplex2DFmt In(grid_in);
    ConstructGrid(G, GeomG, In, In);
    
    extrusion_view::grid<Complex2D> ExG(G);
    extrusion_view::geometry<stored_geometry_complex2D, mapping_type> ExGeom(ExG, GeomG, f);
    
    OstreamGMV3DFmt Out("ex.gmv");
    ConstructGrid(Out,ExG,ExGeom);
  }
  {
    Complex2D G;
    stored_geometry_complex2D GeomG(G);
    
    string grid_in = "grid2.complex2d";
    IstreamComplex2DFmt In(grid_in);
    ConstructGrid(G, GeomG, In, In);
    
    extrusion_view::grid<Complex2D> ExG(G);
    typedef extrusion_view::geometry<stored_geometry_complex2D, mapping_type> geom_type;
    geom_type ExGeom   (ExG, GeomG, f);
    geom_type ExGeomSym(ExG, GeomG, f, geom_type::symmetric);
    
    OstreamGMV3DFmt Out("ex2.gmv");
    ConstructGrid(Out,ExG,ExGeom);  
    OstreamGMV3DFmt Out2("ex2sym.gmv");
    ConstructGrid(Out2,ExG,ExGeomSym);
  }


  
}
