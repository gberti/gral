#ifndef GRAL_GB_MEASUREMENT_CHECK_EDGE_FACET_INTERSECTIONS_H
#define GRAL_GB_MEASUREMENT_CHECK_EDGE_FACET_INTERSECTIONS_H

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Subranges/enumerated-element-range.h"

namespace GrAL {

namespace measurement {

/*! \brief Class for checking edge/facet intersections of cells

    This class server to find degenerated cells by checking if
    there are any intersections of edges and facets of a given cell.
    This applies only to cells for which there are non-incident
    pairs of edges and facets, for example cubes. 
    It can be used to find heavily twisted trilinear cubes,
    where evaluations of jacobians not necessarily find invalid cells.

 */
template<class GRID, class GEOM, class GT = grid_types<GRID> >
class check_edge_facet_intersections 
{
public:
  typedef GRID grid_type;
  typedef GEOM geom_type;
  typedef GT   gt;

  typedef typename gt::archetype_type archetype_type;
  typedef grid_types<archetype_type> agt;
private:
  grid_type const* g;
  geom_type const* geom;

  typedef enumerated_edge_range<archetype_type> arch_edge_range;
  typedef grid_function<typename agt::Cell,arch_edge_range> arch_cell_edge_range;
  // map cell facets to non-incident edges of same cell
  // (once for each archetype)
  std::vector<arch_cell_edge_range> arch_non_incident_edges;

  partial_grid_function<typename gt::Cell, int> num_intersections_c;
  partial_grid_function<typename gt::Facet,int> num_intersections_f;
  partial_grid_function<typename gt::Edge, int> num_intersections_e;
public:
  check_edge_facet_intersections(grid_type const& gg, geom_type const& geo)
   : g(&gg), geom(&geo),
     num_intersections_c(*g,0),
     num_intersections_f(*g,0),
     num_intersections_e(*g,0)
    {}
  
  // main functionality
  // could also have: check_cell(Cell c)
  void check_all_cells();


  // member access

  grid_type const& TheGrid() const { return *g;}

  partial_grid_function<typename gt::Cell, int> const&
  IntersectedCells() const { return num_intersections_c;}
  partial_grid_function<typename gt::Facet, int> const&
  IntersectedFacets() const { return num_intersections_f;}
  partial_grid_function<typename gt::Edge, int> const&
  IntersectedEdges() const { return num_intersections_e;}

private:
  void find_non_incident_facet_edge_pairs();
};

} //  namespace measurement;

} // namespace GrAL 


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Measurement/check-edge-facet-intersections.C"
#endif

#endif
