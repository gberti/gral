
#include "Gral/Base/flag.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include "Utility/pre-post-conditions.h"

#include <iostream>


int main()
{
  using namespace cartesian2d;

  typedef CartesianGrid2D grid_type;
  typedef grid_types<grid_type> gt;
  grid_type G(2,3);
  gt::Cell c(*G.FirstCell());
  gt::Edge e = * (c.FirstEdge());
  gt::Vertex v = e.V1();
  flag<grid_type> f(v,e,c);
  std::cout << " flag = " 
	    << f.vertex().handle() << ' ' 
	    << f.edge  ().handle() << ' '
	    << f.cell  ().handle() << '\n';

  f.switch_vertex();
  gt::Vertex v2 = f.vertex();
  std::cout << "switched vertex:   " << v2.handle() << "\n";
  REQUIRE( v2 == e.V2(), "wrong switch_vertex() in flag!\n",1);
  f.switch_vertex();
  std::cout << "switched vertex^2: " << f.vertex().handle() << "\n";

  REQUIRE( v  == f.vertex(),  "switch_vertex()^2 != id!\n",1);


  f.switch_edge();
  std::cout << "switched edge:   " << f.edge().handle() << "\n";
  f.switch_edge();
  std::cout << "switched edge^2: " << f.edge().handle() << "\n";
  gt::Edge e2 = f.edge();
  REQUIRE( e2  == e,  "switch_edge()^2 != id!\n",1);

}


