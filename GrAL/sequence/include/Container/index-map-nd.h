#ifndef GRAL_GB_CARTESIAN3D_INDEXMAP_ND_H
#define GRAL_GB_CARTESIAN3D_INDEXMAP_ND_H

// $LICENSE

#include "Container/tuple.h"

/*! Map between \f$ [0,n_1]\times[0,n_2]\times \cdots \times[0,n_N] \subset \Z^N \f$ 
    and \f$ [0,n_1 n_2  \cdots n_N -1] \subset \Z\f$

    Highest 1D index \f$ n_N \f$ variest fastest

    \todo Create test cases.
    \todo Check if some coordinates are 0.
    \todo index_type may be misleading: 
       Normally a n-dimensional multi_index is meant here.
 */
template<unsigned N>
class index_map_nd {
 public:
 
  typedef tuple<int,N> index_type;
 private:
  index_type n;
  index_type prod; // prod[k] = n[k+1]* ... *n[N-1]

 public:
  index_map_nd() {} 
  index_map_nd(index_type const& nn) : n(nn)
    {
      prod[N-1] = 1;
      for(int k = N-2; k >= 0; --k) {
	prod[k] = prod[k+1]*n[k+1];
      }
    }
  // the next two operators could probably be made more
  // efficient by using template metaprogramming
 int operator()(index_type const& p) const
    { 
      int res = 0;
      for(int k = N-1; k >= 0; --k) {
	res += prod[k]*p[k];
      }
      return res;
    }

  index_type operator()(int i) const 
    {
      index_type res;
      int remainder = i;
      for(unsigned k = 0; k < N; ++k) {
	res[k] = remainder / prod[k];
	remainder -= prod[k]*res[k]; 
	// prod[N-1] = 1 => remainder = 0 for k = N-1
      }
      return res;
    }

  index_type const& max_tuple() const { return n;} 
  int        min_flat_index() const { return 0;}
  int        max_flat_index() const { return n[0]*prod[0] -1;}
};


#endif
