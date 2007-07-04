#ifndef GRAL_GB_CGNS_IO_WRITE_CGNS_H
#define GRAL_GB_CGNS_IO_WRITE_CGNS_H

// $LICENSE_NEC_2007

#include <cgnslib.h>

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-numbering.h"

#include "Geometry/point-traits.h"
#include "Utility/as-string.h"

#include <string>
#include <vector>

namespace GrAL {

  /*! \brief Write triangular unstructured grid to CGNS file
 
  */
  template<class GRID, class GEOM, class GF>
  void write_triangular_grid_to_cgns(std::string const& filenm,
				     GRID const& g,
				     GEOM const& geom,
				     GF const& cell_regions,
				     int num_cell_regions)
  {
    using namespace std;
    typedef grid_types<GRID> gt;

    int cgns_index_file;
    cg_open(filenm.c_str(),MODE_WRITE,&cgns_index_file);

    int cgns_index_base;
    string cgns_basename = "MyBase";
    int cgns_celldim = g    .dimension();
    int cgns_physdim = geom.space_dimension();
    cg_base_write(cgns_index_file,cgns_basename.c_str(),
		  cgns_celldim,cgns_physdim,
		  &cgns_index_base);

    int cgns_index_zone;
    string cgns_zonename = "MyZone";
    int cgns_isize[3][1];
    cgns_isize[0][0] = g.NumOfVertices();
    cgns_isize[1][0] = g.NumOfCells();
    cgns_isize[2][0] = 0; // boundary vertex size - ??
    cg_zone_write(cgns_index_file,cgns_index_base, 
		  cgns_zonename.c_str(), cgns_isize[0],
		  Unstructured, 
		  &cgns_index_zone);


    // assuming geom.space_dimension() == 2 here.
    vector<double> x_coords(g.NumOfVertices());
    vector<double> y_coords(g.NumOfVertices());
    vector<double> z_coords(g.NumOfVertices());
    int nv = 0;
    typedef point_traits<typename GEOM::coord_type> pt;
    for(typename gt::VertexIterator v(g); !v.IsDone(); ++v, ++nv) {
      x_coords[nv] = pt::x(geom.coord(*v));
      y_coords[nv] = pt::y(geom.coord(*v));
      z_coords[nv] = pt::z(geom.coord(*v));
    }
    int cgns_index_coord;
    cg_coord_write(cgns_index_file,cgns_index_base,cgns_index_zone,
		   RealDouble,"CoordinateX",&(x_coords[0]),&cgns_index_coord);
    cg_coord_write(cgns_index_file,cgns_index_base,cgns_index_zone,
		   RealDouble,"CoordinateY",&(y_coords[0]),&cgns_index_coord);
    cg_coord_write(cgns_index_file,cgns_index_base,cgns_index_zone,
		   RealDouble,"CoordinateZ",&(z_coords[0]),&cgns_index_coord);


    int nelem_start = 1;
    int nelem_end   = nelem_start-1; //g.NumOfCells();
    int nbdyelem    = 0; 
    element_numbering<typename gt::Vertex> vnum(g, 1);
    for(int reg = 0; reg < num_cell_regions; ++reg) {
      ElementType_t cgns_element_type = TRI_3;
      vector<int> cgns_ielem;
      int nvc = 0;
      for(typename gt::CellIterator c(g); !c.IsDone(); ++c) {
	if(cell_regions(*c) == reg) {
	  ++nelem_end;
	  for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc, ++nvc)
	    cgns_ielem.push_back(vnum(*vc));
	}
      }
    
      int cgns_index_section;
      string section_name = "MySection" + as_string(reg);
      cg_section_write(cgns_index_file,cgns_index_base,cgns_index_zone,
		       section_name.c_str(),cgns_element_type, 
		       nelem_start, nelem_end, nbdyelem, &(cgns_ielem[0]), 
		       &cgns_index_section);
      nelem_start = nelem_end + 1;
      // nelem_end   = nelem_start;
    }
    if(nelem_end != g.NumOfCells())
      std::cerr << "WARNING: written " << nelem_end << " cells, but grid has " << g.NumOfCells() << " cells." << std::endl;


    cg_close(cgns_index_file);

  }


  template<class GRID, class GEOM>
  void write_triangular_grid_to_cgns(std::string const& filenm,
				     GRID const& g,
				     GEOM const& geom)
  {
    typedef grid_types<GRID> gt;
    partial_grid_function<typename gt::Cell, int> zero(g,0);
    // constant_grid_function<typename gt::Cell, int> zero(g,0);
    write_triangular_grid_to_cgns(g,geom,zero, 0);
  }

} // namespace GrAL


#endif
