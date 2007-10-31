/*! \file 
 */

#include "Geometry/geometric-primitives.h"
#include "Geometry/GeomPrimitives/cylinder.h"
#include "Geometry/GeomPrimitives/infinite-cylinder.h"
#include "Container/tuple-point-traits.h"

#include <boost/range/size.hpp>

#include <iostream>

namespace GrAL {

  typedef tuple<double,2> coord_type;
  template class space_point<coord_type>;

  template class segment<coord_type>;
  template class ray<coord_type>;
  template class line<coord_type>;

  template class triangle<coord_type>;
  template class tetrahedron<coord_type>;

  template class cylinder<coord_type>;
  template class infinite_cylinder<coord_type>;
}

int main() {

  using namespace GrAL;
  using namespace std;

  typedef tuple<double,2> coord_type;

  coord_type p0(0.0,0.0), p1(0.0,2.0);
  double r = 0.5;

  cylinder<coord_type>          C (p0,p1, r);
  infinite_cylinder<coord_type> CI(line<coord_type>(p0,p1-p0), r);

  typedef coord_type ct;
  coord_type p[] = { p0, p1, (p0+p1)/2.0, ct(0,-0.5), ct(0,2.5), ct(0.4,1.0), ct(0.6,1.0) };

  for(int i = 0; i < boost::size(p); ++i)
    cout << "Point " << p[i] << " is" << (C.inside(p[i]) ? "     " : " not ") << "inside finite, " 
	 << (CI.inside(p[i]) ? "     " : " not ") << "inside infinite\n";

  {
    coord_type p0(0.0,0.0), p1(1.0,0.0);
    segment<coord_type> s(p0,p1);
    coord_type n(0,1);

    coord_type p[] = { p0, p1, 0.5*(p0+p1), 0.5*(p0+p1) + n, ct(-2,0), p0+ct(-4.0,3), p0+n, p1+n, ct(2.,0), p1+ct(4.0,3.0) };
    cout << "Segment s = [" << s.p0() << "," << s.p1() << "]\n";
    for(int i = 0; i < sizeof(p)/sizeof(ct); ++i) {
      cout << "Point " << p[i] << ": " 
	   << "s.project(p) = " << s.project(p[i]) << "; "
	   << "s.nearest(p) = " << s.nearest(p[i]) << "; "
	   << "distance(p,s) = " << distance(space_point<coord_type>(p[i]),s) << "\n";  
    }
  }
}
