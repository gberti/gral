#ifndef NMWR_GB_FUNCTIONALGEBRA_FA_H
#define NMWR_GB_FUNCTIONALGEBRA_FA_H

// $LICENSE

#include "Config/compiler-config.h"

#include <string>

#include "FunctionAlgebra/function-algebra-impl.h"

#include "Geometry/algebraic-primitives.h" // for assign_point

#include "Utility/reference-count.h"




class Derivative; 
class RFunction;


//! Class serving as derivative for RFunction
 
class Derivative {
private:
  int k; //!<  k = order of derivative
  const point x;
  const RFunction* f; // k-th derivative of f at x
public:
  Derivative(const RFunction* ff, const point& xx, int kk) : k(kk), x(xx), f(ff) {} 
  virtual ~Derivative() {}
  virtual point operator() (const point& h) const;  // h=(h_1,....,h_k)
  //  friend Derivative operator , (const Derivative& d1, const Derivative& d2);
};



  

/*!
  \brief The class RFunction implements a function algebra

  RFunctions may be combined by algebraic operators
  and composed by operator(). There is also support for
  differentiation.

  RFunction is a wrapper for function_algebra_impl.  
  It uses dynamic binding and is not very efficient, 
  if many combination steps are involved.
*/

class RFunction {
private:
  copy_on_write_ptr<function_algebra_impl> ff;
  
  double a; // return a*f+b, so that we can accumulate +-/ * const for efficiency
  point  b;
protected:

  friend class Derivative;
public:
  typedef RFunction self;
  typedef function_algebra_impl::coord_type  coord_type;
  typedef point_traits<coord_type>           pt;
  typedef pt::component_type                 real_type;
  typedef coord_type                         argument_type;
  typedef coord_type                         result_type;

  RFunction();
  RFunction(function_algebra_impl * fff) : ff(fff),  a(1.0), b(ff->dIm(), coord_type::zero_init)  {}
  ~RFunction() {}

  //----------------- Function Interface -----------------------

  //! evaluation
  coord_type     operator() (const coord_type& x) const  {return (a*ff->eval(x) +b);}
  //! composition
  RFunction operator() (const RFunction& F) const;

  //@{ @name Dimensions of domain of definition and image
  unsigned dIm()  const {return ff->dIm();}
  unsigned dDef() const {return ff->dDef();}
  unsigned dim_of_image()  const { return dIm();}
  unsigned dim_of_domain() const { return dDef();}
  //@}

  // seems to be used only by GraphicsDevice.
  bool IsAffine() const {return ff->IsAffine();}


  //------------ algebraic operations -------------------------

  //@{ @name Algebraic operations
  RFunction& operator+=(const coord_type& p) {b+=p; return (*this);}
  RFunction& operator-=(const coord_type& p) {b-=p; return (*this);}
  RFunction& operator*=(const double& d) {a*= d; b*=d; return (*this);}
  RFunction& operator/=(const double& d) {a/= d; b/=d; return (*this);}

  friend RFunction operator + (const RFunction& f1, const RFunction& f2);
  friend RFunction operator + (const RFunction& f1, const coord_type&     a){RFunction f(f1); return f+=a;}
  friend RFunction operator + (const coord_type&     a,  const RFunction& f2){RFunction f(f2); return f+=a;}
  friend RFunction operator + (const real_type&      a,  const RFunction& f2){ return (coord_type(a) + f2);}
  friend RFunction operator + (const RFunction& f1, const real_type&      a) { return (f1 + coord_type(a));}

  friend RFunction operator - (const RFunction&  f1, const RFunction&  f2);
  friend RFunction operator - (const RFunction&  f1, const coord_type& a){RFunction f(f1); return f+= -a;}
  friend RFunction operator - (const coord_type& a,  const RFunction&  f2)
  { RFunction f(f2); f*=-1.0; f+= a; return f;}
  friend RFunction operator - (const real_type&      a,  const RFunction& f2){ return (coord_type(a) - f2);}
  friend RFunction operator - (const RFunction& f1, const real_type&      a) { return (f1 - coord_type(a));}

  friend RFunction operator * (const RFunction& f1, const RFunction& f2);
  friend RFunction operator * (const RFunction& f1, const double&     a){RFunction f(f1); return f*=a;}
  friend RFunction operator * (const double&     a, const RFunction& f2){RFunction f(f2); return f*=a;}

  friend RFunction operator / (const RFunction& f1, const RFunction& f2);
  friend RFunction operator / (const RFunction& f1, const double&     a) {RFunction f(f1); return f*= (1.0/a);}
  friend RFunction operator / (const double&     a, const RFunction& f2);

  friend RFunction operator-(const RFunction& F) { return ( -1.0 * F);} 
  //@}

  //@{ @name Making single functions components of a vector-valued  function
  friend RFunction combine(const RFunction& f1, const RFunction& f2);
  friend RFunction combine(const RFunction& f1, const RFunction& f2, const RFunction& f3)
    { return combine(f1,combine(f2,f3));}
  //(f1,f2)(x) = (f1(x),f2(x))
  //@}

  //@{ @name Derivation 
  friend  Derivative d(const RFunction& f, const coord_type& x);
  Derivative derive(const coord_type& x) const { return Derivative(this,x,1);}
  coord_type derive(const coord_type& x, const coord_type& h) const 
   {return a*ff->derive(x,h);}
  //@}  

  std::string name() const {return ff->name();}

  /*@{ @name code generation functions.
       Write C++ code evaluating the function.
  */
  std::string write_code_eval(std::ostream& out, int& vnum, 
                              const std::string& var)    const;
  std::string write_code_derive(std::ostream& out, int& vnum, 
                                const std::string& varx, 
                                const std::string& varh) const;
  void write_class(std::ostream& out, const std::string& name) const;
  /*@}*/

};

inline Derivative d(const RFunction& F, const RFunction::coord_type& x) { return F.derive(x);}


extern RFunction operator * (const  RFunction::coord_type& p, const RFunction& f);
extern RFunction operator * (const RFunction& f,  const  RFunction::coord_type& p);

//  Piecewise(F0,Fneg,Fpos) = ( F0 < 0 ? Fneg : Fpos)
extern RFunction Piecewise(const RFunction& F0, 
                           const RFunction& Fneg, 
                           const RFunction& Fpos);






// concrete classes defining operations on derivatives

//coord_type Derivative::operator() (const coord_type& h) const { return f->derive(x,h);} 

/* confusion of g++
class DerivCombination : public Derivative {
private:
 const Derivative& d1,d2;
public:
  DerivCombination(const Derivative& d1,const Derivative& d2);
  coord_type operator()(const coord_type& h) const { return ((d1(h),d2(h)));}
};
  
inline Derivative operator , (const Derivative& d1, const Derivative& d2)
{ return DerivCombination(d1,d2);}
*/

extern const RFunction Identity1D;
extern const RFunction Identity2D;
extern const RFunction Identity3D;
extern       RFunction IdentityND(int dim);

// standard library RFunctions
extern const RFunction Sqr; // x -> x*x
extern const RFunction Sqrt; 
extern const RFunction Sin;
extern const RFunction Cos;
extern const RFunction Atan;
extern const RFunction Atan2; 
extern const RFunction Exp;
extern const RFunction Log;

// affine functions
extern RFunction NullFct(int dD=1,int dI = 1);
// extern RFunction Constant(double c, int dDef);
extern RFunction Constant(const  RFunction::coord_type& p, int dDef);
extern RFunction X(int i);
extern RFunction X(int i,int dim);
extern RFunction Scale(const  RFunction::coord_type& diag);
inline RFunction Scale2D(double s) { return Scale( RFunction::coord_type(s,s));}
inline RFunction Scale3D(double s) { return Scale( RFunction::coord_type(s,s,s));}
extern RFunction Translate(const  RFunction::coord_type& t);
inline RFunction Translate(double x, double y, double z) { return Translate( RFunction::coord_type(x,y,z));}
inline RFunction Translation(const  RFunction::coord_type& t) { return Translate(t);}
extern RFunction ProjectToHyperplane(const  RFunction::coord_type& normal, double dist =0.0); 
extern RFunction DistanceToHyperplane(const  RFunction::coord_type& normal, double dist =0.0);
extern RFunction Rotation2D(double a);
extern RFunction Rotation3D(const  RFunction::coord_type& axis, double a);

// some other useful functions
extern RFunction Norm2(int dim); // || x ||^2  where x in R^dim
extern RFunction Circle(const  RFunction::coord_type& m, double r); // ||x-m|| - sqrt(r)
extern RFunction Polynome1D(const  RFunction::coord_type& c); // c[1] + x*c[2] + x^2*c[3] + ....
extern RFunction S3(const double& a, const double& b); // an S-shaped pw cubic polynome, 
// S3ab(a) = 0, S3ab(b) = 1, S3ab globally C^1 and constant outside [a,b].
// it is NOT required that a < b !

template<class P>
inline
RFunction LinearSegment(P const& p1, P const& p2)
{
  function_algebra_impl::coord_type q1, q2;
  assign_point(q1,p1);
  assign_point(q2,p2);
  RFunction t = X(1,1);
  return ((1.0-t)*Constant(q1,1) + t*Constant(q2,1));
}

#endif
