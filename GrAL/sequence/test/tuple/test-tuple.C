#include "Container/tuple.h"

int main() {
  tuple<int,   3> ti3(1,1,2);
  tuple<float, 3> tf3(1.5,1.5,1.6);
  
  tuple<int,   3> ti3b = product(ti3,ti3);
  tuple<float, 3> tf3b = product(ti3,tf3);
  tuple<float, 3> tf3c = product(tf3,ti3);

}
