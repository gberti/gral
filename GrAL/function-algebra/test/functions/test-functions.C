#include "FunctionAlgebra/function-algebra.h"

#include "Utility/pre-post-conditions.h"

#include <iostream>

inline void check_operator(point const& p1,point const& p2, string const& op)
{
  REQUIRE( (p1[1] == p2[1]), "F1" << op << "F2: Unequal results: "
	   << "F1(p) + F2(p) = " << p1 
	   << "(F1 +F2)(p) = " << p2 << '\n',1);
}

int main() {
  RFunction F1(Sin);
  RFunction F2(Cos);

  RFunction F1AddF2(F1 + F2);
  RFunction F1SubF2(F1 - F2);
  RFunction F1DivF2(F1 / F2);
  RFunction F1MulF2(F1 * F2);

  typedef RFunction::coord_type coord_type;
  coord_type p(1.0);
  
  check_operator(F1(p) + F2(p), F1AddF2(p), "+");
  check_operator(F1(p) - F2(p), F1SubF2(p), "-");
  check_operator(F1(p) / F2(p)[1], F1DivF2(p), "/");
  check_operator(F1(p) * F2(p)[1], F1MulF2(p), "*");
 
  cout << " F1(1.0) = " << F1(p) << '\n'
       << " F2(1.0) = " << F2(p) << '\n'
       << " F1(1.0) + F2(1.0) = " << F1(p) + F2(p) << '\n'
       << " (F1+F2)(1.0)      = " << F1AddF2(p) << '\n'
       << " F1(1.0) - F2(1.0) = " << F1(p) - F2(p) << '\n'
       << " (F1-F2)(1.0)      = " << F1SubF2(p) << '\n'
       << " F1(1.0) / F2(1.0) = " << F1(p) / F2(p)[1] << '\n'
       << " (F1/F2)(1.0)      = " << F1DivF2(p) << '\n'
       << " F1(1.0) * F2(1.0) = " << F1(p) * F2(p) << '\n'
       << " (F1+F2)(1.0)      = " << F1MulF2(p) << '\n';

}
