#ifndef GRAL_GB_GMV_FMT_OUTPUT2D_H
#define GRAL_GB_GMV_FMT_OUTPUT2D_H

// $LICENSE_NEC

#include <vector>
#include <string>

#include "Gral/IO/gmv-format-output.h"

#include "Utility/as-string.h"


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

protected:
  virtual void begin_variable() { Out() << "surfvars\n";}
  virtual void end_variable()   { Out() << "endsvars\n";}   
};

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/gmv-format-output2d.tt.C"
#else

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


#endif


#endif







