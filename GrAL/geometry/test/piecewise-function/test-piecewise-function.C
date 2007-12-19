
#include "Geometry/piecewise-function.h"
#include "Geometry/polynomial.h"

#include "Geometry/spline.h"

#include <iostream>

template<class F>
void print_function(std::ostream& out, F const& f, double * t_beg, double* t_end, std::string const& name)
{
  for(double * t = t_beg; t != t_end; ++t)
    out << name << "(" << *t << ")= " << f(*t) << "  ";
  out << "\n";
  for(double * t = t_beg; t != t_end; ++t)
    out << name << "'(" << *t << ")= " << f.d(*t) << "  ";
  out << "\n";
  for(double * t = t_beg; t != t_end; ++t)
    out << name << "''(" << *t << ")= " << f.dd(*t) << "  ";
  out << "\n";
}


int main() {
  using namespace std;  


  {
    typedef GrAL::polynomial<1,double> poly_type;
    typedef GrAL::piecewise_function<poly_type> function_type;

    poly_type p0(poly_type::coefficients_type(1.0,2.0));  // p(t) = 1 + 2t, p'(t) = 2
    poly_type p1(poly_type::coefficients_type(3.0,-1.0)); // p(t) = 3 -  t, p'(t) = 2
    
    poly_type p[] = { p0, p1 };
    function_type f(p,p+sizeof(p)/sizeof(poly_type));

    double t[] = { -1, -0.5, 0, 0.5, 1, 2, 3 };
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "p  (" << t[i] << ")= " << f(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "p' (" << t[i] << ")= " << f.d(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "p''(" << t[i] << ")= " << f.dd(t[i]) << "  ";
    cout << "\n";
  }

  {
    
    typedef GrAL::polynomial<3,double> cubic;
    typedef GrAL::piecewise_function<cubic> cubic_spline;
    cubic_spline s = GrAL::make_spline(0,0,0, 1, 0);
    cubic s1 = s[0];
    cubic s2 = s[1];

    cout << "Testing spline: \n";
    double t[] = { 0, 1, 2 };
    print_function(cout, s, t, t+sizeof(t)/sizeof(double), "s");
    print_function(cout, s1, t, t+sizeof(t)/sizeof(double), "s1");
    print_function(cout, s2, t, t+sizeof(t)/sizeof(double), "s2");
    /*
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "s  (" << t[i] << ")= " << s(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "s' (" << t[i] << ")= " << s.d(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "s''(" << t[i] << ")= " << s.dd(t[i]) << "  ";
    cout << "\n";
    */
  }  

  {
    typedef GrAL::polynomial<4,double> quartic;
    quartic s = GrAL::make_quartic<double>(0.0,0.0,0.0, 1.0, 0.0);

    cout << "Testing quartic: \n";
    double t[] = { 0, 0.25, 0.5, 0.75, 1};
    print_function(cout, s, t, t+sizeof(t)/sizeof(double), "s");
  }
 

}
