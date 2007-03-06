/*! \file
    Test harness for \c cartesiannd::mapped_geometry<CARTGRID>
*/


#include "Gral/Grids/CartesianND/mapped-geometry.h"
#include "Gral/Grids/CartesianND/all.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/gmv-format-output3d.h"

#include <iostream>

using namespace GrAL;

namespace GrAL { namespace cartesiannd {
  
  typedef default_coord<grid<1> >::type coord1d;
  typedef default_coord<grid<2> >::type coord2d;
  typedef default_coord<grid<3> >::type coord3d;
  
  typedef matrix<1,1,0>  matrix1d;
  typedef matrix<2,2,0>  matrix2d;
  typedef matrix<3,3,0>  matrix3d;

  typedef affine_mapping<matrix1d, coord1d> mapping1d;
  typedef affine_mapping<matrix2d, coord2d> mapping2d;
  typedef affine_mapping<matrix3d, coord3d> mapping3d;

  template class mapped_geometry<grid<1>, mapping1d>;
  template class mapped_geometry<grid<2>, mapping2d>;
  template class mapped_geometry<grid<3>, mapping3d>;
}}

template<class GEOM>
void test_geometry(GEOM const& Geom, std::ostream& out) 
{

  typedef grid_types<typename GEOM::grid_type> gt; 
  out << "Print Geom of Grid [" 
      << Geom.TheGrid()->low_vertex_index() 
      << "," 
      << Geom.TheGrid()->high_vertex_index() 
      << "]" << std::endl;
  for(typename gt::VertexIterator v(Geom.TheGrid()); ! v.IsDone(); ++v) {
    out << v.index() << " @ " << Geom.coord(*v);
    out << " in cell " << Geom.locate(Geom.coord(*v)).TheCell().index()
	<< " (" << Geom.locate(Geom.coord(*v)).tagname();
    if( Geom.locate(Geom.coord(*v)).tagname() == "projection")
      out << ":  " << Geom.locate(Geom.coord(*v)).TheCoord();
    out << ")"
	<< std::endl;
  }
}

int main() 
{
  using namespace GrAL;
  using namespace std;

  {
    typedef grid_types<cartesiannd::grid<2> > gt;
    typedef cartesiannd::grid<2>              grid_type;
    grid_type R(gt::index_type(0,0), gt::index_type(3,3));

    typedef cartesiannd::default_coord<grid_type>::type   coord_type;
    typedef matrix<2,2,0>  matrix_type;
    typedef affine_mapping<matrix_type, coord_type> mapping_type;
    typedef cartesiannd::mapped_geometry<grid_type, mapping_type> geom_type;
    {
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 1.0;
      matrix_type A_inv(A);
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      geom_type GeomR(R, M);
      GeomR.set_inverse_mapping(M_inv);
    
      test_geometry(GeomR,cout);
    
      OstreamGMV2DFmt Out("2x2.gmv");
      ConstructGrid(Out,R,GeomR);

    }
   
    {
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 2.0;
      matrix_type A_inv(0.0);
      A_inv(0,0) =1.0; A_inv(1,1) = 0.5;
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      geom_type GeomR(R, M);
      GeomR.set_inverse_mapping(M_inv);

      test_geometry(GeomR,cout);
    }
 
    {
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 2.0;
      matrix_type A_inv(0.0);
      A_inv(0,0) =1.0; A_inv(1,1) = 0.5;
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      vector<geom_type> GeomR;
      GeomR.push_back(geom_type(R, gt::index_type(0,0), gt::index_type(1,1), M));
      GeomR.back().set_inverse_mapping(M_inv);

      test_geometry(GeomR.back(),cout);
    }
 

    {
      grid_type R2(gt::index_type(0,0), gt::index_type(3,2));
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 1.0;
      matrix_type  A_inv(A);
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      mapping_type M_dummy(A);
      geom_type GeomR(R2, M_dummy);
      GeomR.set_mapping(M);
      GeomR.set_inverse_mapping(M_inv);
      test_geometry(GeomR,cout);
    }
    {
      grid_type R2(gt::index_type(0,0), gt::index_type(2,3));
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 1.0;
      matrix_type  A_inv(A);
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      mapping_type M_dummy(A);
      geom_type GeomR(R2, M_dummy);
      GeomR.set_mapping(M);
      GeomR.set_inverse_mapping(M_inv);
      test_geometry(GeomR,cout);
    }

    {
      grid_type R2(gt::index_type(0,0), gt::index_type(2,3));
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 1.0;
      matrix_type  A_inv(A);
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      mapping_type M_dummy;
      geom_type GeomR(R2, M_dummy);
      GeomR.set_mapping(M);
      GeomR.set_inverse_mapping(M_inv);
      grid_type R_refined(gt::index_type(0,0), gt::index_type(2,5));
      geom_type Geom_refined(R_refined, GeomR);
      test_geometry(Geom_refined,cout);
    }

    {
      cout << "Testing refinement (isotropic case)\n";
      grid_type R(gt::index_type(0,0), gt::index_type(3,3));
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 1.0;
      matrix_type  A_inv(A);
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      mapping_type M_dummy;
      geom_type GeomR(R, gt::index_type(0,0), gt::index_type(1,1), M_dummy);
      GeomR.set_mapping(M);
      GeomR.set_inverse_mapping(M_inv);
      grid_type R_refined(gt::index_type(0,0), gt::index_type(5,5));
      geom_type Geom_refined(R_refined, GeomR);
      cout << "Master geometry:\n ";
      test_geometry(GeomR,cout);
      cout << "Refined geometry:\n";
      test_geometry(Geom_refined,cout);
    }

    {
      cout << "Testing refinement (anisotropic case)\n";
      grid_type R(gt::index_type(0,0), gt::index_type(3,3));
      matrix_type A(0.0);
      A(0,0) = 1.0; A(1,1) = 1.0;
      matrix_type  A_inv(A);
      mapping_type M(A);
      mapping_type M_inv(A_inv);
      mapping_type M_dummy;
      geom_type GeomR(R, gt::index_type(0,0), gt::index_type(1,1), M_dummy);
      GeomR.set_mapping(M);
      GeomR.set_inverse_mapping(M_inv);
      grid_type R_refined(gt::index_type(0,0), gt::index_type(9,5));
      geom_type Geom_refined(R_refined, GeomR);
      cout << "Master geometry:\n ";
      test_geometry(GeomR,cout);
      cout << "Refined geometry:\n";
      test_geometry(Geom_refined,cout);
    }


  }   


  {
    typedef grid_types<cartesiannd::grid<3> > gt;
    typedef cartesiannd::grid<3>              grid_type;
    grid_type R(gt::index_type(0,0,0), gt::index_type(3,3,3));

    typedef cartesiannd::default_coord<grid_type>::type   coord_type;
    typedef matrix<3,3,0>  matrix_type;
    typedef affine_mapping<matrix_type, coord_type> mapping_type;
    typedef cartesiannd::mapped_geometry<grid_type, mapping_type> geom_type;
    {
      mapping_type M = mapping_type::identity(); 
      mapping_type M_inv = M; 
      geom_type GeomR(R, M);
      GeomR.set_inverse_mapping(M_inv);
    
      test_geometry(GeomR,cout);
    
      OstreamGMV3DFmt Out("2x2x3.gmv");
      ConstructGrid(Out,R,GeomR);

    }
  }
}
