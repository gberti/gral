#include "FunctionAlgebra/function-algebra.h"

#include "Utility/pre-post-conditions.h"

#include <iostream>

int main() {
  RFunction F1(Sin);
  RFunction F2(Cos);

  RFunction F1AddF2(F1 + F2);
  RFunction F1SubF2(F1 - F2);
  RFunction F1DivF2(F1 / F2);
  RFunction F1MulF2(F1 * F2);

  typedef RFunction::coord_type coord_type;
  coord_type p(1.0);
  
  REQUIRE_ALWAYS( (F1(p) + F2(p) == F1AddF2(p)), "F1+F2: unequal results!",1);
  REQUIRE_ALWAYS( (F1(p) - F2(p) == F1SubF2(p)), "F1+F2: unequal results!",1);
  REQUIRE_ALWAYS( (F1(p) / F2(p)[1] == F1DivF2(p)), "F1+F2: unequal results!",1);
  REQUIRE_ALWAYS( (F1(p) * F2(p) == F1MulF2(p)), "F1+F2: unequal results!",1);
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
