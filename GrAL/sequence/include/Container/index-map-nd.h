#ifndef GRAL_GB_SEQUENCE_INDEXMAP_ND_H
#define GRAL_GB_SEQUENCE_INDEXMAP_ND_H

// $LICENSE

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"
#include "Utility/pre-post-conditions.h"


namespace GrAL {

/*! \brief Map between \f$ [l_0,b_0[ \times [l_1,b_1[ \times \cdots \times [l_{N-1},b_{N-1}[ \subset \Z^N \f$ 
    and \f$ [0,n_0 n_1  \cdots n_{N-1} -1] \subset \Z\f$ (where \f$ n_i = b_i - l_i \f$)

    Highest 1D index \f$ n_{N-1} \f$ variest fastest.
    \f[
     p = (p_0, \ldots, p_{N-1}) \mapsto \sum_{k=0}^{N-1} p_k \prod_{j=k+1}^{N-1} n_j
    \f]

    Tests in test-index-map-nd.C

    \note The name index_type may be misleading: 
           In this class, an N-dimensional multi-index is meant,
           whereas a linear  (1D) index is called flat index.
 */
template<unsigned N>
class index_map_nd {
 public:
 
  typedef tuple<int,N> index_type;
 private:
  index_type low;
  index_type n;
  index_type prod; // prod[k] = n[k+1]* ... *n[N-1]
  
 public:
  /*! \brief Construct empty map */
  index_map_nd() : low(0), n(0) { init(); } 

  /*! \brief Construct map range \f$ [0,n_0[\times[0,n_1[\times \cdots \times[0,n_{N-1}[ \f$

      Some \f$n_i\f$ may be zero, then the range is empty 
      and using the mapping operators will result in an error
      (there is no valid input for them).
   */
  index_map_nd(index_type const& nn) : low(0), n(nn) { init();}
 /*! \brief Construct map range \f$ [l_0,b_0[\times[l_1,b_1[\times \cdots \times[l_{N-1},b_{N-1}[ \f$
   
      Here \f$l = \f$ \c low_ and \f$ b = \f$ \c beyond_. 
      If \f$ b_i - l_i \leq 0\f$ for some \f$ i \f$, the range is empty,
      and using the mapping operators will result in an error
      (there is no valid input for them).
   */

  index_map_nd(index_type const& low_,
	       index_type const& beyond_) : low(low_), n(beyond_-low_) {init();}
private:
  void init()
    {
      prod[N-1] = 1;
      for(int k = (int)N-2; k >= 0; --k) {
	// sort out degenerate cases:
	// n[k+1] <= 0 means empty range: nk1 = 0,=> prod[0] = 0
	int nk1 = (n[k+1] > 0 ? n[k+1] : 0);
	prod[k] = prod[k+1]*nk1;
      }
    }

public:
  // the next two operators could probably be made more
  // efficient by using template metaprogramming
  /*! \name Mapping function  */

  /*@{*/
  /*! \brief Map N-dimensional index \c p to flat index
      \pre \c valid(p)
      \post 
      \code
       index_map_nd<N>             map; 
       index_map_nd<N>::index_type it;
       it == map(map(it));
      \endcode
   */
 int operator()(index_type const& p) const
    { 
      c(p);
      int res = 0;
      index_type pp = p - min_tuple();
      for(int k = N-1; k >= 0; --k) {
	res += prod[k]*pp[k];
      }
      return res;
    }
  /*! \brief Map flat index \c i to N-dimensional index
   */
  index_type operator()(int i) const 
    {
      c(i);
      index_type res;
      int remainder = i;
      for(unsigned k = 0; k < N; ++k) {
	if(n[k] == 1) // 'flat' dimension: skip
	  res[k] = 0;
	else
	  res[k] = remainder / prod[k];
	remainder -= prod[k]*res[k]; 
	// prod[N-1] = 1 => remainder = 0 for k = N-1
      }
      return res + min_tuple();
    }
  /*@}*/

  /*! \name Validity checks */
  /*@{*/

  /*! \brief True if \p is valid: \f$ min_tuple()[k] \leq \f$ p[k] \f$ \leq \f$ \c max_tuple()[k] \f$ 0 \leq k \leq N-1 \f$ */
  bool valid(index_type const& p) const {
    bool res = true;
    for(unsigned k = 0; k < N; ++k) 
      res = res && ( low[k] <= p[k] && p[k] < low[k]+n[k]);
    return res;
  }
  /*! \brief True if \c i is valid: \f$ 0 \leq \f$ \c i \f$ \leq \f$ max_flat_index() */
  bool valid(int i) const {
    return (0 <= i && i <= max_flat_index());
  }
  /*@}*/


  /*! \name Bounds accessors */
  /*@{*/

  /*! \brief Maximal valid N-dimensional index \f$ (n_0-1, \ldots, n_{N-1}-1)\f$
   */
  index_type max_tuple() const { 
    index_type mx;
    for(unsigned k = 0; k < N; ++k)
      mx[k] = low[k]+n[k]-1;
    return mx;
  } 
  /*! \brief Minimal valid N-dimensional index  */
  index_type min_tuple() const { return low;}
  /*! \brief Index one beyond the maximal admissible index in each dimension. */
  index_type beyond_tuple() const { return low + n;}
  /*! \brief N-dimensional size 
      
       \c size()[i] gives the number of valid entries in the ith direction. 
       If \c size()[i] \f$ \leq 0 \f$ for some \c i, the range is empty.
    */
  index_type size() const { return n;}

  /*! \brief Minimal flat index, = 0 */
  int        min_flat_index() const { return 0;}
  /*! \brief Maximal flat index, = \f$ \prod_{k=0}^{N-1} n_k -1 \f$ */
  int        max_flat_index() const { return n[0]*prod[0] -1;}
  /*! \brief Linear size */
  unsigned flat_size() const { return 1+ max_flat_index() - min_flat_index();}
  /*@}*/

private:
  void c(index_type const& p) const { REQUIRE(valid(p), "p = " << p, 1);}
  void c(int               i) const { REQUIRE(valid(i), "i = " << i, 1);}
};

} // namespace GrAL 


#endif





