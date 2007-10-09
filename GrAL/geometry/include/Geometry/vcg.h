#ifndef GRAL_GB_GEOMETRY_VCG_H
#define GRAL_GB_GEOMETRY_VCG_H

// $LICENSE_NEC_2006

#include "Geometry/differentiation.h"

#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/geometric-primitives.h"


namespace GrAL {


  /*! \defgroup vcg_grp Volume Constructive Geometry \experimental
      \brief Framework for solid modeling based on pseudo distance functions


      A pseudo distance function (pdf) \f$ f \f$  for a given body \f$ \Omega \f$ 
      satisfies
       - \f$ f(x) < 0 \quad  \forall x \in \Omega \f$
       - \f$ f(x) = 0  \quad \forall x \in \bd \Omega \f$
       - \f$ f(x) > 0  \quad \forall x \in \R^n \setminus \Omega \f$

      Ideally, we would like \f$ f(x) = dist(x,\bd \Omega) \f$, 
      but this can usually be achieved only for simple primitives like spheres.
      

      \note There seems to be no good definition for a pseudo distance function.
      We can try to require some monotony property: for each path \f$ \gamma \f$
      with \f$ f \circ \gamma \f$ monotonically increasing, 
      we require that also \f$ dist \circ \gamma \f$ is monotonically increasing.
      Or the other way around? 
      None of it is true: union of 2 halfspaces, there is a wedge
      where dist is dist. to corner / intersection of boundaries, f is parallel to boundary.
      Then moving along dist isolines will cut f isolines, and vice versa.

      But for each non-singular point x, there should be a cone of monotonic directions 
      simulataneously for both f and dist. 
      Limit:  \f$ lim_{n->\infty} dist(x_n) = \infty \equivalent  lim_{n->\infty} f(x_n) = \infty \f$
      For each pair of points \f$ x,y \f$ with \f$ f(x) < f(y) \f$ exists an f-monotonic connecting path \f$ \gamma\f$.
      [Wrong even for dist: Local minima must be separated from local maxima by \f$ \bd \Omega \f$.]
  
      Perhaps 1:1 correspondence between local extrema for f and dist? 
      (May need to accept curves as loc. of extrema). Or homeomorphic sets of iso-sets?
      continous deformation of pseudo-distance into Euclidian distance? Gradient descent comes to surface?

     \see Tested in \ref test-vcg.C
   */



  /*! \brief Definitions related to \ref vcg_grp
    \ingroup vcg_grp 
   */
  namespace vcg {
    template<class COORD, class RES>  class pseudo_distance_fct_impl;
    template<class COORD, class RES>  class pseudo_distance_fct;
    template<class F>                 class pseudo_distance_fct_impl_F;

    // concrete shapes
    template<class COORD, class RES>  class halfspace;
    template<class COORD, class RES>  class sphere;
    template<class COORD, class RES>  class infinite_cylinder;
    template<class F1, class F2>      class union_pdf;
    template<class F1, class F2>      class intersection_pdf;
  }

    /*! \defgroup vcg_primitives Concrete primitive pseudo-distance functions

      \ingroup vcg_grp 

      This module contains a collection of concrete pseudo-distance functions
      for \ref vcg_grp (most of them are actually true distance functions).
    */


  /*! \defgroup vcg_operators Operators for combining pseudo-distance functions
      
     \ingroup vcg_grp

     This modules contains elementary set operations like union and intersection.
     Note that combining true distance functions with these primitives
     in general does not result in true distance functions, but only in
     pseudo-distance functions. 
  */ 

  /*! \defgroup vcg_transformations Transformations for geometries defined by pseudo-distance functions
    
     \ingroup vcg_grp
  */

  /*! \brief Base class for generic (type-erased) pseudo-distance function
    \ingroup vcg_grp 
  */
    template<class COORD, class RES>
    class vcg::pseudo_distance_fct_impl {
    public:
      typedef COORD argument_type;
      typedef RES   result_type;

      virtual result_type   operator()(argument_type x) const = 0;
      virtual argument_type d         (argument_type x) const = 0;

      virtual ~pseudo_distance_fct_impl() {}
    };



    /*! \brief Generic pseudo-distance function 
      \ingroup vcg_grp 
     */
    template<class COORD, class RES>
    class vcg::pseudo_distance_fct {
    private:
      typedef pseudo_distance_fct_impl<COORD,RES> impl_type;
      ref_ptr<impl_type>                          impl; 
    public:
      typedef COORD argument_type;
      typedef RES   result_type;
      pseudo_distance_fct() {}
      pseudo_distance_fct(impl_type        * imp) : impl(imp) {}
      pseudo_distance_fct(ref_ptr<impl_type> imp) : impl(imp) {}

      result_type   operator()(argument_type x) const { return (*impl)(x);}
      argument_type d         (argument_type x) const { return impl->d(x);}
    };


  /*! \brief Pseudo distance function derived implementation

      \templateparams
       - \c F is a mapping type, e.g. like those defined in \ref vcg_primitives
   */
    template<class F>
    class vcg::pseudo_distance_fct_impl_F 
      : public  pseudo_distance_fct_impl<typename F::argument_type,
					 typename F::result_type> 
    {
    public:
      typedef pseudo_distance_fct_impl<typename F::argument_type,
				       typename F::result_type>    base;
      typedef typename base::argument_type argument_type;
      typedef typename base::result_type   result_type;

    private:
      ref_ptr<const F> f;
    public:
      pseudo_distance_fct_impl_F(F         const& ff) : f(ff) {}
      pseudo_distance_fct_impl_F(ref_ptr<F const> ff) : f(ff) {}
      
      virtual result_type   operator()(argument_type x) const { return (*f)(x);}
      //FIXME: do this only if f has builtin derivative
      virtual argument_type d         (argument_type x) const { return f->d(x);}
    };

    
  
    /* // go perhaps to mesh_and_more -> images 
    template<class IMAGE>
    class pseudo_distance_fct_impl_img 
      :  public  pseudo_distance_fct_impl<typename IMAGE::voxel_size_type,
					  typename IMAGE::value_type>
    {
      typedef IMAGE image_type;
      typedef image_as_function_view<image_type> image_view_type;
    private:
      ref_ptr<image_view_type> img;
    public:
      typedef typename IMAGE::voxel_size_type argument_type;
      typedef typename IMAGE::value_type      result_type;

      pseudo_distance_fct_impl_img(image_type const& img) 
	: f(new_ref_ptr(new image_view_type(img))) {}
      virtual result_type   operator()(argument_type x) const;
      virtual argument_type d         (argument_type x) const;
    };
    */

  
  namespace vcg {
    template<class F>
    pseudo_distance_fct<typename F::argument_type, 
			typename F::result_type> 
    DistanceFct(F const& f) { 
      typedef pseudo_distance_fct<typename F::argument_type, typename F::result_type> res_type;
      return res_type(new pseudo_distance_fct_impl_F<F>(f)); 
    }

    template<class F>
    pseudo_distance_fct<typename F::argument_type, 
			typename F::result_type> 
    DistanceFct(ref_ptr<F> f) { 
      typedef pseudo_distance_fct<typename F::argument_type, typename F::result_type> res_type;
      return res_type(new pseudo_distance_fct_impl_F<F>(f)); 
    }
  }


    /*! \brief Euclidian distance function for a halfspace

       \ingroup vcg_primitives
     */
    template<class COORD, class RES>
    class vcg::halfspace {
    public:
      typedef COORD      coord_type;
      typedef RES        real_type;
      typedef real_type  result_type;
      typedef coord_type argument_type;
    private:
      // outer normal
      coord_type normal;
      real_type  dist;
      
      typedef algebraic_primitives<coord_type> ap;
    public:
      /*! \brief Construct halfspace with boundary point \c x0 and outward normal \c n
       */
      halfspace(coord_type n, coord_type x0) 
	: normal(ap::normalization(n)), dist(ap::dot(normal,x0)) {}
      
      real_type operator()(coord_type x) const { return signed_distance(x);}
      real_type signed_distance(coord_type x) const { return ap::dot(x,normal) - dist;}
      
      argument_type d(coord_type x) const { return normal;}
    };
  
  namespace differentiation { 
    template<class COORD, class RES>  
    struct has_builtin_gradient<vcg::halfspace<COORD, RES> > { typedef true_type type;};
  }
  
    /*! \brief Euclidean distance function for a sphere

       \ingroup vcg_primitives
     */
    template<class COORD, class RES>
    class vcg::sphere {
    public:
      typedef COORD      coord_type;
      typedef coord_type argument_type;
      typedef RES        result_type;
      typedef double     real_type;
      
      typedef GrAL::algebraic_primitives<coord_type> ap;
    private:
      real_type  r;
      coord_type m;
      
    public:
      sphere(real_type rr, coord_type mm = coord_type(0.0)) : r(rr), m(mm) {}
      result_type operator()(coord_type x) const { return signed_distance(x);}
      result_type signed_distance(coord_type x) const { return result_type(ap::distance(x,m) - r);}

      argument_type d(coord_type x) const { return ap::normalization(x-m);}

    };

  namespace differentiation {
    template<class COORD, class RES>  
    struct has_builtin_gradient<vcg::sphere<COORD, RES> > { typedef true_type type;};
  }

    /*! \brief Euclidean distance function for an infinite cylinder

       \ingroup vcg_primitives
     */
    template<class COORD, class RES>
    class vcg::infinite_cylinder {
    public:
      typedef COORD      coord_type;
      typedef RES        real_type;
      typedef real_type  result_type;
      typedef coord_type argument_type;
    private:

      GrAL::line<coord_type> axis;
      real_type              r;

      typedef algebraic_primitives<coord_type> ap;
    public:
      infinite_cylinder(real_type rr, GrAL::line<coord_type> a) : axis(a), r(rr) {}
      infinite_cylinder(real_type rr, coord_type p0, coord_type p1) : axis(p0,ap::normalization(p1-p0)), r(rr) {}

      real_type operator()(coord_type x) const { return signed_distance(x);}

      real_type signed_distance(coord_type x) const {
	real_type dist = ap::distance(x, projection(x)) - r;
	return dist;
      }
      argument_type projection(coord_type x) const { return axis.p0() + ap::dot(axis.dir(), x - axis.p0())*axis.dir();}
      argument_type d         (coord_type x) const { return ap::normalization(x-projection(x));}
    };

  namespace differentiation {
    template<class COORD, class RES>  
    struct has_builtin_gradient<vcg::infinite_cylinder<COORD, RES> > { typedef true_type type;};
  }


  namespace vcg { namespace detail {

      // R-function differentiable min/max (R_\alpha family, Shapiro:RFP:1991)
      template<class T>
      inline T r_min(T x, T y, T a = T(1), int m = 0) { 
	T res = (1/(1+a))*(x + y - sqrt(x*x+y*y - 2*a*x*y));
	return ( m == 0 ? res : res * pow((x*x + y*y), m/2.0));
      }
      
      template<class T>
      inline T r_min_dx(T x, T y, T a = T(1), int m = 0) { 
	T dx_res = (1.0/(1.0+a))*(1.0 - (2*x - 2*a*y)*0.5/sqrt(x*x+y*y - 2*a*x*y));
	return ( m == 0 ? dx_res : 2*x * (m/2.0)*pow((x*x+y*y), m/2.0 - 1) * r_min(x,y,a,m) + dx_res * pow(x*x+y*y, m/2.0));
      }
      template<class T>
      inline T r_min_dy(T x, T y, T a = T(1), int m = 0) { return r_min_dx(y,x,a,m);}
      
      
      template<class T>
      inline T r_max(T x, T y, T a = T(1), int m = 0) { 
	T res = (1/(1+a))*(x + y + sqrt(x*x+y*y - 2*a*x*y));
	return ( m == 0 ? res : res * pow((x*x + y*y), m/2.0));
      }
      
      template<class T>
      inline T r_max_dx(T x, T y, T a = T(1), int m = 0) { 
	T dx_res = (1.0/(1.0+a))*(1.0 + (2*x - 2*a*y)*0.5/sqrt(x*x+y*y - 2*a*x*y));
	return ( m == 0 ? dx_res : 2*x * (m/2.0)*pow((x*x+y*y), m/2.0 - 1) * r_min(x,y,a,m) + dx_res * pow(x*x+y*y, m/2.0));
      }
      template<class T>
      inline T r_max_dy(T x, T y, T a = T(1), int m = 0) { return r_max_dx(y,x,a,m);}
    
    /*  
      // blending function in \f$ x \wedge_a y \f$ from R-function theory
      struct no_blending {
	template<class T>
	static T a(T x, T y) { return 1;}
      };
      
      struct global_blending {
	template<class T>
	static T a(T x, T y) { return T(s_a);}
	
	static double s_a;
      };
      
      
      struct local_blending {
	template<class T>
	static T a(T x, T y) { return f(x-y);}
	
	template<class T>
	static T f(T x) { 
	  T y = x*x;
	  return ( y >= 1 ? 1 : y*y*(3 - 2*y));
	}
      };
      
      double global_blending::s_a = 0;
    */      

      /*! \brief Local blending displacement
	
      See \cite{Pasko:2005:BBF}
      */
      template<class T>
      class blend_displ {
	T m_a, m_a1, m_a2;
      public:
	typedef T result_type;
	explicit blend_displ(T aa = 0, T aa_i = 1) 
	  :  m_a(aa), m_a1(aa_i), m_a2(aa_i) { init();}
	/*! Construct with magnitude and unsymmetric widths
	  
	\param \c mag Magnitude of the blending. <tt> mag == 0 </tt> disables blending.
	In the case of union, <tt>mag < 0</tt> adds material, <tt> mag > 0 </tt> subtracts material.
	In the case of intersection, it's the other way around.
	\param \c width1, \c width2 Extent of the blending in terms of the distance function arguments.
	*/
	blend_displ(T mag, T width1, T width2) 
	  : m_a(mag), m_a1(width1), m_a2(width2) { init(); }
	
	/*! Blending displacement
	  
	The blending displacement is evaluated on the distance function values \c f1 and  \c f2
	*/
	T operator()(T f1, T f2) const {
	  T rr = f1*f1/a1() + f2*f2/a2();
	  return (rr > 1 ? 0 : a()*(1-rr)*(1-rr)*(1-rr)/(1+rr));
	}
	
	T dx1(T f1, T f2) const {
	  T rr = f1*f1/a1() + f2*f2/a2();
	  return (rr > 1 ? 0 : 2*f1/a1() * drr(rr));
	}
	
	T dx2(T f1, T f2) const {
	  T rr = f1*f1/a1() + f2*f2/a2();
	  return (rr > 1 ? 0 : 2*f2/a2() * drr(rr));
	}
	
      private:
	T drr(T rr) const { 
	  T r1 = 1+rr;
	  T r_1 = 1-rr;
	  return a()*( -3*r_1*r_1/r1 + (-1)*r_1*r_1*r_1/(r1*r1));
	}
	
      private:
	T a1() const { return m_a1;}
	T a2() const { return m_a2;}
	T a () const { return m_a;}
	
	void init() {
	  m_a1 = (m_a1 < 0 ? -m_a1 : m_a1);
	  m_a2 = (m_a2 < 0 ? -m_a2 : m_a2);
	}
      };

  } } // namespace vcg { namespace detail 

    /*! \brief pseudo-distance function (pdf) for computing union of two bodies given by pdfs

       The union can be blended (default: no blending).

       \ingroup vcg_operators 
    */
    template<class F1, class F2>
    class vcg::union_pdf {
    public:
      typedef typename F1::result_type   result_type;
      typedef typename F1::argument_type argument_type;
      typedef typename F1::coord_type    coord_type;
    private:
      ref_ptr<const F1> f1;
      ref_ptr<const F2> f2;
      detail::blend_displ<result_type> displ;
    public:
      /*! \brief Construct from pseudo distance fields \c ff1 and  \c ff2
  
          \param \c ff1, \c ff2: pseudo distance fields
          \param \c b_mag Magnitude of blending, 
             - <tt>  b_mag < 0 </tt> subtracts material
             - <tt>  b_mag == 0 </tt> disables blending
             - <tt>  b_mag < 0 </tt>  adds material
          \param \c b_range Width of the blending, in terms of the pdf values of \c ff1 and \c ff2 
       */
      union_pdf(F1 const& ff1, F2 const& ff2, 
		 result_type b_mag = 0, 
		 result_type b_range = 1) : f1(ff1), f2(ff2), displ(-b_mag, b_range) {}
      union_pdf(ref_ptr<const F1> ff1, ref_ptr<const F2> ff2, 
		 result_type b_mag = 0, 
		 result_type b_range = 1) : f1(ff1), f2(ff2), displ(-b_mag, b_range) {}
      
      result_type operator()(argument_type x) const { 
	return std::min((*f1)(x), (*f2)(x)) + displ((*f1)(x), (*f2)(x));
      }
      argument_type d(argument_type x) const { 
	using differentiation::grad;
	result_type r1 = (*f1)(x), r2 = (*f2)(x);
	return displ.dx1(r1,r2)*grad(*f1,x) + displ.dx2(r1,r2) * grad(*f2,x) + (r1 < r2 ? grad(*f1,x) : grad(*f2,x));
      }
      /*
      result_type operator()(argument_type x) const { 
	return r_min((*f1)(x), (*f2)(x)) + displ((*f1)(x), (*f2)(x));
      }
      */

    }; // class union_pdf

  namespace differentiation {
    template<class F1, class F2>  
    struct has_builtin_gradient<vcg::union_pdf<F1,F2> > { typedef true_type type;};
  }

 
  /*! \brief pseudo-distance function (pdf) for computing intersection of two bodies given by pdfs
    
     \ingroup vcg_operators 
  */
  template<class F1, class F2>
  class vcg::intersection_pdf {
    public:
      typedef typename F1::result_type   result_type;
      typedef typename F1::argument_type argument_type;
      typedef typename F1::coord_type    coord_type;
    private:
      ref_ptr<const F1> f1;
      ref_ptr<const F2> f2;
      result_type a;
      int         m;
    public:
      intersection_pdf(F1 const& ff1, F2 const& ff2) : f1(ff1), f2(ff2), a(0.9), m(0) {}
      intersection_pdf(ref_ptr<const F1> ff1, ref_ptr<const F2> ff2) : f1(ff1), f2(ff2), a(0.9), m(0) {}
      result_type operator()(argument_type x) const { 
	//return detail::r_max((*f1)(x), (*f2)(x), a,m);
	return std::max((*f1)(x), (*f2)(x));
      }

      argument_type d(argument_type x) const { 
	using differentiation::grad;
	result_type r1 = (*f1)(x), r2 = (*f2)(x);
	//argument_type res = detail::r_max_dx(r1,r2,a,m) * grad(*f1,x) + detai::r_max_dy(r1,r2,a,m) * grad(*f2,x);
	argument_type res (r1 >= r2 ? grad(*f1,x) : grad(*f2,x));
	//typedef algebraic_primitives<argument_type> ap;
	//return ap::normalization(res);
	return res;
      }
    }; // class intersection_pdf

  namespace differentiation {
    template<class F1, class F2>  
    struct has_builtin_gradient<vcg::intersection_pdf<F1,F2> > { typedef true_type type;};
  }
  

  namespace vcg {
    /*! \brief Convenience function for intersections
      \ingroup vcg_operators 
    */
    template<class F1, class F2>
    intersection_pdf<F1,F2> intersect(F1 const& f1, F2 const& f2) { return  intersection_pdf<F1,F2>(f1,f2);}

    /*! \brief Convenience function for intersections
      \ingroup vcg_operators 
      \todo remove possible cv qualifiers on result type parameters
    */
    template<class F1, class F2>
    intersection_pdf<F1,F2> intersect(ref_ptr<F1> f1, ref_ptr<F2> f2) { return  intersection_pdf<F1,F2>(f1,f2);}
    
    /*! \brief Convenience function for unions
      \ingroup vcg_operators 
    */
    template<class F1, class F2>
    union_pdf<F1,F2> unite(F1 const& f1, F2 const& f2) { return  union_pdf<F1,F2>(f1,f2);}

    /*! \brief Convenience function for unions
      \ingroup vcg_operators 
    */
    template<class F1, class F2>
    union_pdf<F1,F2> unite(ref_ptr<F1> f1, ref_ptr<F2> f2) { return  union_pdf<F1,F2>(f1,f2);}

   /*! \brief Convenience function for unions with blending
      \ingroup vcg_operators 
    */
    template<class F1, class F2>
    union_pdf<F1,F2> unite(F1 const& f1, F2 const& f2, 
			    typename F1::result_type b_mag,
			    typename F1::result_type b_wid) 
    { return  union_pdf<F1,F2>(f1,f2,b_mag,b_wid);}

    /*! \brief Convenience function for unions with blending
      \ingroup vcg_operators 
    */
    template<class F1, class F2>
    union_pdf<F1,F2> unite(ref_ptr<F1> f1, ref_ptr<F2> f2, 
			    typename F1::result_type b_mag,
			    typename F1::result_type b_wid) 
    { return  union_pdf<F1,F2>(f1,f2,b_mag,b_wid);}


    /*! \brief Transform a pseudo-distance function with a pre-applied transformation

       \ingroup vcg_transformations

       This transforms the geometry defined by the pseudo-distance function \c f by the 
       \e inverse of the transformation \c phi 

       \todo differentiation support
    */
    template<class F, class TRAFO>
    class cotransformed_pdf {
    public:
      typedef typename F::result_type        result_type;
      typedef typename TRAFO::argument_type  argument_type;
    private:
      ref_ptr<F const>     m_f;
      ref_ptr<TRAFO const> m_phi;
    public:
      /* \brief construct the pseudo-distance \f$ f \circ \phi \f$
       */
      cotransformed_pdf(F         const& f, TRAFO         const& phi) : m_f(f), m_phi(phi) {}
      /* \brief construct the pseudo-distance \f$ f \circ \phi \f$
       */
      cotransformed_pdf(ref_ptr<F const> f, ref_ptr<TRAFO const> phi) : m_f(f), m_phi(phi) {}

      result_type operator()(argument_type x) const { return (*m_f)((*m_phi)(x)); }
    };


    /*! \brief Project \c x to to the boundary of the solid defined by \c F

        Find the nearest point to \c x on the zero-level-set of \c F.
	This assumes \c F is a pseudo-distance function 
	with \f$ |F(x)|/d_\Omega(x) \to 1 \f$ if \f$ x \to b \in \bd \Omega \f$, 
	for almost every \f$  b \in \bd \Omega \f$. Here \f$ \Omega \f$ is the set 
	\f$ \{ x \mid F(x) < 0 \} \f$, and \f$  d_\Omega \f$ is the true signed distance function.
     */
    template<class F, class COORD> //, class REAL = double>
    COORD project_to_boundary(F const& f, COORD x, double eps = 1.0e-5, int max_iter = 4) 
    {
      typedef typename F::argument_type coord_type;
      coord_type x_n = GrAL::convert_point<coord_type>(x);
      differentiation::gradient<F> grad_f(f); // ,eps);
     
      int n = 0;
      while(fabs(f(x_n)) > 10*eps && n < max_iter) {
	x_n = x_n - f(x_n) * grad_f(x_n);
	++n;
      }
      if(n == max_iter)
	std::cerr << " project_to_boundary(f, x=" << x <<  " (grad_f(x)=" << grad_f(x) << ")" 
		  <<  ", eps=" << eps << ", max_iter=" << max_iter 
		  << "): Maximum number of iterations reached," 
		  << " current values: x_n=" << x_n << " f(x_n)=" << f(x_n) << " grad_f(x_n)=" <<  grad_f(x_n)
		  << std::endl;;
      return GrAL::convert_point<COORD>(x_n);
    }
  } // namespace vcg


} // namespace GrAL

#endif
