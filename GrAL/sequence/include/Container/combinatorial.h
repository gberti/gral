#ifndef GRAL_GB_SEQUENCE_COMBINATORIAL_H
#define GRAL_GB_SEQUENCE_COMBINATORIAL_H

/*! \defgroup combinatorialalgorithms Combinatorial algorithms
    \brief Algorithms calculating combinatorial functions, such as binomial coefficients


*/

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

#endif
