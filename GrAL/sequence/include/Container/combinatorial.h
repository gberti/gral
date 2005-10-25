#ifndef GRAL_GB_SEQUENCE_COMBINATORIAL_H
#define GRAL_GB_SEQUENCE_COMBINATORIAL_H

// $LICENSE_NEC

#include "Utility/pre-post-conditions.h"

namespace GrAL { namespace combinatorial {

/*! \defgroup combinatorialalgorithms Combinatorial algorithms
    \brief Algorithms calculating combinatorial functions, such as binomial coefficients


*/

/*! \brief factorial
  \ingroup combinatorialalgorithms

  \todo Test for overflow
 */
template<class I>
I factorial(I n) {
  REQUIRE_ALWAYS(n >= 0, " n=" << n,1);
  return (n == 0 ? 1 :n*factorial(n-1));
}

template<class I>
I power(I base, unsigned n) 
{
  if(n==0)
    return 1;
  I power_n_2 = power(base, n/2);
  return power_n_2 * power_n_2 * ( (2*(n/2) == n) ? 1 : base);
}

namespace compile_time_functions {

/*! \brief Compile-time power
    \ingroup combinatorialalgorithms

    \todo Test for overflow
 */
template<unsigned BASE, unsigned EXP>
struct power {
  enum { value = BASE * power<BASE,EXP-1>::value};
};

template<unsigned BASE>
struct power<BASE,0> {
  enum { value = 1};
};

  /*! \brief Compile-time factorial 
   */
template<unsigned N>
struct factorial {
  enum { value = N * factorial<N-1>::value};
};

template<>
struct factorial<0> {
  enum { value = 1};
};

} // compile_time_functions

namespace compile_time_functions {
/*! \brief Compile time binomial coefficient
    \ingroup combinatorialalgorithms

*/
template<unsigned N, unsigned K>
struct binomial_coefficient { enum { value = binomial_coefficient<N-1,K>  ::value
				     +       binomial_coefficient<N-1,K-1>::value}; };
				     
template<unsigned N>
struct binomial_coefficient<N,0> { enum { value=1};};

template<unsigned N>
struct binomial_coefficient<N,N> { enum { value=1};};

} // compile_time_functions



/*! \brief Runtime recursive binomial coefficient
    \ingroup combinatorialalgorithms

  \note This uses inefficient recursive runtime evaluation
        It is not suitable for larger numbers.
*/
inline 
unsigned binomial_coeff_simple_minded(unsigned n, unsigned k)
{
  REQUIRE(n >= k, "n=" << n << " k=" << k, 1);
  return ( k == 0 ? 
	   1 : 
	   ( n == k ?  
	     1 : 
	     binomial_coeff_simple_minded(n-1,k)  
	     +
	     binomial_coeff_simple_minded(n-1,k-1)
	     )
	   );
}


/*! \brief Runtime binomial coefficient calculation
    \ingroup combinatorialalgorithms

    \note This is intended to be a poly-algorithm choosing the most efficient version
          depending on the input.
*/
inline 
unsigned binomial_coeff(unsigned n, unsigned k) { return binomial_coeff_simple_minded(n,k);}


/*! \brief Lexicographic successor of ordered tuple
    \ingroup combinatorialalgorithms

    Return lexicographic successor of an ordered n-tuple v of indices in [0, maxitem].
    If there is no successor, v is returned.
    \pre  v[0] < v[1] < \cdots < v[n-1] <= maxitem
    \post If r is the return value, then 
         <tt> r[0] < r[1] < \cdots < r[n-1] <= maxitem </tt>
         If <tt> v[0] <= maxitem - n </tt> then  \c r is the lexicographic successor of \c v
         in the set of ordered n-tuples over [0,maxitem].

*/

template<class V>
V succ_ordered(V const& v, unsigned maxitem)
{
  V res(v);
  unsigned k = v.size();
  if(k == 0) return res;
  if(res[k-1] < maxitem)
    res[k-1]++;
  else {
    for(int j=k-2; j >= 0; --j) {
      if(res[j] < res[j+1]-1) {
	res[j]++;
	for(unsigned jj = j+1; jj < k; ++jj)
	  res[jj] = res[jj-1]+1;
	break;
      }
    }
  }
  return res;
}




/*! \brief Arithmetic successor of b-ary number 
    \ingroup combinatorialalgorithms
   
   Treat v as a b-ary number \f$ v= (v_{n-1} \ldots v_0)_{b} \f$ 
   and return v+1 (mod base^n)  with n = v.size()
   if all entries are equal to b-1, zero is returned.
*/
template<class V>
V succ_nary_number(V const& v, unsigned b)
{
  unsigned n = v.size();
  REQUIRE( b > 1, "b=" << b,1);
  for(unsigned i = 0; i < n; ++i)
    REQUIRE( (0 <= v[i] && v[i] < b), "v[i]=" << v[i] << " b=" << b, 1);

  V res(v);
  if(n == 0) return res;
  else {
    int i = n-1;
    while( i >= 0  &&  res[i] == b-1) {
      res[i] = 0;
      --i;
    }
    if(i >= 0)
      res[i]++;
  }
  return res;
}

/*! \brief Arithmetic successor of binary number 
    \ingroup combinatorialalgorithms
   
   Treat v as a binary number \f$ v= (v_{n-1} \ldots v_0)_{2} \f$ 
   and return v+1 (mod 2^n)  with n = v.size().
   If all entries are equal to 1, zero is returned.
*/
template<class V>
inline V succ_binary_number(V const& v)
{ return succ_nary_number(v,2);}



template<class I>
struct binary_printer {
  I k;

  binary_printer(I kk) : k(kk) {}

  void print(std::ostream& out) {
    I one = 1;
    for(int i = std::numeric_limits<I>::digits; i >= 1; --i)
      out << ( k & ( one << (i-1)) ? '1' : '0');
  }
  
};

template<class I>
inline std::ostream& operator<<(std::ostream& out, binary_printer<I> b) { b.print(out); return out;}

/*! \brief Print an integral type bit for bit
 */
template<class I>
inline binary_printer<I> print_binary(I k) { return binary_printer<I>(k);}


/*! \brief Mirror the bits of \c x

    \ingroup combinatorialalgorithms

    \f$ x = \sum_{i=0}^N a_i 2^i \mapsto \sum{i=0}^N a_i 2^{N-i} \f$
    where \f$N+1\f$ is the number of bits.
*/

template<class I>
inline I mirror_bits(I x)  {
  I res = 0;
  I N   = std::numeric_limits<I>::digits;
  I two_pow_N_i = power(I(2), N - 1);
  for(unsigned i = 0; i < N; ++i) {
    res += two_pow_N_i * ( x & 1);
    x /= 2;
    two_pow_N_i  /= 2;
  }
  return res;
}



}} // namespace GrAL { namespace combinatorial {


#endif
