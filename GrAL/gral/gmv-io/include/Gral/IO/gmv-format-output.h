#ifndef GRAL_GB_GMV_FMT_OUTPUT_H
#define GRAL_GB_GMV_FMT_OUTPUT_H

// $LICENSE_NEC

#include <vector>
#include <string>

#include "Gral/Grids/Complex2D/all.h"
#include "Container/bijective-mapping.h"

#include "Gral/Base/element-numbering.h"
#include "Gral/Base/partial-grid-morphism.h"
#include "Gral/Algorithms/construct-isomorphism.h"

#include "Container/heterogeneous-list.h"
#include "Utility/as-string.h"


namespace std {

template<class T>
  struct hash<T*> {
    unsigned operator()(T* p) const { T* p0 = 0; return p - p0;}
  };

}

/*! \brief Output adapter for the GMV format.

    The <A HREF= "http://www-xdiv.lanl.gov/XCM/gmv/"> 
    General Mesh Viewer </A> (GMV) is a versatile tool
    for viewing 3D meshes and fields defined on them.

    Usage:
    \code
    OstreamGMV3DFmt Out("grid.gmv");
    ConstructGrid(Out,MyGrid, MyGeom); 
    \endcode

   \todo support GMV's general cell type 
   (currently only tet, hex, prism and pyramid)
   \todo support output of grid functions.
 */
class OstreamGMV3DFmt {
  typedef OstreamGMV3DFmt self;
public:
  typedef Complex2D archetype_type;
protected:
  std::ostream * out;
  bool           owned;
  int num_vars;
private:
  void copy(self const& rhs);

public: 
  OstreamGMV3DFmt();
  OstreamGMV3DFmt(std::ostream& ot);
  OstreamGMV3DFmt(std::string const& nm);

  OstreamGMV3DFmt(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  ~OstreamGMV3DFmt();

  void init(std::string const& nm);

  std::ostream& Out() { return *out;}

  int element_tag(vertex_type_tag) const { return 1;}
  int element_tag(cell_type_tag)   const { return 0;}
  // extend to faces

  template<class GF>
  void copy_gf(GF const& gf) 
    {
      typedef element_traits<typename GF::element_type> et;
      std::string varname = "testvar" + as_string(num_vars); // FIXME!

      typedef typename et::element_type_tag element_type_tag;
      *out << varname << " " 
	   << element_tag(element_type_tag()) 
	   << '\n';;
      for(typename et::ElementIterator e(gf.TheGrid()); ! e.IsDone(); ++e) {
	// we assume  numbering in gf.TheGrid() 
	// is the same as numbering in GMV file.
	*out << gf(*e) << ' ';
      }
    }
  namespace hl = heterogeneous_list;

  void copy_grid_functions_rec(hl::List<END,END>) {}

  template<class GF, class TAIL>
  void copy_grid_functions_rec(hl::List<GF,TAIL> gfs)
    { 
      num_vars++;
      copy_gf(gfs.head());
      copy_grid_functions(gfs.tail());
    }

 
  void copy_grid_functions(hl::List<END,END>) {}

  template<class GF, class TAIL>
  void copy_grid_functions(hl::List<GF,TAIL> gfs)
    {
      *out << "variable" << '\n';
      copy_grid_functions_rec(gfs);
      *out << "\n" << "endvars" << "\n";
    }
  
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

template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid(OstreamGMV3DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO,
		   List<GF,MOREGFS> GFS);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/gmv-format-output.tt.C"
#endif


#endif







