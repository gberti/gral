/*! \file Test cases for simplex<> class template
    
*/

#include "Geometry/simplex.h"

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

int main() 
{
  simplex<3> s3A(3, simplex<2>(2, simplex<1>(1,simplex<0>(0))));
  simplex<3> s3B(3,2,1,0);
  simplex<3> s3C = s3A;
  simplex<3> s3D; s3D = s3B;
}

