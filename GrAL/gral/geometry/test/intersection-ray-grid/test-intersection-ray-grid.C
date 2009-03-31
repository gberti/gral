/*! \file

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometry/intersection-ray-grid.h"
#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Complex2D/custom-grids.h"

#include "Container/tuple-point-traits.h"
#include "Geometry/geometric-primitives.h"
#include "Utility/floating-point.h"


#include <boost/range/size.hpp>
#include <boost/rational.hpp>

#include <iostream>
#include <vector>
#include <limits>





namespace GrAL {


  template<class I, class F>
  inline boost::rational<I> floating2rational(F x)
  { 
    I n, d;
    floating_point::float2rat(x,n,d);
    return  boost::rational<I>(n,d);
  }


  template<class I>
  class convert_scalar<boost::rational<I> > {
  public:
    template<class T>
    static boost::rational<I> from(T x) { return floating2rational<I>(x);}
    
    template<class I2>
    static boost::rational<I> from(boost::rational<I2> x) { return boost::rational<I>(x.numerator(), x.denominator());}
  };     

} // namespace GrAL



int main() 
{
  using namespace std;
  using namespace GrAL;

  typedef Complex2D                 grid_type;
  //typedef stored_geometry_complex2D geom_type;
  typedef tuple<double,3> coord_type;
  typedef simple_geometry<grid_type, coord_type> geom_type;

  //  typedef geom_type::coord_type     coord_type;
  typedef grid_types<grid_type>     gt;

  grid_type G;
  geom_type GeomG;
  
  ConstructGrid(G,GeomG, custom_grids::Simplex3D(), custom_grids::Simplex3DGeom());

 
  typedef ray<coord_type>               ray_type;
  typedef coord_type ct;
 
  coord_type p[]   = { ct(0.2,0.2,0.2), ct(0,0,0), ct(0.5, -1, -1), ct( 1, 1,0), ct( 1, 1, 1) };
  coord_type dir[] = { ct(1.0,1.0,1.0), ct(1,1,1), ct(0,    1,  1), ct(-1,-1,0), ct(-1,-1,-1) };
      
  for(int r = 0; r < boost::size(p); ++r) {
    ray_type rr(p[r],dir[r]);
    cout << "Ray " << p[r] << " -> " << dir[r] << ":\n";
    std::vector<pair<double, gt::Cell>  > results;
    bool intersects =  intersection_ray_grid(rr, G, GeomG, results);
    if(intersects) 
      for(int i = 0; i < results.size(); ++i)
	cout << " Intersection " << i << ": t=" << results[i].first << " @ " << rr(results[i].first)
	     << " in cell " << results[i].second.handle() << "\n";
  }
  {
    cout << "Testing translated geometry:\n";
    geom_type GeomG2 = GeomG;  
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      GeomG2.coord(*v) += ct(0.1,0.1,0.1);
    
    for(int r = 0; r < boost::size(p); ++r) {
      ray_type rr(p[r]+ct(0.1,0.1,0.1),dir[r]);
      cout << "Ray " << p[r] << " -> " << dir[r] << ":\n";
      std::vector<pair<double, gt::Cell>  > results;
      bool intersects =  intersection_ray_grid(rr, G, GeomG2, results);
      if(intersects) 
	for(int i = 0; i < results.size(); ++i)
	  cout << " Intersection " << i << ": t=" << results[i].first << " @ " << rr(results[i].first)
	       << " in cell " << results[i].second.handle() << "\n";
    }
  }

  {
    cout << "Testing rays through corners:\n";
    geom_type GeomG2 = GeomG;
    coord_type trans(0.1,0.1,0.1);
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      GeomG2.coord(*v) += trans;

    coord_type ctr(0.25,0.26,0.24);

    ctr+= trans;
    for(gt::VertexIterator v(G); !v.IsDone(); ++v) {
      ray_type rr(GeomG.coord(*v), ctr - GeomG.coord(*v));
      cout << "Ray " << rr.p0() << " -> " << rr.dir() << ":\n";
      std::vector<pair<double, gt::Cell>  > results;
      bool intersects =  intersection_ray_grid(rr, G, GeomG2, results);
      if(intersects) 
	for(int i = 0; i < results.size(); ++i)
	  cout << " Intersection " << i << ": t=" << results[i].first << " @ " << rr(results[i].first)
	       << " in cell " << results[i].second.handle() << "\n";
    }
  }
  
  {
    typedef boost::rational<long long> rat_real;
    typedef tuple<rat_real, 3>  rat_coord_type;
    typedef simple_geometry<grid_type, rat_coord_type> rat_geom_type;
    typedef ray<rat_coord_type>                        rat_ray_type;
    rat_geom_type RatGeom(G);
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      RatGeom.coord(*v) = convert_point<rat_coord_type>(GeomG.coord(*v));
    rat_real x = floating2rational<long long>(0.25);
    rat_coord_type ctr(x,x,x);
        for(gt::VertexIterator v(G); !v.IsDone(); ++v) {
      rat_ray_type rr(RatGeom.coord(*v), ctr - RatGeom.coord(*v));
      cout << "Ray " << rr.p0() << " -> " << rr.dir() << ":\n";
      std::vector<pair<rat_real, gt::Cell>  > results;
      bool intersects =  intersection_ray_grid(rr, G, RatGeom, results);
      if(intersects) 
	for(int i = 0; i < results.size(); ++i)
	  cout << " Intersection " << i << ": t=" << results[i].first << " @ " << rr(results[i].first)
	       << " in cell " << results[i].second.handle() << "\n";
    }
  }
  

  
  
}
