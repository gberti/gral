#ifndef  GRAL_GB_GEOMETRY_POINT_LOCATOR_H
#define  GRAL_GB_GEOMETRY_POINT_LOCATOR_H

// $LICENSE_NEC_2003

#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include "Utility/ref-ptr.h"

#include "Gral/Geometries/point-location-base.h"
#include "Gral/Grids/CartesianND/all.h"

#include "Gral/Geometries/bounding-box.h"
#include "Gral/Geometries/cell-intersects-pred.h"
#include "Gral/Geometries/cell-as-volume.h"
#include "Gral/Base/element-grid.h"
#include "Gral/Views/restricted-component-view.h"

/*! \brief Point location in a grid by bucketing
    
    \test test-point-locator.C
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

private:
  ref_ptr<grid_type const> the_grid;
  ref_ptr<geom_type const> the_geom;

  ref_ptr<cart_grid_type> buckets;
  ref_ptr<cart_geom_type> bucket_geometry;

  typedef std::vector<typename gt::cell_handle>             cell_set_type;
  typedef partial_grid_function<bucket_cell, cell_set_type> bucket_type;
  bucket_type                                               intersecting;  
 
  double edge_length;

public:
  point_locator() {}
  point_locator(grid_type const& g, geom_type const& geo) : the_grid(g), the_geom(geo), edge_length(-1.0) {}
  point_locator(ref_ptr<grid_type const> g, 
		ref_ptr<geom_type const> geo) : the_grid(g), the_geom(geo), edge_length(-1.0) {}
  void init();

  ref_ptr<grid_type const> TheGrid()     const { return the_grid;}
  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}

  ref_ptr<cart_grid_type const> TheBucketGrid()     const { return buckets;}
  ref_ptr<cart_geom_type const> TheBucketGeometry() const { return bucket_geometry;}
  ref_ptr<bucket_type const>    TheBuckets()        const { return ref_ptr<bucket_type const>(intersecting);}


  void set_edge_length(double el) { edge_length = el;}

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
template<class COORD>
typename point_locator<GRID,GEOM,GT>::location_result_type
point_locator<GRID,GEOM,GT>::locate(COORD const& p) const
{
  //  typedef point_location_result<gt, COORD> location_result_type;
  typedef typename cart_geom_type::location_result_type cart_result_type;
  cart_result_type loc = bucket_geometry->locate(p);
  // FIXME: cart_geom_type always returns either inside or projection
  if(loc.tag() == cart_result_type::outside_tag)
    return location_result_type();
  else if(loc.tag() == cart_result_type::projection_tag) {
    location_result_type res = project(loc.TheCoord(), loc.TheCell());
    return location_result_type(res.TheCell(), res.TheCoord(),  location_result_type::projection_tag);
  }
  else { // loc.tag() == cart_result_type::inside_tag  => inside bucket grid
    if(intersecting(loc.TheCell()).empty())
      return  project(p,loc.TheCell());
    else {
      // test all cells intersecting the bucket of p
      for(typename cell_set_type::const_iterator c = intersecting(loc.TheCell()).begin(); 
	  c !=  intersecting(loc.TheCell()).end(); ++c) {
	cell_volume_type vol_c(*TheGeometry(), Cell(*TheGrid(), *c));
	if(vol_c.is_inside(p)) {
	  return location_result_type(Cell(*TheGrid(), *c), p, location_result_type::inside_tag);
	}
      }
      // still here? => not found
      return project(p,loc.TheCell());
    }
  }
}



template<class GRID, class GEOM, class GT>
template<class COORD>
typename point_locator<GRID,GEOM,GT>::location_result_type
point_locator<GRID,GEOM,GT>::project(COORD const& p, typename point_locator<GRID,GEOM,GT>::bucket_cell const& loc) const
{
  // no cell in intersects[loc] contains p
  typedef algebraic_primitives<cart_coord_type> apcart;
  typedef algebraic_primitives<coord_type>      ap;

  // consider a neighborhood of loc
  index_type low  = loc.low_vertex_index()  - index_type(1);
  index_type high = loc.high_vertex_index() + index_type(1);
  low  = clamp_tuple(TheBucketGrid()->low_vertex_index(), TheBucketGrid()->high_vertex_index(), low);
  high = clamp_tuple(TheBucketGrid()->low_vertex_index(), TheBucketGrid()->high_vertex_index(), high);
  
  typename cgt::cartesian_subrange_type R(TheBucketGrid(), low, high+index_type(1));
  typedef grid_types<typename cgt::cartesian_subrange_type> rgt;
  // diameter of R is larger than maximal possible distance of cell in R to point p
  double diamR = apcart::distance(TheBucketGeometry()->coord(bucket_vertex(TheBucketGrid(), R.low_vertex_index())),
				  TheBucketGeometry()->coord(bucket_vertex(TheBucketGrid(), R.high_vertex_index())));
  double mindist = diamR;
  Cell   mincell;
  coord_type proj;

  // search all cells intersecting the range
  for(typename rgt::CellIterator xloc(R); !xloc.IsDone(); ++xloc)
    for(typename cell_set_type::const_iterator c=intersecting(*xloc).begin(); c != intersecting(*xloc).end(); ++c) {
      Cell C(* TheGrid(), *c);
      cell_volume_type vol_c(*TheGeometry(), C);
      if(vol_c.is_inside(p))
	return location_result_type(C, p, location_result_type::inside_tag);
      for(typename gt::VertexOnCellIterator vc(C); !vc.IsDone(); ++vc) {
	double dist = ap::distance(p, TheGeometry()->coord(*vc));
	if(dist < mindist) {
	  mindist = dist;
	  proj    = TheGeometry()->coord(*vc);
	  mincell = C;
	}
      }
    }
  if(mindist < diamR)
    return location_result_type(mincell, proj, location_result_type::projection_tag);
  else
    return location_result_type(location_result_type::outside_tag);
}

template<class GRID, class GEOM, class GT>
void point_locator<GRID,GEOM,GT>::init()
{
  // create a Cartesian bucket grid
  box<coord_type> bbox = get_grid_bounding_box(*TheGrid(), *TheGeometry());
  index_type size(0);
  cart_coord_type boxmin = convert_point<cart_coord_type>(bbox.the_min());
  cart_coord_type boxmax = convert_point<cart_coord_type>(bbox.the_max());
  if(edge_length > 0.0) {
    size = round_tuple((boxmax - boxmin)/edge_length);
  }
  else {
    int n = (int) pow(TheGrid()->NumOfCells(), 1.0/spacedim);  
    if(n <= 0) n = 1;
    size = index_type(n);
  }
  size = size + index_type(1);

  buckets = ref_ptr<cart_grid_type>(new cart_grid_type(size));

  // create corresponding Cartesian geometry (scaling & translation)
  matrix_type A    (0.0);
  matrix_type A_inv(0.0);
  cart_coord_type T; assign_point(T, bbox.the_min());
  cart_coord_type T_inv;

  // scaling + translation
  // Ax + T = y  => x = A^-1(y-T) = A^-1 y - A^-1 T
  int li = pt::LowerIndex(bbox.the_min());
  int lt = point_traits<cart_coord_type>::LowerIndex(T);
  for(int i = 0; i < spacedim; ++i) {
    A(i,i) = (bbox.the_max() - bbox.the_min())[i+li];
    A_inv(i,i) = 1.0 / A(i,i);
    T_inv[i+lt] = - A_inv(i,i)*T[i+lt]; 
  }

  mapping_type mapping        (A,     T);
  mapping_type inverse_mapping(A_inv, T_inv);
  bucket_geometry = ref_ptr<cart_geom_type>(new cart_geom_type(*buckets, mapping));
  bucket_geometry->set_inverse_mapping(inverse_mapping);

  // intersect each cell of the target mesh with the bucket grid
  // this is very heuristic
  for(typename gt::CellIterator c(*TheGrid()); !c.IsDone(); ++c) {
    // find one cartesian cell 'loc' intersecting c
    typename cgt::Cell loc_c = bucket_geometry->locate(TheGeometry()->barycenter(*c)).TheCell();
    intersecting[loc_c].push_back(c.handle());
  }

  for(typename gt::CellIterator c(*TheGrid()); !c.IsDone(); ++c) {
    // find the bucket cells containing the vertices of c
    for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
      typename cgt::Cell loc = bucket_geometry->locate(TheGeometry()->coord(*vc)).TheCell();
      if(intersecting[loc].empty()) // || intersecting[loc].back() != c.handle())
	intersecting[loc].push_back(c.handle());
    }
    // find all Cartesian cells intersecting c
    
    /*
    typedef cell_intersects<cell_volume_type, cart_geom_type>  pred_type;
    namespace rcv =                                              restricted_grid_component_view;
    typedef rcv::grid_view<cart_grid_type, pred_type>            intersection_range_type;
    typedef grid_types<intersection_range_type>                  rgt;
    cell_volume_type polyc(*TheGeometry(), 
			     *c);
    // pred_type intersects(cell_volume_type(*TheGeometry(), *c), * bucket_geometry);
    pred_type intersects(polyc, * bucket_geometry);
    intersection_range_type intersecting_buckets(* buckets, intersects, loc);
    for(typename rgt::CellIterator bucket(intersecting_buckets); !bucket.IsDone(); ++bucket)
      intersecting[*bucket].push_back(c.handle());
    */
  }
}

#endif
