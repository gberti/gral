#ifndef GRAL_GB_GMV_FMT_OUTPUT3D_H
#define GRAL_GB_GMV_FMT_OUTPUT3D_H

// $LICENSE_NEC

#include <vector>
#include <string>

#include "Gral/IO/gmv-format-output.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Container/bijective-mapping.h"


#include "Gral/Base/element-numbering.h"
#include "Gral/Base/partial-grid-morphism.h"
#include "Gral/Algorithms/construct-isomorphism.h"

#include "Container/heterogeneous-list.h"
#include "Utility/as-string.h"


/*! \brief Output adapter for the GMV format.

    The <A HREF= "http://www-xdiv.lanl.gov/XCM/gmv/"> 
    General Mesh Viewer </A> (GMV) is a versatile tool
    for viewing 3D (and 2D) meshes and fields defined on them.

    Usage:
    \code
    OstreamGMV3DFmt Out("grid.gmv");
    ConstructGrid(Out,MyGrid, MyGeom); 
    \endcode

    See also test/gmv-output3d/test-gmv-output.C

    The support for the output of arbitrary grid functions is still
    experimental.


   \todo support GMV's general cell type 
   (currently only tet, hex, prism and pyramid)
   \todo  Support output of vector variables. Currently,
   all variables are assumed scalar. As GMV supports only 
   one vector field (velocity), other fields (forces) will
   have to be written as set of scalar fields (force-x etc).
   Perhaps the branch could be made depending on point_traits<>?
 */

class OstreamGMV3DFmt : public OstreamGMVFmt_base {
  typedef OstreamGMV3DFmt    self;
  typedef OstreamGMVFmt_base base;
public:
  typedef Complex2D archetype_type;

private:
  void copy(self const& rhs);

public: 
  OstreamGMV3DFmt();
  OstreamGMV3DFmt(std::ostream& ot);
  OstreamGMV3DFmt(std::string const& nm);

  OstreamGMV3DFmt(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  ~OstreamGMV3DFmt();

protected:
  virtual void begin_variable() { Out() << "variable\n";}
  virtual void end_variable()   { Out() << "endvars\n";}   

  
private:
  class StaticData {
  public:
    typedef std::vector<archetype_type>  archetype_table;
    typedef bijective_mapping<int, std::string> archetype_name_map;

    archetype_table   * archetypes;
    archetype_name_map* names;

    StaticData();
    ~StaticData();
  };
  static StaticData data;
public:
  std::string const& name(int a) const
    { return (*(data.names))(a); }
  typedef StaticData::archetype_table::const_iterator ArchetypeIterator;
  ArchetypeIterator BeginArchetype() const { return data.archetypes->begin();}
  ArchetypeIterator EndArchetype()   const { return data.archetypes->end();}

};



/*! \brief ConstructGrid overload for OstreamGMV3DFmt

  \relates OstreamGMV3DFmt
  
  \see Module \ref GMV3Dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV3DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO);

/*! \brief ConstructGrid overload for OstreamGMV2DFmt

  \relates OstreamGMV2DFmt

  This takes an additional list of arbitrary grid functions 
  to be written to the GMV file.

  \see Module \ref GMV2Dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
  \see \ref test-gmv-output.C for an example
 */
template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid_GF(OstreamGMV3DFmt& Out, 
		      GRID const& G,
		      GEOM const& GEO,
		      heterogeneous_list::List<GF,MOREGFS> GFS);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/gmv-format-output3d.tt.C"
#endif


#endif







