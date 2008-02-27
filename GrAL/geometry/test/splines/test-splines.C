#include "Geometry/spline.h"

#include <iostream>
#include <fstream>
#include <cmath>

int main() 
{
  using namespace std;

  {
    GrAL::polynomial<5,double> s_shape = GrAL::make_quintic_s_shape<double>();
    
    double coeff[] = { -1, 2 };
    //  GrAL::polynomial<1,double> f(coeff+0, coeff+2);
    GrAL::polynomial<1,double> f(coeff);
    int N = 100;
    double dx = 1.0/N;
    ofstream out0("s-shape.dat");
    ofstream out1("s-shape-d.dat");
    ofstream out2("s-shape-dd.dat");
    for(int i = 0; i <= N; ++i) {
      out0 << f(i*dx) << "  " << s_shape   (f(i*dx)) << "\n";    
      out1 << f(i*dx) << "  " << s_shape.d (f(i*dx)) << "\n";
      out2 << f(i*dx) << "  " << s_shape.dd(f(i*dx)) << "\n";
    }
    out0.close();
    out1.close();
    out2.close();
  }

  {
    double s0[] = {0,0} , ds0[] = {cos(M_PI/3.0), sin(M_PI/3.0)}, dds0[] = {0,0}, ds1[] = {0,1}, dds1[] = {0,0};
    GrAL::polynomial<4,double> q0 = GrAL::make_quartic<double,double>(s0[0], ds0[0], dds0[0], ds1[0], dds1[0]);
    GrAL::polynomial<4,double> q1 = GrAL::make_quartic<double,double>(s0[1], ds0[1], dds0[1], ds1[1], dds1[1]);
    ofstream out0("quartic0.dat");
    ofstream out1("quartic1.dat");
    int N = 100;
    double dx = 1.0/N;
    for(int i = 0; i <=4; ++i)
      cout << "a" << i << "=" << q0[i] << " " << q1[i] << "  ";
    cout << "\n";
    for(int i = 0; i <= N; ++i) {
      out0 <<  q0(i*dx) << "\n";
      out1 <<  q1(i*dx) << "\n";
    }
  }
}
