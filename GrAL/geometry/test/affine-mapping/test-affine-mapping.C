/*! \file
    Test cases for \c affine_mapping<>

*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"
#include "Geometry/coords.h"
#include "Geometry/matrix-algorithms.h"

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

#include <iostream>


template<class matrix_t, class coord_t>
void test_mapping2d(std::ostream& out)
{
  using namespace GrAL;
  using namespace std;
  typedef affine_mapping<matrix_t, coord_t> mapping_t;
  mapping_t M0(mapping_t::identity());
  mapping_t M1(mapping_t::scaling(coord_t(1.0,2.0)));
  mapping_t M2(mapping_t::scaling        (coord_t(3.0,4.0)));
  mapping_t M3(mapping_t::inverse_scaling(coord_t(3.0,4.0)));
  mapping_t M4 = M2(M3);
  
  mapping_t T1 = mapping_t::translation(coord_t( 1, 3));
  mapping_t T2 = mapping_t::translation(coord_t(-1,-3));
  mapping_t S1 = mapping_t::scaling        (coord_t(3.0,4.0));
  mapping_t S2 = mapping_t::inverse_scaling(coord_t(3.0,4.0));
  mapping_t M5 = T1(S1);
  mapping_t M6 = S2(T2);
  mapping_t M7 = M6(M5);
  mapping_t M8 = M5(M6);
  mapping_t M9 = mapping_t::inverse(M5);  
  
  coord_t x[] = { coord_t(0,0), coord_t(1,2)};
  mapping_t M[] = { M0, M1, M2, M3, M4, M5, M6, M7, M8, M9};
  
  for(int i = 0; i < (int)(sizeof(x)/sizeof(coord_t)); ++i)
    for(int m = 0; m < (int)(sizeof(M)/sizeof(mapping_t)); ++m)
      cout << "M" << m << "(x" << i << ")=" << M[m](x[i]) << endl; 
}


template<class matrix_t, class coord_t>
void test_mapping3d(std::ostream& out)
{
  using namespace GrAL;
  using namespace std;
  typedef affine_mapping<matrix_t, coord_t> mapping_t;

  mapping_t I(mapping_t::identity());
  cout << "I=\n" << I << "\n";
  
  coord_t e1(1.0,0.0,0.0);
  coord_t e2(0.0,1.0,0.0);
  coord_t e3(0.0,0.0,1.0);
  mapping_t R1(mapping_t::rotation3d(e1, 0.0));
  mapping_t R2(mapping_t::rotation3d(e3, 0.0));
  mapping_t R3(mapping_t::rotation3d(e3, 0.0));
  
  cout << "R1=\n" << R1;
  cout << "R1(e1)=" << R1(e1) << "\n"
       << "R1(e2)=" << R1(e2) << "\n"
       << "R1(e3)=" << R1(e3) << "\n";
  cout << "R2=\n" << R2 << '\n'
       << "R2=\n" << R2 << '\n';
  
  
  mapping_t R4(mapping_t::rotation3d(e1, M_PI/2.0));
  mapping_t R5(mapping_t::rotation3d(e3, M_PI/2.0));
  mapping_t R6(mapping_t::rotation3d(e3, M_PI/2.0));
  cout << "R4=\n" << R4 << '\n'  
       << "R5=\n" << R5 << '\n'  
       << "R6=\n" << R6 << '\n';

  cout << "R4^T=\n" << mapping_t::transposed(R4) << "\n";

  { cout << "rotate_to(e1, e1):\n"    << mapping_t::rotate_to(e1,  e1)    << "\n"; }
  { cout << "rotate_to(e1, -e1):\n"   << mapping_t::rotate_to(e1, -e1)    << "\n"; }
  { cout << "rotate_to(e1, e2):\n"    << mapping_t::rotate_to(e1,  e2)    << "\n"; }
  { cout << "rotate_to(e1, e1+e2):\n" << mapping_t::rotate_to(e1,  e1+e2) << "\n"; }
}


int main() {
  using namespace GrAL;
  using namespace std;
  {
    typedef matrix<2,2,1> matrix_t;
    typedef coordN<2>     coord_t;
    test_mapping2d<matrix_t, coord_t>(cout);
    cout << "-------------" << endl;
  }
  {
    typedef matrix<2,2,0> matrix_t;
    typedef coordN<2>     coord_t;
    test_mapping2d<matrix_t, coord_t>(cout);
    cout << "-------------" << endl;
  }


  {
    typedef matrix<3,3,1> matrix_t;
    typedef coordN<3>     coord_t;
    test_mapping3d<matrix_t, coord_t>(cout);
    cout << "-------------" << endl;
  }

  {
    typedef matrix<3,3,1>     matrix_t;
    typedef tuple<double,3>   coord_t;
    test_mapping3d<matrix_t, coord_t>(cout);
    cout << "-------------" << endl;
  }


  {
    typedef matrix<3,3,1>     matrix_t;
    typedef tuple<double,3>   coord_t;
    typedef affine_mapping<matrix_t, coord_t> mapping_t;
    coord_t axis;
    double  angle;

    {
      matrix_t R =   mapping_t::rotation3d(coord_t(1,0,0), M_PI/2.0).TheMatrix();
      get_rotation_axis_angle(R, axis, angle);
      cout << "axis=" << axis << "  angle=" << angle << endl;
    }
    {
      matrix_t R =   mapping_t::rotation3d(coord_t(-1,0,0), M_PI/2.0).TheMatrix();
      get_rotation_axis_angle(R, axis, angle);
      cout << "axis=" << axis << "  angle=" << angle << endl;
    }
    {
      matrix_t R =   mapping_t::rotation3d(coord_t(1,1,1), M_PI/2.0).TheMatrix();
      get_rotation_axis_angle(R, axis, angle);
      cout << "axis=" << axis << "  angle=" << angle << endl;
    }
    {
      matrix_t R =   mapping_t::rotation3d(coord_t(1,0,0), 0.0).TheMatrix();
      get_rotation_axis_angle(R, axis, angle);
      cout << "axis=" << axis << "  angle=" << angle << endl;
    }


  }
}
