/*! \file

*/


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/counting-iterator.h"

#include <algorithm>
#include <iostream>

namespace GrAL {
  template class counting_output_iterator<int>;
}



int main() 
{
  int a[] = { 1, 2, 3};
  int n;

  n = * std::copy(a+0, a+0, GrAL::counter(0));
  std::cout << n << " elements copied " << std::endl;
 
  n = * std::copy(a+0, a+3, GrAL::counter(0));
  std::cout << n << " elements copied " << std::endl;

  int b[] = { 3, 4 };
  n = * std::copy(a+0, a+3, std::copy(b+0, b+2, GrAL::counter(0)));
  std::cout << n << " elements copied " << std::endl;
  

  n = * std::set_difference(a+0, a+3,
			    b+0, b+2,
			    GrAL::counter(0));
  std::cout << n << " elements in a \\ b " << std::endl;
  n = * std::set_difference(a+0, a+3,
			    a+0, a+3,
			    GrAL::counter(0));
  std::cout << n << " elements in a \\ a " << std::endl;


 
}
