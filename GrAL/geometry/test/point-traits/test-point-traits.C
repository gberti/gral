/*! \file
 */

#include "Geometry/point-traits.h"


namespace GrAL {
  // does not work, because some members return array
  // template class point_traits<float[3]>;
}

int main() 
{
  using namespace GrAL;
  typedef point_traits<float[3]> pt3;
  typedef float float3[3];

  float3 f;
  pt3::x(f) = 1.0;
  pt3::y(f) = 0.0;
  pt3::z(f) = 0.0;

  // float3 zero = pt3::Origin();

  typedef double double2[2];
  typedef point_traits<double2> pt2;
  //  double2 e1 = pt2::unit_vector(1);

}
