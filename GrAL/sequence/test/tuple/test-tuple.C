/*! \file

    Tests for \c tuple<T,N>
*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

namespace GrAL {
  template class tuple<int, 0>;
  template class tuple<int, 1>;
  template class tuple<int, 2>;
  template class tuple<int, 3>;
  template class tuple<int, 4>;
}


int main() {
  using namespace GrAL;
  tuple<int, 0> ti0;

  tuple<int,   3> ti3(1,1,2);
  tuple<float, 3> tf3(1.5,1.5,1.6);

  {
    tuple<float, 3> tf3b(ti3.begin(), ti3.end());  
    tuple<float, 3> tf3c(ti3);
    tuple<float, 3> tf3d; tf3d = ti3;
  }



  {
    int x[2] = { 0, 1 };
    tuple<int, 2>   ti2a(0);
    tuple<int, 2>   ti2b(x);
    tuple<int, 2>   ti2c(x, x+2);
    tuple<float, 2> tf2e(ti2a);
    // Copy from generic iterator pair has been disabled
    // for 2D, to avoid ambiguities with constructor t(T t1, T t2)
    // if actual arguments are of a different type and need implicit conversion
    // tuple<float, 2> tf2f(ti2a.begin(), ti2a.end());
    tuple<float, 2> tf2g(tf2e.begin(), tf2e.end());
    //  tuple<float, 2> tf2h(x,x+2);
  }



  {
    int x[3] = { 0, 1, 2 };
    tuple<int, 3>   ti3a(0);
    tuple<int, 3>   ti3b(x);
    tuple<int, 3>   ti3c(x, x+3);
    tuple<float, 3> tf3b(tf3.begin(), tf3.end());  
    tuple<float, 3> tf3c(tf3);
    tuple<float, 3> tf3d; tf3d = tf3;
    tuple<float, 3> tf3e(ti3a);
    tuple<float, 3> tf3f(ti3a.begin(), ti3a.end());
    tuple<float, 3> tf3g(tf3b.begin(), tf3b.end());
    tuple<float, 3> tf3h(x,x+3);
  }

  {
    int x[4] = { 0, 1, 2, 3 };
    tuple<int, 4>    ti4a(0);
    tuple<int, 4>    ti4b(x);
    tuple<int, 4>    ti4c(x, x+4);
    tuple<double, 4> td4a(ti4a);
    tuple<double, 4> td4b(ti4a.begin(), ti4a.end());
    tuple<double, 4> td4c(td4b.begin(), td4b.end());
    tuple<double, 4> td4d(x, x+4);
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
    REQUIRE_ALWAYS(inverse(TF(2,4)) == TF(0.5,0.25), "", 1);

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

    REQUIRE_ALWAYS(max_tuple(TI(1,1), TI(2,0)) == TI(2,1), "", 1);
    REQUIRE_ALWAYS(min_tuple(TI(0,1), TI(2,0)) == TI(0,0), "", 1);
  }
}
