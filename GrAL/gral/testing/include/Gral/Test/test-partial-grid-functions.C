#ifndef NMWR_GB_GRAL_TEST_PARTIAL_GRID_FUNCTIONS_C
#define NMWR_GB_GRAL_TEST_PARTIAL_GRID_FUNCTIONS_C

// $LICENSE

#include "Gral/Test/test-partial-grid-functions.h"
#include "Gral/Base/common-grid-basics.h"

#include <vector>

template<class E>
bool test_partial_grid_functions(typename E::grid_type const& G, 
				 std::ostream& out)
{
  typedef element_traits<E> et;
  typedef typename et::handle_type element_handle;
  partial_grid_function<E,int> f0(G, 0);
  partial_grid_function<E,element_handle> f1(G);
  unsigned ecnt = 0;
  REQUIRE_ALWAYS( f0.size() == 0, "f0.size() = " << f0.size() << "\n",1);
  REQUIRE_ALWAYS( f1.size() == 0, "f1.size() = " << f1.size() << "\n",1);
  for(typename et::ElementIterator e(G); ! e.IsDone(); ++e, ++ecnt) {
    REQUIRE_ALWAYS(f0(*e) == 0, "f0(*e) = " << f0(*e) << "\n",1)
    f1[*e] = e.handle();
    REQUIRE_ALWAYS( f1.size() == ecnt+1, "f1.size() = " << f1.size() << "\n",1);
  }

  partial_grid_function<E,element_handle> f2 = f1;   
  partial_grid_function<E,element_handle> f3; f3 = f1;
  std::vector<partial_grid_function<E,element_handle> > v(2);
  v[0] = v[1] = f1;

  REQUIRE_ALWAYS( &(f1  .TheGrid()) == &G, "Wrong grid in gf!\n",1);
  REQUIRE_ALWAYS( &(f2  .TheGrid()) == &G, "Wrong grid in gf!\n",1);
  REQUIRE_ALWAYS( &(f3  .TheGrid()) == &G, "Wrong grid in gf!\n",1);
  REQUIRE_ALWAYS( &(v[0].TheGrid()) == &G, "Wrong grid in gf!\n",1);
  REQUIRE_ALWAYS( &(v[1].TheGrid()) == &G, "Wrong grid in gf!\n",1);

  REQUIRE_ALWAYS(f1  .size() == ecnt, "Wrong size in GF!\n",1);
  REQUIRE_ALWAYS(f2  .size() == ecnt, "Wrong size in GF!\n",1);
  REQUIRE_ALWAYS(f3  .size() == ecnt, "Wrong size in GF!\n",1);
  REQUIRE_ALWAYS(v[0].size() == ecnt, "Wrong size in GF!\n",1);
  REQUIRE_ALWAYS(v[1].size() == ecnt, "Wrong size in GF!\n",1);

  for(typename et::ElementIterator e(G); ! e.IsDone(); ++e) {
    REQUIRE_ALWAYS(f1  (*e) == e.handle(), "incorrect value of gf!\n",1);
    REQUIRE_ALWAYS(f2  (*e) == e.handle(), "incorrect value of gf!\n",1);
    REQUIRE_ALWAYS(f3  (*e) == e.handle(), "incorrect value of gf!\n",1);
    REQUIRE_ALWAYS(v[0](*e) == e.handle(), "incorrect value of gf!\n",1);
    REQUIRE_ALWAYS(v[1](*e) == e.handle(), "incorrect value of gf!\n",1);
  }
  return true;
}



#endif
