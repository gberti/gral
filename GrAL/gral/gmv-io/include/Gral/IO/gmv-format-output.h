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


namespace std {

template<class T>
  struct hash<T*> {
    unsigned operator()(T* p) const { T* p0 = 0; return p - p0;}
  };

}

class OstreamGMV3DFmt {
  typedef OstreamGMV3DFmt self;
public:
  typedef Complex2D archetype_type;
protected:
  std::ostream * out;
  bool           owned;

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
  \ingroup GMV3Dformat
  \relates OstreamGMV3DFmt
  
  \see Module \ref GMV3Dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV3DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO)
{
  typedef OstreamGMV3DFmt GMV3D;
  ostream& out = Out.Out();
  typedef grid_types<GRID> gt;
  typedef grid_types<GMV3D::archetype_type> gmv_agt;

  out << "gmvinput ascii\n";
  out << "nodev " << G.NumOfVertices() << '\n';

  for(typename gt::VertexIterator v(G); ! v.IsDone(); ++v)
    out << GEO.coord(*v) << '\n';

  element_numbering<typename gt::Vertex> G2GMV(G,1);

  // map archetypes.
  typedef partial_grid_morphism<typename gt::archetype_type,
                                GMV3D::archetype_type>  
  morphism_type;
 
  bijective_mapping<typename gt::archetype_type const*,
                    std::pair<morphism_type, int> > phi;
  for(typename gt::ArchetypeIterator a = G.BeginArchetype();
      a != G.EndArchetype(); ++a) {
    bool found = false;
    int cnt_a = 0;
    for(GMV3D::ArchetypeIterator a_gmv = Out.BeginArchetype();
	a_gmv != Out.EndArchetype(); ++a_gmv, ++cnt_a) {
      morphism_type phi_a(*a,*a_gmv);
      found = construct_isomorphism(*a,*a_gmv, phi_a);
      if(found) {
	phi[&(*a)] = std::make_pair(phi_a,cnt_a);
	break;
      }
    }
    ENSURE(found, "sorry, no mapping to GMV cell type found!\n",1);
    // should use the general cell feature of GMV.
  }

  // map cells
  out << "cells " << G.NumOfCells() << "\n";
  for(typename gt::CellIterator c(G); ! c.IsDone(); ++c) {
    morphism_type const& phi_a = phi(&G.ArchetypeOf(*c)).first;
    int                  cnt_a = phi(&G.ArchetypeOf(*c)).second;
    GMV3D::archetype_type const& archetype_gmv(phi_a.ImgGrid());
    out << Out.name(cnt_a) << " "
	<< archetype_gmv.NumOfVertices() << " ";
    // gmv-archetype -> G::archetype -> G::global -> GMV::global
    for(gmv_agt::VertexIterator vc(archetype_gmv); !vc.IsDone(); ++vc)
      out << G2GMV( (*c).V(phi_a.inverse()(*vc)))  << " ";
    out << '\n';
  }

  out << "endgmv\n";
}




#endif







