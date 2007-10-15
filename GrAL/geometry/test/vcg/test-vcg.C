/*! \file 

   Test harness for \ref vcg_grp

*/

// $LICENSE_NEC_2006

#include "Geometry/vcg.h"
#include "Geometry/algebraic-primitives.h"

#include <limits>

template<class F, class COORD, class REAL>
void test_pdf(F const& f, COORD x, REAL fx_exp, COORD px_exp, REAL eps)
{
  typedef GrAL::algebraic_primitives<COORD> ap;
  REAL fx = f(x);
  REQUIRE_ALWAYS( fabs(fx - fx_exp) <= eps, "fx=" << fx << " exp:" << fx_exp, 1);
  COORD px = GrAL::vcg::project_to_boundary(f,x, eps);
  REQUIRE_ALWAYS( fabs(f(px)) <= eps, "f(px) = " << f(px) << " > eps = " << eps, 1); 
  REQUIRE_ALWAYS( ap::distance(px,px_exp) <= eps, "px=" << px << " exp:" << px_exp, 1);
}

int main() {

  typedef double              real;
  typedef GrAL::tuple<real,2> c2;
  typedef GrAL::tuple<real,3> c3;

  //  real eps = std::numeric_limits<real>::eps();

  {
    GrAL::vcg::halfspace<c2,real> h(c2(1.0,0.0), c2(0.0,0.0));
    
    real eps = 0.0;
    test_pdf(h, c2( 0.0,0.0),  0.0, c2(0.0,0.0), eps);
    test_pdf(h, c2( 2.0,2.0),  2.0, c2(0.0,2.0), eps);
    test_pdf(h, c2(-2.0,3.0), -2.0, c2(0.0,3.0), eps);
  }

  {
    GrAL::vcg::sphere<c2,real> f(1.0, c2(0.0,0.0));
    
    real eps = 0.0;
    test_pdf(f, c2( 1.0,0.0),  0.0, c2(1.0,0.0), eps);
    test_pdf(f, c2( 2.0,0.0),  1.0, c2(1.0,0.0), eps);
    test_pdf(f, c2( 0.5,0.0), -0.5, c2(1.0,0.0), eps);
  }

  {
    GrAL::vcg::infinite_cylinder<c2,real> f(1.0, c2(0.0,0.0), c2(1.0,0.0));
    
    real eps = 0.0;
    test_pdf(f, c2( 0.0,1.0),  0.0, c2(0.0,1.0), eps);
    test_pdf(f, c2( 0.0,2.0),  1.0, c2(0.0,1.0), eps);
    test_pdf(f, c2( 0.0,0.5), -0.5, c2(0.0,1.0), eps);
  }


  {
    typedef GrAL::vcg::halfspace<c2,real> h_type;
    h_type h1(c2(-1.0, 0.0), c2(0.0,0.0));
    h_type h2(c2( 0.0,-1.0), c2(0.0,0.0));

    typedef GrAL::vcg::union_pdf       <h_type, h_type> uh_type; 
    typedef GrAL::vcg::intersection_pdf<h_type, h_type> ih_type; 

    uh_type uh = GrAL::vcg::unite    (h1,h2); // { x >= 0 || y >= 0 }
    ih_type ih = GrAL::vcg::intersect(h1,h2); // { x >= 0 && y >= 0 }
    real eps = 100.0 * std::numeric_limits<real>::epsilon();

    test_pdf(uh, c2( 0.0, 0.0),  0.0, c2( 0.0, 0.0), eps);
    test_pdf(uh, c2(-1.0,-0.5),  0.5, c2(-1.0, 0.0), eps);
    test_pdf(uh, c2(-1.0, 0.5), -0.5, c2(-1.0, 0.0), eps);

    test_pdf(ih, c2( 0.0, 0.0),  0.0, c2( 0.0, 0.0), eps);
    test_pdf(ih, c2( 1.0, 0.5), -0.5, c2( 1.0, 0.0), eps);
    test_pdf(ih, c2( 1.0,-0.5),  0.5, c2( 1.0, 0.0), eps);
  }


}
