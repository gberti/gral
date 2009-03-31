#ifndef GRAL_GB_GMV_FMT_OUTPUT2D_H
#define GRAL_GB_GMV_FMT_OUTPUT2D_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <vector>
#include <string>

#include "Gral/IO/gmv-format-output.h"

#include "Utility/as-string.h"

namespace GrAL {

/*! \brief Output adapter for the GMV format (2D grids).

    The <A TARGET=_parent HREF="http://www-xdiv.lanl.gov/XCM/gmv/"> 
    General Mesh Viewer </A> (GMV) is a versatile tool
    for viewing 3D meshes and fields defined on them.

    Usage: (\c MyGrid is 2D grid object, 
    model of $GrAL Cell-VertexInputGridRange)
    \code
    OstreamGMV2DFmt Out("grid.gmv");
    ConstructGrid(Out,MyGrid, MyGeom); 
    \endcode

    Tests in \ref test-gmv-output2d.C

    \todo The output of arbitrary grid functions is not working well
     for 2D grids, as vertex and cell grid functions cannot be mixed.
     It works best if you output only cell-based grid function.
 */
class OstreamGMV2DFmt : public OstreamGMVFmt_base {
  typedef OstreamGMV2DFmt    self;
  typedef OstreamGMVFmt_base base;

protected:
  void copy(self const& rhs);

public: 
  OstreamGMV2DFmt();
  OstreamGMV2DFmt(std::ostream& ot);
  OstreamGMV2DFmt(std::string const& nm);

  OstreamGMV2DFmt(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  ~OstreamGMV2DFmt();

  template<class GRID>
  void copy_grid_functions(GRID const&, heterogeneous_list::List<END,END>) {}

  template<class GRID, class GF, class TAIL>
  void copy_grid_functions(GRID const& g, heterogeneous_list::List<GF,TAIL> gfs)
  {
    if(num_gfs_of_dim(gfs,0) > 0) {
      begin_variable();
      copy_grid_functions_of_dim(g,gfs,0);
      end_variable(); 
    }
    if(num_gfs_of_dim(gfs,2) > 0) {
      begin_surfvariable();
      copy_grid_functions_of_dim(g,gfs,2);
      end_surfvariable();
    }
    copy_materials_rec(g,gfs);
  }

protected:
  virtual void begin_variable() { Out() << "variable\n";}
  virtual void end_variable()   { Out() << "endvars\n";}   
  virtual void begin_surfvariable() { Out() << "surfvars\n";}
  virtual void end_surfvariable()   { Out() << "endsvars\n";}  


  template<class GF, class TAIL>
  int num_gfs_of_dim(heterogeneous_list::List<GF, TAIL> gfs, int dim) const
  {
    typedef element_traits<typename GF::element_type> et;
    int val = ( dim == et::dim ? 1 : 0);
    return val + num_gfs_of_dim(gfs.tail(), dim);
  }
  int num_gfs_of_dim(heterogeneous_list::List<END,END>, int) const { return 0;}

  template<class GRID, class GF, class TAIL>
  void copy_grid_functions_of_dim(GRID const& g, heterogeneous_list::List<GF, TAIL> gfs, int dim) 
  {
    typedef element_traits<typename GF::element_type> et;
    if(dim == et::dim && !(material_treat_special && gfs.head().name == "material")) {
      copy_gf(g, *(gfs.head().gf), gfs.head().name);
      ++num_vars;
    }
    copy_grid_functions_of_dim(g, gfs.tail(), dim);
  }
  template<class GRID>
  void copy_grid_functions_of_dim(GRID const&, heterogeneous_list::List<END,END>, int) {}

};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/gmv-format-output2d.tt.C"
#else

namespace GrAL {

/*! \brief ConstructGrid overload for OstreamGMV2DFmt

  \relates OstreamGMV2DFmt

  \see Test in \ref test-gmv-output2d.C 
  \see Gral base module \ref copyoperations
 */
template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV2DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO);

/*! \brief ConstructGrid overload for OstreamGMV2DFmt

  \relates OstreamGMV2DFmt

  This takes an additional list of arbitrary grid functions 
  to be written to the GMV file.

  \see Test in \ref test-gmv-output2d.C 
  \see Gral base module \ref copyoperations
 */
template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid_GF(OstreamGMV2DFmt& Out, 
		      GRID const& G,
		      GEOM const& GEO,
		      heterogeneous_list::List<GF,MOREGFS> GFS);

} // namespace GrAL 

#endif


#endif







