#ifndef NMWR_GB_OPERATORS_H
#define NMWR_GB_OPERATORS_H


// $LICENSE



/*!  \defgroup operators Operator classes
     \ingroup accessors
*/

//! \ingroup operators 

template<class T>
struct assign {
  void operator()(T& ls, const T& rs) { ls = rs;}
};

//! \ingroup operators 

template<class T>
struct add_assign {
  void operator()(T& ls, const T& rs) { ls += rs;}
};

//! \ingroup operators 

template<class T>
struct mult_assign {
  void operator()(T& ls, const T& rs) { ls *= rs;}
};


#endif
