/*! \file

    Tests for \c tuple<T,N>
*/

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

template class tuple<int, 0>;
template class tuple<int, 1>;
template class tuple<int, 2>;
template class tuple<int, 3>;
template class tuple<int, 4>;


int main() {
  tuple<int, 0> ti0;

  tuple<int,   3> ti3(1,1,2);
  tuple<float, 3> tf3(1.5,1.5,1.6);

  {
    tuple<float, 3> tf3b(ti3.begin(), ti3.end());  
    tuple<float, 3> tf3c(ti3);
    tuple<float, 3> tf3d; tf3d = ti3;
  }

  {
    tuple<float, 3> ti3b(tf3.begin(), tf3.end());  
    tuple<float, 3> ti3c(tf3);
    tuple<float, 3> ti3d; ti3d = tf3;
  }
  

  {
    tuple<int,   3> ti3b = product(ti3,ti3);
    tuple<float, 3> tf3c = product(ti3,tf3);
    tuple<float, 3> tf3d = product(tf3,ti3);
  }

  {
    tuple<int, 2> t1(1,1);
    tuple<int, 2> t2(1,0);
    tuple<int, 2> t3(1,1);
    tuple<int, 2> t4(0,0);

    REQUIRE_ALWAYS( (t1 == t3), "",1);
    REQUIRE_ALWAYS( (t1 != t2), "",1);
    REQUIRE_ALWAYS( (t1 != t4), "",1);   

    REQUIRE_ALWAYS( (t1 >= t3), "",1);
    REQUIRE_ALWAYS( (t1 <= t3), "",1);

    REQUIRE_ALWAYS( (t1 >  t4), "",1);
    REQUIRE_ALWAYS( (t4 <  t1), "",1);
  }

  {
    typedef tuple<int,2> TI;
    typedef tuple<float, 2> TF;    

    REQUIRE_ALWAYS(product (TI(1,2), TI(3,4)) == TI(3,8), "",1);
    REQUIRE_ALWAYS(quotient(TI(3,8), TI(3,4)) == TI(1,2), "",1);

    REQUIRE_ALWAYS(floor_tuple(TF(0.5, 0.5)) == TF(0.0, 0.0), "",1);
    REQUIRE_ALWAYS(ceil_tuple (TF(0.5, 0.5)) == TF(1.0, 1.0), "",1);
    REQUIRE_ALWAYS(round_tuple(TF(0.6, 0.6)) == TF(1.0, 1.0), "",1);
    REQUIRE_ALWAYS(round_tuple(TF(0.4, 0.4)) == TF(0.0, 0.0), "",1);
    
    REQUIRE_ALWAYS( does_divide(TI(2,3), TI(2,6)), "", 1);
    REQUIRE_ALWAYS(!does_divide(TI(2,3), TI(2,5)), "", 1);

    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(0,0))  == TI(0,0), "res == " << clamp_tuple(TI(0,0), TI(1,1), TI(0,0)), 1);
    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(0,1))  == TI(0,1), "", 1);
    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(1,1))  == TI(1,1), "", 1);
    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(-1,1)) == TI(0,1), "", 1);
    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(-1,-1)) == TI(0,0), "", 1);
    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(-1,2)) == TI(0,1), "", 1);
    REQUIRE_ALWAYS(clamp_tuple(TI(0,0), TI(1,1), TI(2,2))  == TI(1,1), "", 1);
  }
}
