/*! \file Test harness for class point_locator<>
   
*/ 

#include "Gral/Geometry/point-locator.h"

#include "Gral/Grids/CartesianND/mapped-geometry.h"

#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include "Container/histogram-table.h"
#include "Container/sequence-algorithms.h"

#include <iostream>



typedef cartesiannd::grid<2> grid_type;
typedef cartesiannd::default_coord<grid_type>::type   coord_type;
typedef matrix<2,2,0>  matrix_type;
typedef affine_mapping<matrix_type, coord_type> mapping_type;
typedef cartesiannd::mapped_geometry<grid_type, mapping_type> geom_type;
typedef grid_types<grid_type>                    gt;
template class point_locator<grid_type, geom_type, gt>;


struct size_functor {
  template<class T>
  unsigned operator()(T const& t) const { return t.size();}
};

template<class LOCATOR, class IT>
void test_locator(LOCATOR const& Loc, IT begin, IT end, std::ostream& out)
{
    out << "Bucket grid: [" 
	<< Loc.TheBucketGrid()->low_vertex_index() << ","
	<< Loc.TheBucketGrid()->high_vertex_index() << "]" << endl;
    histogram_table<int> Hist;
    sequence::compute_histogram(Loc.TheBuckets()->begin(), Loc.TheBuckets()->end(), Hist, size_functor());
    out << "Bucket histogram:" << std::endl;
    for(histogram_table<int>::const_iterator h=Hist.begin(); h != Hist.end(); ++h)
      out << h->first << " cells: " << h->second << " buckets." << std::endl;

    for(IT i = begin; i != end; ++i) {
      cout << *i << ": ";
      if(Loc.locate(*i).tagname() == "inside")
	cout << "in cell " << Loc.locate(*i).TheCell().index();
      else if(Loc.locate(*i).tagname() == "projection")
	cout << " projected to " << Loc.locate(*i).TheCoord();
      else
	cout << " not in grid";
      cout << endl;
    }

}

int main()
{
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
    matrix_type  A(0.0); A(0,1) = -1.0;  A(1,0) = 1.0;
    mapping_type M(A);
    grid_type R(it(0,0), it(11,11));
    geom_type GeomR(R, it(0,0), it(1,1), M);
    
    point_locator<grid_type, geom_type, gt> Locator(R,GeomR);
    Locator.init();
    
    typedef coord_type ct;
    coord_type P[9] = { ct(0.0,0.0),  ct(-1.0,0.0), ct(1.0,1.0), 
			ct(0.0, 0.5), ct(0,1.0), ct(0.0,5.0), 
			ct(-0.5,9.5), ct(-9.5,9.5), ct(-9.5,0.5)};

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
    coord_type P[9] = { ct(0.0,0.0),  ct(-1.0,0.0), ct(1.0,1.0), 
			ct(0.0, 0.5), ct(0,1.0), ct(0.0,5.0), 
			ct(-0.5,9.5), ct(-9.5,9.5), ct(-9.5,0.5)};

    test_locator(Locator, P, P + sizeof(P)/sizeof(ct), cout);
  }



}
