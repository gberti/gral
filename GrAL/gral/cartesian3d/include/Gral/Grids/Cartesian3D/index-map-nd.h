#ifndef GRAL_GB_CARTESIAN3D_INDEXMAP_ND_H
#define GRAL_GB_CARTESIAN3D_INDEXMAP_ND_H

// $LICENSE

#include "Container/tuple.h"

/*! Map between \f$ [0,n_0]\times[0,n_1]\times \cdots \times[0,n_{N-1}] \subset \Z^N \f$ 
    and \f$ [0,n_0 n_1  \cdots n_{N-1} -1] \subset \Z\f$

    Highest 1D index \f$ n_{N-1} \f$ variest fastest.
    \f[
     p = (p_0, \ldots, p_{N-1}) \mapsto \sum_{k=0}^N-1 p_k \prod_{j=k+1}^{N-1} n_j
    \f]

    \todo Create test cases.
    \todo Check if some coordinates are 0.
    \todo The name index_type may be misleading: 
       Normally an n-dimensional multi_index is meant here.
    \todo Move to sequence module
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
  /*! Construct map range \f$ [0,n_0]\times[0,n_2]\times \cdots \times[0,n_{N-1}] \fN

      Some $n_i$ may be zero. 
      If some $n_i < 0$, the range is empty, and using the mapping operators will result in an error
      (there is no valid input for them).
   */
  index_map_nd(index_type const& nn) : n(nn)
    {
      // REQUIRE(non_negative(n), "n = " << n, 1);
      prod[N-1] = 1;
      for(int k = N-2; k >= 0; --k) {
	// sort out degenerate cases:
	// n[k+1] == 0 is valid, will not change produkt: nk1 = 1
	// n[k+1] <  0 means empty range: nk1 = 0,=> prod[0] = 0
	int nk1 = (n[k+1] > 0 ? n[k+1] ( n[k+1] == 0 ? 1 : 0));
	prod[k] = prod[k+1]*nk1;
      }
    }
  // the next two operators could probably be made more
  // efficient by using template metaprogramming
 int operator()(index_type const& p) const
    { 
      c(p);
      int res = 0;
      for(int k = N-1; k >= 0; --k) {
	res += prod[k]*p[k];
      }
      return res;
    }

  index_type operator()(int i) const 
    {
      c(i);
      index_type res;
      int remainder = i;
      for(unsigned k = 0; k < N; ++k) {
	if(n[k] == 0) // 'flat' dimension: skip
	  res[k] = 0;
	else
	  res[k] = remainder / prod[k];
	remainder -= prod[k]*res[k]; 
	// prod[N-1] = 1 => remainder = 0 for k = N-1
      }
      return res;
    }

  bool valid(index_type const& p) const {
    bool res = true;
    for(int k = 0; k < N; ++k) 
      res = res && ( 0 <= p[k] && p[k] < n[k]);
    return res;
  }
  bool valid(int i) const {
    return (0 <= i && i <= max_flat_index());
  }
  bool non_negative(index_type const& n) const {
    bool res = true;
    for(int k = 0; k < N; ++k)
      res = res && (n[k] >= 0);
    return res;
  }

  index_type max_tuple() const { 
    index_type mx;
    for(unsigned k = 0; k < N; ++k)
      mx[k] = n[k]-1;
    return mx;
  } 
  int        min_flat_index() const { return 0;}
  int        max_flat_index() const { return n[0]*prod[0] -1;}

private:
  void c(index_type const& p) { REQUIRE(valid(p), "p = " << p, 1);}
  void c(int               i) { REQUIRE(valid(i), "i = " << i, 1);}
};




#endif





