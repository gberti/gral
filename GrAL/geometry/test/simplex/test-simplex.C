/*! \file 

   Test cases for simplex<> class template
*/

#include "Geometry/simplex.h"
#include "Utility/pre-post-conditions.h"


namespace GrAL {
  template class simplex<0,float>;
  template class simplex<1,float>;
  template class simplex<2,float>;
  template class simplex<3,float>;
  template class simplex<4,float>;
  
  template class simplex<0>;
  template class simplex<1>;
  template class simplex<2>;
  template class simplex<3>;
  template class simplex<4>;
}

int main() 
{
  using namespace GrAL;

  simplex<3> s3A(3, simplex<2>(2, simplex<1>(1,simplex<0>(0))));
  simplex<3> s3B(3,2,1,0);
  simplex<3> s3C = s3A;
  simplex<3> s3D; s3D = s3B;

  {
    simplex<1> s1(1,2);
    simplex<1> s2(2,1);

    REQUIRE_ALWAYS( equal_ordered  ::equal(s1,s2), "", 1);
    REQUIRE_ALWAYS(!equal_unordered::equal(s1,s2), "", 1);
    REQUIRE_ALWAYS(!compare_ordered  ::less(s1,s2), "", 1);
    REQUIRE_ALWAYS( compare_unordered::less(s1,s2), "", 1);

    REQUIRE_ALWAYS( (s1 == s2), "", 1);
    REQUIRE_ALWAYS(!(s1 != s2), "", 1);
    REQUIRE_ALWAYS(!(s1 <  s2), "", 1);
    REQUIRE_ALWAYS(!(s1 >  s2), "", 1);

    simplex<1> s3(3,0);

    REQUIRE_ALWAYS(!equal_ordered  ::equal(s1,s3), "", 1);
    REQUIRE_ALWAYS(!equal_unordered::equal(s1,s3), "", 1);
    REQUIRE_ALWAYS(!compare_ordered  ::less(s1,s3), "", 1);
    REQUIRE_ALWAYS( compare_unordered::less(s1,s3), "", 1);

    REQUIRE_ALWAYS(!(s1 == s3), "", 1);
    REQUIRE_ALWAYS( (s1 != s3), "", 1);
    REQUIRE_ALWAYS(!(s1 <  s3), "", 1);
    REQUIRE_ALWAYS(!(s3 >  s1), "", 1);

    simplex<1> s4(3,1);

    REQUIRE_ALWAYS(!equal_ordered  ::equal(s1,s4), "", 1);
    REQUIRE_ALWAYS(!equal_unordered::equal(s1,s4), "", 1);
    REQUIRE_ALWAYS( compare_ordered  ::less(s1,s4), "", 1);
    REQUIRE_ALWAYS( compare_unordered::less(s1,s4), "", 1);

    REQUIRE_ALWAYS(!(s1 == s4), "", 1);
    REQUIRE_ALWAYS( (s1 != s4), "", 1);
    REQUIRE_ALWAYS( (s1 <  s4), "", 1);
    REQUIRE_ALWAYS( (s4 >  s1), "", 1);

  }
}

