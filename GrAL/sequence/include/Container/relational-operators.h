#ifndef GRAL_GB_UTILITY_RELATIONAL_OPERATORS_H
#define GRAL_GB_UTILITY_RELATIONAL_OPERATORS_H

// $LICENSE_NEC

/*! \brief Relational operators for use in templates
   
    \ingroup operators
*/

namespace relational_operators {

  struct eq  { template<class T, class U> bool operator()(T const& t, U const& u) const { return t == u;}};
  struct neq { template<class T, class U> bool operator()(T const& t, U const& u) const { return t != u;}};
  struct lt  { template<class T, class U> bool operator()(T const& t, U const& u) const { return t <  u;}};
  struct gt  { template<class T, class U> bool operator()(T const& t, U const& u) const { return t >  u;}};
  struct leq { template<class T, class U> bool operator()(T const& t, U const& u) const { return t <= u;}};
  struct geq { template<class T, class U> bool operator()(T const& t, U const& u) const { return t >= u;}};

}


#endif
