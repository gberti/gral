#ifndef NMWR_GB_BIJECTIVE_MAPPING_C
#define NMWR_GB_BIJECTIVE_MAPPING_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/bijective-mapping.h"

namespace GrAL {

template<class T1, class T2, class Policy>
void write_bm(bijective_mapping<T1,T2,Policy> const& m, ::std::ostream& out)
{
  typedef typename bijective_mapping<T1,T2,Policy>::domain_type dt;
  typedef typename dt::const_iterator                    d_iter;
  for(d_iter x = m.domain().begin(); ! (x == m.domain().end()); ++x)
    out  << *x << ' ' << m(*x) << '\n';
}

template<class T1, class T2, class Policy>
void read_bm(bijective_mapping<T1,T2,Policy> & m, ::std::istream& in)
{
  T1 x;
  T2 mx;
  while(in >> x >> mx) {
    m[x] = mx;
  }
}

template<class T1, class T2, class Policy>
void bijective_mapping<T1,T2,Policy>::calculate_inverse() const
{
  for(map_iterator it = the_map.begin(); !(it == the_map.end()); ++it)
    ((inv_table_type &)the_inverse_map)[(*it).second] = (*it).first;
}

template<class T1, class T2, class Policy>
bijective_mapping<T1,T2,Policy>::bijective_mapping(inverse_mapping<T2,T1,Policy> const& inv)
  : the_map(inv.domain().size()),
    inverse_ok(false)
   
{
  typedef typename inverse_mapping<T2,T1,Policy>::domain_type inv_domain_type;
  typedef typename inv_domain_type::const_iterator ran_iter;

  for(ran_iter x = inv.domain().begin(); !(x ==  inv.domain().end()); ++x)
    the_map[*x] = inv(*x);
}

} // namespace GrAL 
#endif
