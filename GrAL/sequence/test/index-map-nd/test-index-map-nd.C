/*! \file 
    
    Test cases for the class template index_map_nd<N>.
    We test the cases N=1, N=2, N=3.

*/
#include "Container/index-map-nd.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>


template<class MAP>
void test_map(MAP const& map, std::ostream& out)
{
  typedef typename MAP::index_type index_type;
  out << "map.min_flat_index()=" << map.min_flat_index()
      << " map.max_flat_index()=" << map.max_flat_index()
      << " map.max_tuple()= "     << map.max_tuple() <<  endl;
  for(int i = map.min_flat_index(); i <= map.max_flat_index(); ++i) {
    index_type it = map(i);
    int        j  = map(it);
    out << "i= " << i << " map(i)= " << it << endl;
    REQUIRE_ALWAYS( (i==j), " i=" << i << " j=" << j << " index=" << it << "\n", 1);
  }
}


int main() {

  using std::endl;
  std::ostream& out = std::cout;

 

  typedef index_map_nd<1>::index_type it1;
  out << '\n'
      << "Testing 1D maps\n"
      << "===============" << endl;
  {
    int p[1] = {-1};
    it1 it(p);
    index_map_nd<1> map(it);
    out << "Testing map: n = " << it << endl;
    test_map(map, out);
    out << endl;
    REQUIRE_ALWAYS( ! map.valid(it), "", 1);
    REQUIRE_ALWAYS( ! map.valid(0 ), "", 1);
  }
  {
    int p[1] = {0};
    it1 it(p);
    index_map_nd<1> map(it);
    out << "Testing map: n = " << it << endl;
    test_map(map, out);
    out << endl;
    REQUIRE_ALWAYS( ! map.valid(it), "", 1);
    REQUIRE_ALWAYS( ! map.valid(0 ), "", 1);
  }
  {
    int p[1] = {1};
    it1 it(p);
    index_map_nd<1> map(it);
    out << "Testing map: n = " << it << endl;
    test_map(map, out);
    out << endl;
    REQUIRE_ALWAYS( ! map.valid(it), "", 1);
    REQUIRE_ALWAYS( ! map.valid(1 ), "", 1);
  }
  {
    int p[1] = {5};
    it1 it(p);
    index_map_nd<1> map(it);
    out << "Testing map: n = " << it << endl;
    test_map(map, out);
    out << endl;
    REQUIRE_ALWAYS( ! map.valid(it), "", 1);
    REQUIRE_ALWAYS( ! map.valid(5 ), "", 1);
  }
  

  typedef index_map_nd<2>::index_type it2;
  out << '\n'
      << "Testing 2D maps\n"
      << "===============" << endl;
  {
    int p[2] = {-1,-1};
    it2 it(p);
    index_map_nd<2> map(it);
    out << "Testing map: n = " << it << endl;
    test_map(map, out);
    out << endl;
    REQUIRE_ALWAYS( ! map.valid(it), "", 1);
    REQUIRE_ALWAYS( ! map.valid(0 ), "", 1);
  }
  {
    int p[2] = {0,0};
    it2 it(p);
    index_map_nd<2> map(it);
    out << "Testing map: n = " << it << endl;
    test_map(map, out);
    out << endl;
  }
  {
    int p[2] = {1,0};
    it2 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<2> map(it);
    test_map(map, out);
    out << endl;
  }
  {
    int p[2] = {1,1};
    it2 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<2> map(it);
    test_map(map, out);
    out << endl;
  }
  {
    int p[2] = {2,1};
    it2 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<2> map(it);
    test_map(map, out);
    out << endl;
  }


  typedef index_map_nd<3>::index_type it3;
  out << '\n'
      << "Testing 3D maps\n"
      << "===============" << endl;
  {
    int p[3] = {0,0,0};
    it3 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<3> map(it);
    test_map(map, out);
    out << endl;
  }
  {
    int p[3] = {1,1,1};
    it3 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<3> map(it);
    test_map(map, out);
    out << endl;
  }
  {
    int p[3] = {1,2,3};
    it3 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<3> map(it);
    test_map(map, out);
    out << endl;
  }
  {
    int p[3] = {3,2,1};
    it3 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<3> map(it);
    test_map(map, out);
    out << endl;
  }
  {
    int p[3] = {2,3,4};
    it3 it(p);
    out << "Testing map: n = " << it << endl;
    index_map_nd<3> map(it);
    test_map(map, out);
    out << endl;
  }




}
