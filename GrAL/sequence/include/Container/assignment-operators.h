#ifndef GRAL_GB_ASSIGNMENT_OPERATORS_H
#define GRAL_GB_ASSIGNMENT_OPERATORS_H


// $LICENSE

/*!  \defgroup operators Operator classes
     \ingroup accessors
*/

/*! \brief Assignment operators for parameterizing generic functions
    \ingroup operators
*/
namespace assignment_operators {

//! \ingroup operators 

struct assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls = rs;}
};

//! \ingroup operators 


struct add_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls += rs;}
};

//! \ingroup operators 

struct sub_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls -= rs;}
};

//! \ingroup operators 

struct mult_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls *= rs;}
};

//! \ingroup operators 

struct div_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls /= rs;}
};


} // namespace assigment_operators

#endif
