#ifndef GRAL_GB_GEOMETRY_SPLINE_H
#define GRAL_GB_GEOMETRY_SPLINE_H

// $LICENSE_NEC_2007

/*! \file 

*/


#include "Geometry/polynomial.h"
#include "Geometry/piecewise-function.h"



namespace GrAL {

  /*! \brief produce a spline fulfilling special boundary conditions 

     Returns a cubic spline \e s consisting of 2 cubic polynomials such that
     \f$ s(0) = s0, s'(0) = ds0, s''(0) = dds(0), s'(2) = ds2, s''(2) = dds2 \f$.

     \Note Implementation:
     We have two cubic splines \f$s_1, s_2\f$ with \f$ s_i(t) = a_it^3 +b_it^2 + c_it + d_i\f$ and \f$ s_2 \f$,
     s.t. \f{eqnarray*}
        s_1(0) &=& s0 = d_1 \\
        s_1'(0) &=& ds0 = c_1 \\
        s_1''(0) &=& dds0 = 2b_1 \\
        s_1(1) &=& s_2(0) \Rightarrow a_1 + b_1 + c_1 + d_1 = d_2 \\
        s_1'(1) &=& s_2'(0) \Rightarrow 3a_1+2b_1+c_1 = c_2 \\
        s_1''(1) &=& s_2''(0) \Rightarrow 6a_1 + 2b_1 = 2b_2 \\
	s_2'(1) &=& ds2 = c_2 + 2b_2 + 3 a_2 \\
	s_2''(1) &=& dds2 = 2b_2 + 6 a_2 
     \f}
  */
  piecewise_function<polynomial<3,double> > make_spline(double s0, double ds0, double dds0,
							double ds2, double dds2)
  {
    double d_1 = s0;
    double c_1 = ds0;
    double b_1 = 0.5*dds0;
    double a_1 = (1.0/12)*(        -2*ds0 -3*dds0 + 2*ds2 -1*dds2);
    double d_2 = (1.0/12)*(12*s0 + 10*ds0 +3*dds0 + 2*ds2 -1*dds2);
    double c_2 = (1.0/12)*(         6*ds0 +3*dds0 + 6*ds2 -3*dds2);
    double b_2 = (1.0/12)*(        -6*ds0 -3*dds0 + 6*ds2 -3*dds2);
    double a_2 = (1.0/12)*(         2*ds0 +1*dds0 - 2*ds2 +3*dds2);

    typedef polynomial<3,double> poly_type;
    typedef poly_type::coefficients_type ct;
    double co1[] = { d_1, c_1, b_1, a_1 };
    double co2[] = { d_2, c_2, b_2, a_2 };
    poly_type::coefficients_type coeff1(co1, co1 + 4);
    poly_type::coefficients_type coeff2(co2, co2 + 4);
    poly_type p[2] = { poly_type(ct(co1, co1 + 4)), poly_type(ct(co2,co2+4)) };
    return piecewise_function<poly_type>(p,p+2);
  }

  /*! \brief produce a quartic fulfilling special boundary conditions 

     Returns a quartic \e s 
     \f$ s(0) = s0, s'(0) = ds0, s''(0) = dds(0), s'(1) = ds1, s''(1) = dds1 \f$.

  */
  template<class T>
  polynomial<4,T> make_quartic(T s0, T ds0, T dds0, T ds1, T dds1)
  {
    typedef polynomial<4,T> poly_type;
    typedef typename poly_type::coefficients_type ct;
    T coeff[5] = { s0, ds0, 
		   T(0.5)  * dds0, 
		   T(1.0/3)*(-dds1 + 3*ds1 - 3*ds0 -2*dds0), 
		   T(0.25)  * ( dds1 - 2*ds1 + 2*ds0 +  dds0) };
    return poly_type(ct(coeff, coeff+5));
  }
  

};

#endif
