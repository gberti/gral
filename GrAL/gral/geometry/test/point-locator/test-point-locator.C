/*! \file Test harness for class point_locator<>
   
*/ 

#include "Gral/Geometry/point-locator.h"

#include "Gral/Grids/CartesianND/mapped-geometry.h"

#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include "Container/histogram-table.h"
#include "Container/sequence-algorithms.h"

#include <iostream>

using namespace GrAL;


typedef cartesiannd::grid<2> grid_type;
typedef cartesiannd::default_coord<grid_type>::type   coord_type;
typedef matrix<2,2,0>  matrix_type;
typedef affine_mapping<matrix_type, coord_type> mapping_type;
typedef cartesiannd::mapped_geometry<grid_type, mapping_type> geom_type;
typedef grid_types<grid_type>                    gt;

namespace GrAL {
  template class point_locator<grid_type, geom_type, gt>;
}


struct size_functor {
  template<class T>
  unsigned operator()(T const& t) const { return t.size();}
};

template<class LOCATOR, class IT>
void test_locator(LOCATOR const& Loc, IT begin, IT end, std::ostream& out)
{
  typedef typename LOCATOR::bucket_vertex bucket_vertex;
    out << "Bucket grid: [" 
	<< Loc.TheBucketGrid()->low_vertex_index() << " @ " 
	<< Loc.TheBucketGeometry()->coord(bucket_vertex(Loc.TheBucketGrid(),Loc.TheBucketGrid()->low_vertex_index())) << ","
	<< Loc.TheBucketGrid()->high_vertex_index() << " @ " 
	<< Loc.TheBucketGeometry()->coord(bucket_vertex(Loc.TheBucketGrid(),Loc.TheBucketGrid()->high_vertex_index()))
	<< "]" << std::endl;
    histogram_table<int> Hist;
    sequence::compute_histogram(Loc.TheBuckets()->begin(), Loc.TheBuckets()->end(), Hist, size_functor());
    out << "Bucket histogram:" << std::endl;
    for(histogram_table<int>::const_iterator h=Hist.begin(); h != Hist.end(); ++h)
      out << h->first << " cells: " << h->second << " buckets." << std::endl;

    out << "Buckets:\n";
    typedef typename LOCATOR::Cell Cell;
    for(typename LOCATOR::bucket_cell_iterator b(Loc.TheBucketGrid()); !b.IsDone(); ++b) {
      if(! (*Loc.TheBuckets())(*b).empty()) {
	out << "bucket " << (*b).index() << ": ";
	for(unsigned c = 0; c < (*Loc.TheBuckets())(*b).size(); ++c)
	  out << Cell(Loc.TheGrid(), (*Loc.TheBuckets())(*b)[c]).index() << ", ";
      }
      out << "\n";
    }

    for(IT i = begin; i != end; ++i) {
      out << *i << ": ";
      if(Loc.locate(*i).tagname() == "inside")
	out << "in cell " << Loc.locate(*i).TheCell().index();
      else if(Loc.locate(*i).tagname() == "projection")
	out << " projected to " << Loc.locate(*i).TheCoord() << " in  cell " << Loc.locate(*i).TheCell().index();
      else
	out << " not found in grid";
      out << std::endl;
    }

}

int main()
{
  using namespace GrAL;
  using namespace std;
  typedef gt::index_type it;

  {
    matrix_type  A(0.0); A(0,0) = A(1,1) = 1.0;
    mapping_type M(A);
    grid_type R(it(0,0), it(2,2));
    geom_type GeomR(R, it(0,0), it(1,1), M);
    
    point_locator<grid_type, geom_type, gt> Locator(R,GeomR);
    Locator.init();
    
    typedef coord_type ct;
    coord_type P[6] = { ct(0.0,0.0), ct(-1.0,0.0), ct(1.0,1.0), ct(0.5, 0.5), ct(1.0,0.0), ct(0.0,1.0)};

    test_locator(Locator, P, P + sizeof(P)/sizeof(ct), cout);
  }

  {
    // A = (0 -1)  => rotation by 90 degrees to the left.
    //     (1  0)
    matrix_type  A(0.0); A(0,1) = -1.0;  A(1,0) = 1.0;
    mapping_type M(A);
    grid_type R(it(0,0), it(6,6));
    geom_type GeomR(R, it(0,0), it(1,1), M);

    cout << "V(0,0) @" << GeomR.coord(gt::Vertex(R, it(0, 0)))  << "\n";
    cout << "V(5,5) @" << GeomR.coord(gt::Vertex(R, it(5, 5))) << "\n";
    
    point_locator<grid_type, geom_type, gt> Locator(R,GeomR);
    Locator.init();
    

    typedef coord_type ct;
    coord_type P[] = { ct(0.0,0.0),  ct(-1.0,0.0), ct(1.0,1.0), 
		       ct(0.0, 0.5), ct(0,1.0), ct(0.0,5.0), 
		       ct(-0.5,4.5), ct(-4.5,4.5), ct(-4.5,0.5),
		       ct(-0.5,5.5), ct(-5.5,5.5), ct(-5.5,0.5)};

    test_locator(Locator, P, P + sizeof(P)/sizeof(ct), cout);
  }

  {
    matrix_type  A(1.0); A(0,1) = -1.0;  // left rotation by Pi/4, stretch by sqrt(2)
    mapping_type M(A);
    grid_type R(it(0,0), it(11,11));
    geom_type GeomR(R, it(0,0), it(1,1), M);
    
    point_locator<grid_type, geom_type, gt> Locator(R,GeomR);
    Locator.init();
    
    typedef coord_type ct;
    coord_type P[] = { ct(0.0,0.0),  ct(-1.0,0.0), ct(1.0,1.0), 
		       ct(0.0, 0.5), ct(0,1.0), ct(0.0,5.0), 
		       ct(-0.5,9.5), ct(-9.5,9.5), ct(-9.5,0.5)};

    test_locator(Locator, P, P + sizeof(P)/sizeof(ct), cout);
  }

  {
    matrix_type  A(2.0); A(0,1) = A(1,0) = 1.0;  // shearing & scaling, (1,1) -> (3,3), (1,0) -> (2,1)
    mapping_type M(A);
    grid_type R(it(0,0), it(6,6));
    geom_type GeomR(R, it(0,0), it(1,1), M);

    cout << "V(" << R.low_vertex_index()  << ") @ " << GeomR.coord(gt::Vertex(R, R.low_vertex_index ()))  << "\n";
    cout << "V(" << R.high_vertex_index() << ") @ " << GeomR.coord(gt::Vertex(R, R.high_vertex_index()))  << "\n";

    
    point_locator<grid_type, geom_type, gt> Locator(R,GeomR);
    Locator.init();
 
    typedef coord_type ct;
    coord_type P[] = { ct(0.0,0.0),  ct(15.0,15.0), ct(5.0,10.0), ct(10.0,5.0), 
		       ct(1.0, 0.0), ct(0.0,1.0), ct(-1.0, -1.0),
		       ct(20.0, 30.0), ct(2.0, 1.0),
		       ct(2.0,0.0), ct(3.0, 0.0), ct(4.0,0.0), ct(5.0, 0.0),
		       ct(2.0,-1.0), ct(3.0, -1.0), ct(4.0,-1.0), ct(5.0, -1.0)};

    test_locator(Locator, P, P + sizeof(P)/sizeof(ct), cout);
  }



}
