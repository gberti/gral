/*! \file 

   Test harness for matrix<M,N,OFF>

 */

// $LICENSE_NEC_2003

#include "Geometry/matrix.h"
#include "Geometry/coords.h"

using namespace GrAL;

template class matrix<2,2,0>;
template class matrix<2,2,1>;
template class matrix_traits<matrix<2,2,0> >;
template class matrix_traits<matrix<2,2,1> >;

#include <iostream>

int main() {
  using namespace std;

  typedef matrix_traits<matrix<2,2,0> > mt0;
  typedef matrix_traits<matrix<2,2,1> > mt1;


  matrix<2,2,0> M0;
  matrix<2,2,1> M1;
  M0(0,0) = M1(1,1) = 1;
  M0(1,0) = M1(2,1) = 2;
  M0(0,1) = M1(1,2) = 3;
  M0(1,1) = M1(2,2) = 4;

  matrix<2,2,0> M0_1(0.0);
  M0_1(0,0) = M0_1(1,1) = 1.0;

  coordN<2> p(-1,-2);
  cout << "M0=\n"  << M0 << endl;
  cout << "M1=\n"  << M1 << endl;
  cout << "M0*p= " << M0*p << endl;
  cout << "M1*p= " << M1*p << endl;
  cout << "M1-M1=\n"  << M1-M1 << endl;
  cout << "M1+M1=\n"  << M1+M1 << endl;
  cout << "M0*M0_1=\n" << M0*M0_1 << endl;
  cout << "-M0=\n" << -1.0*M0 << endl;
  cout << "-M0=\n" << M0*-1.0 << endl;

  cout << "det(M0)=" << mt0::det(M0)       << endl;
  cout << "M0^T=\n"  << mt0::transpose(M0) << endl;
  cout << "M0^-1=\n" << mt0::inverse  (M0) << endl;

  cout << "det(M1)=" << mt1::det(M1)       << endl;
  cout << "M1^T=\n"  << mt1::transpose(M1) << endl;
  cout << "M1^-1=\n" << mt1::inverse  (M1) << endl;



  {
    matrix<3,3,1> N(0.0);
    typedef matrix_traits<matrix<3,3,1> > mt;
    N(1,2) = -1;
    N(2,1) = 1;
    N(3,3) = 1;
    cout << "N=\n"    << N << endl;
    cout << "det(N)=" << mt::det(N)       << endl;
    cout << "N^T=\n"  << mt::transpose(N) << endl;
    cout << "N^-1=\n" << mt::inverse  (N) << endl;
  }

}
