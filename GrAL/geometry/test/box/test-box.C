/*! \file
    \brief Test case for box<COORD> class template.

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Geometry/box.h"
#include "Geometry/coords.h"
#include "Utility/pre-post-conditions.h"
#include "Container/tuple-point-traits.h"

#include <iostream>
#include <iomanip>


template<class COORD>
inline void  print_contains(GrAL::box<COORD> const& b, COORD const& p)
{
  std::cout << " box [" << b.the_min() << ";" << b.the_max() << "] " 
	    << (b.contains(p) ? "contains " : "does not contain ")
	    << " point " << p << std::endl;
}

int main() {
  using namespace GrAL;
  using namespace std;
  typedef coordN<2> coord_type;
  typedef box<coord_type> box_type;
  
  cout << "+numeric_limits<float> ::max() =  " << setprecision(numeric_limits<float> ::digits10) <<  numeric_limits<float>::max() << endl
       << "-numeric_limits<float> ::max() = " << setprecision(numeric_limits<float> ::digits10) << -numeric_limits<float>::max() << endl
       << "+numeric_limits<double>::max() =  " << setprecision(numeric_limits<double>::digits10) <<  numeric_limits<double>::max() << endl
       << "-numeric_limits<double>::max() = " << setprecision(numeric_limits<double>::digits10) << -numeric_limits<double>::max() << endl;


  coord_type p_1(-1,-1);
  coord_type p0 (0,0);
  coord_type p1 (1,1);

  box_type b_10(p_1,p0);     REQUIRE_ALWAYS(!b_10.empty(), "",1);
  box_type b0_1(p0 ,p_1);    REQUIRE_ALWAYS(!b0_1.empty(), "",1);
  box_type b_11(p_1,p1);     REQUIRE_ALWAYS(!b_11.empty(), "",1);
  box_type b1_1(p1,p_1);     REQUIRE_ALWAYS(!b1_1.empty(), "",1);
  box_type b01(p0,p1);

  box_type be;               REQUIRE_ALWAYS(be.empty(), "", 1);
  
  print_contains(b_10, p0);
  print_contains(b_10, p_1);
  print_contains(b0_1, p0);
  print_contains(b0_1, p_1);
  print_contains(b_11, p0);
  print_contains(b1_1, p0);

  box_type B0(p0);                                REQUIRE_ALWAYS(!B0  .empty(),"",1); 
  box_type B_11(box_type(p_1) | box_type(p1) );   REQUIRE_ALWAYS(!B_11.empty(),"",1); 
  box_type B1_1(box_type(p1)  | box_type(p_1));   REQUIRE_ALWAYS(!B1_1.empty(),"",1); 
  
  print_contains(B0,   p0);  
  print_contains(B_11, p0);
  print_contains(B1_1, p0);

  
  REQUIRE_ALWAYS( b_10.contains(p0),  "",1);  
  REQUIRE_ALWAYS( b_10.contains(p_1), "",1);  
  REQUIRE_ALWAYS( b0_1.contains(p0),  "",1);  
  REQUIRE_ALWAYS( b0_1.contains(p_1), "",1);  




  REQUIRE_ALWAYS( b_11.contains(p0), "", 1);
  REQUIRE_ALWAYS( b1_1.contains(p0), "", 1);

  REQUIRE_ALWAYS(  (b_11 & b_10).contains(p0), "", 1);
  REQUIRE_ALWAYS( !(b_11 & b_10).contains(p1), "", 1);

  REQUIRE_ALWAYS(  (b01 | b_10).contains(p0),  "", 1);
  REQUIRE_ALWAYS(  (b01 | b_10).contains(p_1), "", 1);
  REQUIRE_ALWAYS(  (b01 | b_10).contains(p1),  "", 1);

  REQUIRE_ALWAYS( ! (be & b_11).contains(p0), "", 1);
  REQUIRE_ALWAYS( ! (be & b_11).contains(p_1), "", 1);
  REQUIRE_ALWAYS( ! (be & b_11).contains(p1), "", 1);
  

  print_contains( be & b_11, p0);
  print_contains( be | b_11, p0);

  cout << "Volume be  : " << be   .volume() << "\n"; 
  cout << "Volume b_11: " << b_11.volume() << "\n"; 
  cout << "Volume b_01: " << b01 .volume() << "\n"; 

  typedef coord_type ct;
  ct points[] = { ct(1.0,1.0), ct(2.0,1.0), ct(2.0, 2.0), ct(1.0, 2.0)};
  
  box<coord_type> b;
  b.init(points+0, points+sizeof(points)/sizeof(ct));
  REQUIRE_ALWAYS(b.the_min() == ct(1.0,1.0), " b.the_min()=" << b.the_min(), 1);
  REQUIRE_ALWAYS(b.the_max() == ct(2.0,2.0), " b.the_max()=" << b.the_max(), 1);

  box_type b_empty     = box_type::empty_box();
  box_type b_infinite  = box_type::infinite_box();

  cout << "b_empty:    " << b_empty.min()    << " " << b_empty.max()    << "\n"
       << "b_infinite: " << b_infinite.min() << " " << b_infinite.max() << std::endl;

  {
    typedef tuple<double,2> coord_type_2;
    coord_type_2 p0(0.0,0.0);
    coord_type_2 p1(1.0,2.0);

    box<coord_type_2> b2(p0,p1);
    box_type b2_copy = convert<coord_type>(b2);
    REQUIRE_ALWAYS(b2_copy.min() == convert_point<coord_type>(b2.min()), "", 1);
    REQUIRE_ALWAYS(b2_copy.max() == convert_point<coord_type>(b2.max()), "", 1);
  }

}
