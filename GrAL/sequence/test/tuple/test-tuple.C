#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

template tuple<int, 0>;
template tuple<int, 1>;
template tuple<int, 2>;
template tuple<int, 3>;
template tuple<int, 4>;


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
}
