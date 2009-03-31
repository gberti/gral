#ifndef  GRAL_GB_GEOMETRY_POINT_LOCATOR_H
#define  GRAL_GB_GEOMETRY_POINT_LOCATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include "Utility/ref-ptr.h"
#include "Container/sequence-algorithms.h"

#include "Gral/Geometries/point-location-base.h"
#include "Gral/Grids/CartesianND/all.h"

#include "Gral/Geometries/bounding-box.h"
#include "Gral/Geometries/cell-intersects-pred.h"
#include "Gral/Geometries/cell-as-volume.h"
#include "Gral/Base/element-grid.h"
#include "Gral/Views/restricted-component-view.h"

#include <vector>

namespace GrAL {

  /*! \defgroupd geometricsearching Geometric searching on grids

  */

/*! \brief Point location in a grid by bucketing
    
    \ingroup geometricsearching
    \see Tested in \ref test-point-locator.C
*/
template<class GRID, class GEOM, class GT = grid_types<GRID> >
class point_locator 
{
public:
  typedef GRID grid_type;
  typedef GEOM geom_type;
  typedef GT   gt;
  typedef typename gt::Cell   Cell;
  typedef typename gt::Vertex Vertex;

  typedef typename geom_type::coord_type coord_type;
  typedef point_traits<coord_type>       pt;
  typedef typename pt::component_type    scalar_type;
  typedef point_location_result<gt, coord_type> location_result_type;

  enum { dim = grid_type::dim, spacedim = pt::dimension };

  typedef cartesiannd::grid<spacedim>             cart_grid_type;
  typedef matrix<spacedim, spacedim, 0>           matrix_type;
  typedef typename cartesiannd::default_coord<cart_grid_type>::type cart_coord_type;
  typedef affine_mapping<matrix_type, cart_coord_type> mapping_type; 
  typedef cartesiannd::mapped_geometry<cart_grid_type, mapping_type> 
                                                  cart_geom_type;
  typedef grid_types<cart_grid_type> cgt;
  typedef typename cgt::index_type   index_type;
  typedef typename cgt::Cell         bucket_cell;
  typedef typename cgt::Vertex       bucket_vertex;
  typedef typename cgt::CellIterator bucket_cell_iterator;

  typedef cell_as_volume<geom_type, Cell> cell_volume_type;

  typedef algebraic_primitives<cart_coord_type> apcart;
  typedef algebraic_primitives<coord_type>      ap;

private:
  ref_ptr<grid_type const> the_grid;
  ref_ptr<geom_type const> the_geom;

  ref_ptr<cart_grid_type> buckets;
  ref_ptr<cart_geom_type> bucket_geometry;

  typedef std::vector<typename gt::cell_handle>             cell_set_type;
  typedef partial_grid_function<bucket_cell, cell_set_type> bucket_type;
  bucket_type                                               intersecting;  
 
  double edge_length;      // prescribed edge length of bucket grid
  bool   assume_cartesian; // try to build optimal bucket grid for Cartesian target grid
  // control location of additional points used to associate cells to buckets
  // = 0.0: use vertices, = 0.5: use midpoints between centers and vertices
  double shrink_factor;  
  // control size of search region when no cell in initial bucket is found.
  // The maximal cell diameter is multiplied by this number. Default: 1.0
  double search_region_growth_factor; 

  double max_cell_radius;
  double max_projection_distance; // declare points as outside when farther away from grid

  // for statistics
  mutable int current_point_in_cell_tests;
  mutable std::vector<int> point_in_cell_hist; 
  mutable std::vector<int> location_result_hist; 
public:
  point_locator() { init_parameters();}
  point_locator(grid_type const& g, geom_type const& geo) 
    : the_grid(g), the_geom(geo)  { init_parameters();}
  point_locator(ref_ptr<grid_type const> g, 
		ref_ptr<geom_type const> geo) : the_grid(g), the_geom(geo) { init_parameters();}

  void init();
  void init(grid_type const& g, geom_type const& geo) 
   { init(ref_ptr<grid_type const>(g), ref_ptr<geom_type const>(geo)); }
  void init(ref_ptr<grid_type const> g, 
	    ref_ptr<geom_type const> geo) {
    the_grid = g;
    the_geom = geo;
    init();
  }
  
  void init_parameters() { 
    edge_length      = -1.0; 
    shrink_factor    = 0.25; 
    assume_cartesian = false;
    search_region_growth_factor = 1.0;
 
    max_cell_radius  = 0.0;
    max_projection_distance = 0.0;
    init_statistics();
  }
  void init_statistics() const {
    current_point_in_cell_tests = 0;
    point_in_cell_hist   = std::vector<int>(3,0);
    location_result_hist = std::vector<int>(3,0);
  }
  void reset_statistics()   const { init_statistics();}
  void incr_statistics(int loc_result) const { 
    point_in_cell_hist  [loc_result] += current_point_in_cell_tests;
    location_result_hist[loc_result] += 1;
    current_point_in_cell_tests = 0;
  }
  int point_in_cell_tests() const { return  sequence::sum(point_in_cell_hist.begin(),point_in_cell_hist.end());}
  int point_in_cell_tests(int loc_result) const { return point_in_cell_hist[loc_result];}
  int num_results        (int loc_result) const { return location_result_hist[loc_result];}
  int num_locations      () const { return sequence::sum(location_result_hist.begin(), location_result_hist.end());}

  ref_ptr<grid_type const> TheGrid()     const { return the_grid;}
  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}

  ref_ptr<cart_grid_type const> TheBucketGrid()     const { return buckets;}
  ref_ptr<cart_geom_type const> TheBucketGeometry() const { return bucket_geometry;}
  ref_ptr<bucket_type const>    TheBuckets()        const { return ref_ptr<bucket_type const>(intersecting);}
  unsigned NumUsedBuckets() const { return TheBuckets()->size();}
  unsigned NumEmptyBuckets() const { return TheBucketGrid()->NumOfCells() - NumUsedBuckets();}



  void do_assume_cartesian() { assume_cartesian = true;}
  void set_edge_length(double el) { 
    edge_length = el;
  }
  void set_search_region_growth(double gr) { 
    REQUIRE_ALWAYS(gr >= 0.0, "Must use positive growth factor",1);
    search_region_growth_factor = gr;
  }

  void calculate_max_cell_radius();
  /*! \brief Find a cell containing \c p

      \post
      If a cell containing \c p is found, <tt> locate(p).tag() == point_location_base::insided_tag </tt>,
      and \c locate(p).TheCell() is the cell containing p. <BR>
      If no such cell is found,  <tt> locate(p).tag() == point_location_base::projection_tag </tt>,
      \c locate(p).TheCoord() is the projection of \c p to the grid, and \c locate(p).TheCell() 
      is the cell containing the projection.
  */
  template<class COORD>
  location_result_type
  locate(COORD const& p) const;

  template<class COORD>
  location_result_type
  project(COORD const& p, bucket_cell const& loc) const;
 

 
};

template<class GRID, class GEOM, class GT>
void point_locator<GRID,GEOM,GT>::calculate_max_cell_radius()
{
  for(typename gt::CellIterator c(* TheGrid()); !c.IsDone(); ++c) {
    scalar_type cell_radius = 0.0;
    coord_type center = TheGeometry()->barycenter(*c);
    for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
      cell_radius = std::max(cell_radius, ap::distance(center, TheGeometry()->coord(*vc)));
    }
    max_cell_radius = std::max(max_cell_radius, cell_radius);
  }
}

template<class GRID, class GEOM, class GT>
template<class COORD>
typename point_locator<GRID,GEOM,GT>::location_result_type
point_locator<GRID,GEOM,GT>::locate(COORD const& p) const
{
  // std::cout << "locate(" << p << ") " << "\n";
  current_point_in_cell_tests = 0;
  //  typedef point_location_result<gt, COORD> location_result_type;
  typedef typename cart_geom_type::location_result_type cart_result_type;
  cart_result_type loc = bucket_geometry->locate(p);
  // FIXME: cart_geom_type always returns either inside or projection
  if(loc.tag() == cart_result_type::outside_tag) {
    // this could be handled as special situation requiring no point-in-cell tests
    incr_statistics(location_result_type::outside_tag);
    return location_result_type();
  }
  else if(loc.tag() == cart_result_type::projection_tag) {
    location_result_type res = project(loc.TheCoord(), loc.TheCell());
    if(res.tag() ==  location_result_type::outside_tag) {
      incr_statistics(location_result_type::outside_tag);
      return res;
    }
    else {
      incr_statistics(location_result_type::projection_tag);
      return location_result_type(res.TheCell(), res.TheCoord(),  location_result_type::projection_tag);
    }
  }
  else { // loc.tag() == cart_result_type::inside_tag  => inside bucket grid 
   // test all cells intersecting the bucket of p
    for(typename cell_set_type::const_iterator c = intersecting(loc.TheCell()).begin(); 
	c !=  intersecting(loc.TheCell()).end(); ++c) {
      cell_volume_type vol_c(*TheGeometry(), Cell(*TheGrid(), *c));
      ++current_point_in_cell_tests;
      if(vol_c.is_inside(p)) {
	incr_statistics(location_result_type::inside_tag);
	return location_result_type(Cell(*TheGrid(), *c), p, location_result_type::inside_tag);
      }
    }

    // still here? => not found, search in neighborhood
    location_result_type res = project(p,loc.TheCell());
    incr_statistics(res.tag());
    return res;
  }
}



template<class GRID, class GEOM, class GT>
template<class COORD>
typename point_locator<GRID,GEOM,GT>::location_result_type
point_locator<GRID,GEOM,GT>::project(COORD const& p, typename point_locator<GRID,GEOM,GT>::bucket_cell const& loc) const
{
  //  std::cout << "project(" << p << "," << loc.index() << ")\n";
 
  // no cell in intersects[loc] contains p

  // consider a neighborhood of loc
  /*
  index_type low  = loc.low_vertex_index()  - index_type(1);
  index_type high = loc.high_vertex_index() + index_type(1);


  low  = clamp_tuple(TheBucketGrid()->low_vertex_index(), TheBucketGrid()->high_vertex_index(), low);
  high = clamp_tuple(TheBucketGrid()->low_vertex_index(), TheBucketGrid()->high_vertex_index(), high);
  typename cgt::cartesian_subrange_type R(TheBucketGrid(), low, high+index_type(1));
  */
  COORD rmax(search_region_growth_factor*max_cell_radius); // could be max_cell_radius(loc) 
  typename cgt::cartesian_subrange_type R(TheBucketGrid(),
					  TheBucketGeometry()->locate(p-rmax).TheCell(),
					  TheBucketGeometry()->locate(p+rmax).TheCell());
  /*
  std::cout << "R= [" 
	    << TheBucketGeometry()->coord(bucket_vertex(TheBucketGrid(), R.low_vertex_index()))
	    << ",  "
	    << TheBucketGeometry()->coord(bucket_vertex(TheBucketGrid(), R.high_vertex_index()))
	    << "]\n";
  */
  typedef grid_types<typename cgt::cartesian_subrange_type> rgt;
  // diameter of R is larger than maximal possible distance of cell in R to point p
  double diamR = apcart::distance(TheBucketGeometry()->coord(bucket_vertex(TheBucketGrid(), R.low_vertex_index())),
				  TheBucketGeometry()->coord(bucket_vertex(TheBucketGrid(), R.high_vertex_index())));
  double mindist = diamR;
  Cell   mincell;
  coord_type proj;

  //std::cout << "mindist=" << mindist << "\n";
  // search all cells intersecting the range
  for(typename rgt::CellIterator xloc(R); !xloc.IsDone(); ++xloc)
    for(typename cell_set_type::const_iterator c=intersecting(*xloc).begin(); c != intersecting(*xloc).end(); ++c) {
      Cell C(* TheGrid(), *c);
      cell_volume_type vol_c(*TheGeometry(), C);
      ++current_point_in_cell_tests;
      if(vol_c.is_inside(p))
	return location_result_type(C, convert_point<coord_type>(p), location_result_type::inside_tag);
      // use minimal distance to vertex of C as distance(p,C), and treat minimal vertex as projection.
      for(typename gt::VertexOnCellIterator vc(C); !vc.IsDone(); ++vc) {
	double dist = ap::distance(convert_point<coord_type>(p), TheGeometry()->coord(*vc));
	if(dist < mindist) {
	  //std::cout << "C=" << C.handle() << "@" << TheGeometry()->coord(* C.FirstVertex()) << " "
	  //	    << "d=" << dist << "  ";
	  mindist = dist;
	  proj    = TheGeometry()->coord(*vc);
	  mincell = C;
	}
      }
    }
  //  std::cout << std::endl;
  if(mindist < diamR)
    return location_result_type(mincell, proj, location_result_type::projection_tag);
  else
    return location_result_type(location_result_type::outside_tag);
}

template<class GRID, class GEOM, class GT>
void point_locator<GRID,GEOM,GT>::init()
{
  reset_statistics();
  calculate_max_cell_radius();

  typedef point_traits<cart_coord_type> cpt;
  // create a Cartesian bucket grid
  box<coord_type> bbox = get_grid_bounding_box(*TheGrid(), *TheGeometry());
  index_type size(0);
  cart_coord_type boxmin = convert_point<cart_coord_type>(bbox.the_min());
  cart_coord_type boxmax = convert_point<cart_coord_type>(bbox.the_max());

  // get adequate cartesian grid size 
  cart_coord_type  diag = boxmax-boxmin;
  int  maxdiff_idx = std::max_element(diag.begin(), diag.end()) - diag.begin();
  double scale_factor; // scale [0,1]^d to bbox

  if(assume_cartesian) 
    edge_length = TheGeometry()->length(* TheGrid()->FirstEdge());

  if(edge_length > 0.0) {
    size = GrAL::convert_point<index_type>(ceil_tuple((boxmax - boxmin)/edge_length));
    scale_factor = edge_length*size[maxdiff_idx]; // >= diag[maxdiff_idx]
  }
  else {
    double sizeratio = 1.0;
    int li = cpt::LowerIndex(diag);
    for(int i = 0; i < spacedim; ++i)
      if(i != maxdiff_idx)
	sizeratio *= diag[i+li]/diag[maxdiff_idx];
    // aim: size[0]* ... *size[dim-1] \approx  TheGrid()->NumOfCells()
    int n_max = (int) pow(TheGrid()->NumOfCells()/sizeratio, 1.0/spacedim);  
    if(n_max <= 0) n_max = 1;
    for(int i = 0; i < spacedim; ++i)
      size[i] = (int) ceil(n_max * (diag[i+li]/diag[maxdiff_idx+li]));
    scale_factor = diag[maxdiff_idx]; 
  }


  buckets.make_shared(new cart_grid_type(size+index_type(1)));

  // create corresponding Cartesian geometry (scaling & translation)
  matrix_type A    (0.0);
  matrix_type A_inv(0.0);
  cart_coord_type T = boxmin; 
  cart_coord_type T_inv;

  // scaling + translation
  // Ax + T = y  => x = A^-1(y-T) = A^-1 y - A^-1 T
  int lt = cpt::LowerIndex(T);
  for(int i = 0; i < spacedim; ++i) {
    A(i,i) = scale_factor; //(bbox.the_max() - bbox.the_min())[i+li];
    A_inv(i,i)  = 1.0 / A(i,i);
    T_inv[i+lt] = - A_inv(i,i)*T[i+lt]; 
  }

  mapping_type mapping        (A,     T);
  mapping_type inverse_mapping(A_inv, T_inv);
  bucket_geometry.make_shared(new cart_geom_type(*buckets, mapping));
  bucket_geometry->set_inverse_mapping(inverse_mapping);


  //-----  intersect each cell of the target mesh with the bucket grid -----
  // We use a very heuristical approximation 

  // First, add each cell to the bucket containing its center.
  for(typename gt::CellIterator c(*TheGrid()); !c.IsDone(); ++c) {
    typename cgt::Cell loc_c = bucket_geometry->locate(TheGeometry()->barycenter(*c)).TheCell();
    intersecting[loc_c].push_back(c.handle());
  }

  // Then, add cell to buckets containing some other points of the cell
  // Here, we take points between the vertices and the center of the cell
  for(typename gt::CellIterator c(*TheGrid()); !c.IsDone(); ++c) {
    // find the bucket cells containing the vertices of c
    for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
      double s = shrink_factor;
      typename cgt::Cell loc = bucket_geometry->locate(  TheGeometry()->coord(*vc)    *(1.0-s) 
						       + TheGeometry()->barycenter(*c)*  s   ).TheCell();
      //if(intersecting[loc].empty()) // || intersecting[loc].back() != c.handle())
      if(! sequence::contains(intersecting[loc].begin(), intersecting[loc].end(), c.handle()))
	intersecting[loc].push_back(c.handle());
    }
    
    // find all Cartesian cells intersecting c - very expensive!
    // If this step is done, the buckets always contain all cells intersecting them,
    // and no search is necessary.
    /*
    typedef cell_intersects<cell_volume_type, cart_geom_type>    pred_type;
    namespace rcv =                                              restricted_grid_component_view;
    typedef rcv::grid_view<cart_grid_type, pred_type>            intersection_range_type;
    typedef grid_types<intersection_range_type>                  rgt;
    cell_volume_type polyc(*TheGeometry(), 
			     *c);
    pred_type intersects(polyc, * bucket_geometry);
    intersection_range_type intersecting_buckets(* buckets, intersects, loc);
    for(typename rgt::CellIterator bucket(intersecting_buckets); !bucket.IsDone(); ++bucket)
      intersecting[*bucket].push_back(c.handle());
    */
  }
}

} // namespace GrAL 

#endif
