/*! \file
    Test cases for \c affine_mapping<>

*/


#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"
#include "Geometry/coords.h"

#include <iostream>

int main() {
  using namespace std;

  typedef matrix<2,2,1> matrix_t;
  typedef coordN<2>     coord_t;
  typedef affine_mapping<matrix_t, coord_t> mapping_t;
  matrix_t A1(0.0); 
  A1(1,1) = 1;
  A1(2,2) = 1;
  mapping_t M1(A1);

  matrix_t A2(0.0); 
  A2(1,1) = 1;
  A2(2,2) = 2;
  mapping_t M2(A2);
  mapping_t M3(mapping_t::scaling        (coord_t(3.0,4.0)));
  mapping_t M4(mapping_t::inverse_scaling(coord_t(3.0,4.0)));
  
  coord_t x1(0,0);
  coord_t x2(1,2);

  coord_t M1x1 = M1(x1);
  coord_t M2x1 = M2(x1);
  coord_t M3x1 = M3(x1);
  coord_t M4x1 = M4(x1);
  coord_t M4M3x1 = M4(M3x1);

  coord_t M1x2 = M1(x2);
  coord_t M2x2 = M2(x2);
  coord_t M3x2 = M3(x2);
  coord_t M4x2 = M4(x2);
  coord_t M4M3x2 = M4(M3x2);

  cout << "x1=" << x1 
       << " M1*x1=" << M1x1
       << " M2*x1=" << M2x1
       << " M3*x1=" << M3x1
       << " M4*x1=" << M4x1
       << " M4*M3*x1=" << M4M3x1
       << endl;

  cout << "x2=" << x2 
       << " M1*x2=" << M1x2
       << " M2*x2=" << M2x2
       << " M3*x2=" << M3x2
       << " M4*x2=" << M4x2
       << " M4*M3*x2=" << M4M3x2
       << endl;

	       
}
