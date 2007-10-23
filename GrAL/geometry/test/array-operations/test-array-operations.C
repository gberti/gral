/*! \file
 */

#include "Geometry/array-operations.h"
#include "Geometry/point-traits.h"
#include "Geometry/coords.h"

#include "Container/tuple-point-traits.h"

#include "Utility/pre-post-conditions.h"


int main() 
{
  {
    typedef GrAL::coordN<2>                coord_type;
    typedef GrAL::tuple<int,2>             int_coord_type;
    typedef GrAL::point_traits<coord_type> pt;
    coord_type x(1,4);
    coord_type y(2,4);
    {
      coord_type z = GrAL::product(x,y);
      REQUIRE_ALWAYS(z == coord_type(2,16), "z= " << z, 1);
    }
    {
      coord_type z = GrAL::quotient(x,y);
      REQUIRE_ALWAYS(z == coord_type(0.5,1), "z= " << z, 1);
    }
    {
      coord_type z = GrAL::sum(x,y);
      REQUIRE_ALWAYS(z == coord_type(3,8), "z= " << z, 1);
      z = x + y;
      REQUIRE_ALWAYS(z == coord_type(3,8), "z= " << z, 1);
    }
    {
      coord_type z = GrAL::difference(x,y);
      REQUIRE_ALWAYS(z == coord_type(-1,0), "z= " << z, 1);
      z = x - y;
      REQUIRE_ALWAYS(z == coord_type(-1,0), "z= " << z, 1);
    }
    {
      coord_type z = x * 2;
      REQUIRE_ALWAYS(z == coord_type(2,8), "z= " << z, 1);
      z = 2 * x;
      REQUIRE_ALWAYS(z == coord_type(2,8), "z= " << z, 1);
      unsigned char s = 2;
      z = s * x;
      REQUIRE_ALWAYS(z == coord_type(2,8), "z= " << z, 1);
    }
    {
      coord_type z = y / 2.0;
      REQUIRE_ALWAYS(z == coord_type(1,2), "z= " << z, 1);
    }
    { 
      coord_type max = max_tuple(x,y);
      coord_type min = min_tuple(x,y);
      REQUIRE_ALWAYS(max == y, "max= " << max, 1);
      REQUIRE_ALWAYS(min == x, "min= " << min, 1);
    }

    {
      coord_type x(0.4, 1.6);
      coord_type z = GrAL::floor_tuple(x);
      REQUIRE_ALWAYS(GrAL::floor_tuple(x) == coord_type(0,1), "z=" << GrAL::floor_tuple(x), 1);
      REQUIRE_ALWAYS(GrAL::ceil_tuple (x) == coord_type(1,2), "z=" << GrAL::ceil_tuple (x), 1);
      REQUIRE_ALWAYS(GrAL::round_tuple(x) == coord_type(0,2), "z=" << GrAL::round_tuple(x), 1);
      int_coord_type fx = GrAL::floor_tuple<int_coord_type>(x);
      int_coord_type cx = GrAL::ceil_tuple <int_coord_type>(x);
      int_coord_type rx = GrAL::round_tuple<int_coord_type>(x);
      REQUIRE_ALWAYS(GrAL::convert_point<coord_type>(fx) == GrAL::floor_tuple(x), "", 1)
      REQUIRE_ALWAYS(GrAL::convert_point<coord_type>(cx) == GrAL::ceil_tuple (x), "", 1)
      REQUIRE_ALWAYS(GrAL::convert_point<coord_type>(rx) == GrAL::round_tuple(x), "", 1)
    }

    {
      coord_type x1(1, 2);
      coord_type x2(-1,3);
      coord_type L(0,0);
      coord_type H(2,2);
      GrAL::clamper<coord_type> clamping(L,H);
      REQUIRE_ALWAYS(clamp_tuple(L,H,x1) == x1, "", 1);
      REQUIRE_ALWAYS(clamp_tuple(L,H,x2) == coord_type(0,2), "", 1);
      REQUIRE_ALWAYS(clamping(x1) == x1, "", 1);
      REQUIRE_ALWAYS(clamping(x2) == coord_type(0,2), "", 1);
      int_coord_type cx = GrAL::clamp_tuple<int_coord_type>(L,H,x1);
      REQUIRE_ALWAYS(GrAL::convert_point<coord_type>(cx) == GrAL::clamp_tuple (L,H,x), "", 1)
    }

 }

  {
    typedef GrAL::tuple<double,2> coord_type;
    typedef GrAL::point_traits<coord_type> pt;
    coord_type x(1,2);
    coord_type y(2,3);
    coord_type z = GrAL::product(x,y);
    REQUIRE_ALWAYS(z == coord_type(2,6), "z= " << z, 1);
    z = x * 2.0;
  }  

  {
    // does not work: componentwise ops disabled for tuple,
    // because it defines its own
    // typedef GrAL::tuple<double,2>            coord_type;
    typedef GrAL::coordN<2>                coord_type;
    typedef GrAL::point_traits<coord_type> pt;
    coord_type a = pt::construct(0.0,0.0);
    coord_type b(1.0,0.0);
    coord_type c(1.0,1.0);
    bool bb  = GrAL::componentwise_compare(a,a, GrAL::relational_operators::eq());
    REQUIRE_ALWAYS(GrAL::componentwise_equal(a,a), "",1);
    REQUIRE_ALWAYS(GrAL::componentwise_less   (a,c), "",1);
    REQUIRE_ALWAYS(GrAL::componentwise_greater(c,a), "",1);
    REQUIRE_ALWAYS(GrAL::componentwise_less_equal   (a,b), "",1);
    REQUIRE_ALWAYS(GrAL::componentwise_greater_equal(b,a), "",1);
    REQUIRE_ALWAYS(!GrAL::componentwise_less  (a,b), "",1);
    REQUIRE_ALWAYS(!GrAL::componentwise_greater(b,a), "",1);
    
  }
}
