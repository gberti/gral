#ifndef NMWR_GB_FUNCTIONALGEBRA_FA_H
#define NMWR_GB_FUNCTIONALGEBRA_FA_H

/*!
//  The class RFunction implements a function algebra:
//  RFunctions may be combined by algebraic operators
//  and composed by operator(). There is also support for
//  differentiation.
//  This uses dynamic binding and is not very efficient, 
//  if many combination steps are involved.
//
//  (c) Guntram Berti 1997, 
//  Lehrstuhl f"ur 
//  Numerische Mathematik und Wissenschaftliches Rechnen
//  BTU Cottbus
//  Universit"atsplatz 3-4
//  D-03044 Cottbus
//  http://math-s.math.tu-cottbus.de/NMWR
//
//  last change: 28/08/97
*/
///////////////////////////////////////////////////////////////

#include "compiler-config.h"

#include <math.h>
#include <strstream.h>
#include "mystring.h"

// #include "Work-around/comma-template.h"
#include "LinAlg/point.h"
#include "RFunction/my_handle_types.h"
#include "RFunction/fct.h"


class Derivative; // generalization of RFunction ??
class RFunction;


// a class defining the behavior of a derivative
 
class Derivative {
private:
  int k; // k == order of deriv.
  const point x;
  const RFunction* f; // *this == k-th derivative of f at x
public:
  Derivative(const RFunction* ff, const point& xx, int kk) : k(kk), x(xx), f(ff) {} 
  virtual ~Derivative() {}
  virtual point operator() (const point& h) const;  // h=(h_1,....,h_k)
  //  friend Derivative operator , (const Derivative& d1, const Derivative& d2);
};



  

// a wrapper hiding the pointer to class fct for the user
// this class defines the user interface

class RFunction : public my_handle_base {
private:
  fct* f() const { return (fct *)PTR;}
  double a; // return a*f+b, so that we can accumulate +-/ * const for efficiency
  point b;
protected:

  friend class Derivative;
public:
  typedef RFunction self;
  MAKE_DEFAULT_IO_OPS_SELF
  MAKE_LESS_OP_SELF
  typedef point coord_type;
  typedef point argument_type;
  typedef point result_type;
  typedef double real_type;

  RFunction();
  RFunction(fct* f) : my_handle_base(f), a(1.0), b(f->dIm(),zero_init)  {}
  RFunction(const RFunction& F): my_handle_base(F), a(F.a), b(F.b)  {}
  RFunction& operator=(const RFunction& F) {
    my_handle_base::operator=(F);
    a=F.a;
    b=F.b;
    return *this;
  }
  virtual ~RFunction() { clear();}

  Derivative derive(const point& x) const { return Derivative(this,x,1);}
  virtual point derive(const point& x, const point& h) const {return a*f()->derive(x,h);}

  virtual point operator() (const point& x) const  {return (a*f()->eval(x) +b);}
  int dIm() const { return f()->dIm();}
  int dDef() const {return f()->dDef();}
  unsigned dim_of_image() const { return dIm();}
  unsigned dim_of_domain() const { return dDef();}

  bool IsAffine() const {return f()->IsAffine();}
  string name() const {return f()->name();}
  //virtual RFunction& operator[](int ) = 0; // (f1,f2)[1] = f1 if f1:R^n->R;
  //friend RFunction& operator+= (const double);
  RFunction operator() (const RFunction& F) const;
  // confuses g++ 
  //  friend RFunction operator , (const RFunction& f1, const RFunction& f2);
  // used instead:
  friend RFunction combine(const RFunction& f1, const RFunction& f2);
  friend RFunction combine(const RFunction& f1, const RFunction& f2, const RFunction& f3)
    { return combine(f1,combine(f2,f3));}
  //(f1,f2)(x) = (f1(x),f2(x))
  bool operator==(const RFunction& F) const { return (f() == F.f());} // missing comp. a, b!!
  // ------> wait for point being my_handle-class and therefore comparable
  RFunction& operator+=(const point& p) {b+=p; return (*this);}
  RFunction& operator-=(const point& p) {b-=p; return (*this);}
  RFunction& operator*=(const double& d) {a*= d; b*=d; return (*this);}
  RFunction& operator/=(const double& d) {a/= d; b/=d; return (*this);}
  friend RFunction operator + (const RFunction& f1, const RFunction& f2);
  friend RFunction operator + (const RFunction& f1, const point&     a){RFunction f(f1); return f+=a;}
  friend RFunction operator + (const point&     a,  const RFunction& f2){RFunction f(f2); return f+=a;}
  friend RFunction operator + (const real_type&      a,  const RFunction& f2){ return (point(a) + f2);}
  friend RFunction operator + (const RFunction& f1, const real_type&      a) { return (f1 + point(a));}

  friend RFunction operator - (const RFunction& f1, const RFunction& f2);
  friend RFunction operator - (const RFunction& f1, const point&     a){RFunction f(f1); return f+= -a;}
  friend RFunction operator - (const point&     a, const RFunction& f2)
  {RFunction f(f2); f*=-1.0; f+= a; return f;}
  friend RFunction operator - (const real_type&      a,  const RFunction& f2){ return (point(a) - f2);}
  friend RFunction operator - (const RFunction& f1, const real_type&      a) { return (f1 - point(a));}

  friend RFunction operator * (const RFunction& f1, const RFunction& f2);
  friend RFunction operator * (const RFunction& f1, const double&     a){RFunction f(f1); return f*=a;}
  friend RFunction operator * (const double&     a, const RFunction& f2){RFunction f(f2); return f*=a;}
  friend RFunction operator / (const RFunction& f1, const RFunction& f2);
  friend RFunction operator / (const RFunction& f1, const double&     a){RFunction f(f1); return f*= (1.0/a);}
  friend RFunction operator / (const double&     a, const RFunction& f2);
  friend RFunction operator-(const RFunction& F) { return ( -1.0 * F);} 


  friend  Derivative d(const RFunction& f, const point& x);
   
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  string write_code_derive(ostream& out, int& vnum, 
			  const string& varx, const string& varh) const;
  void write_class(ostream& out, const string& name) const;


};


extern RFunction operator * (const point& p, const RFunction& f);
extern RFunction operator * (const RFunction& f, const point& p);

//  Piecewise(F0,Fneg,Fpos) = ( F0 < 0 ? Fneg : Fpos)
extern RFunction Piecewise(const RFunction& F0, const RFunction& Fneg, const RFunction& Fpos);

inline Derivative d(const RFunction& F, const point& x) { return F.derive(x);}





// concrete classes defining operations on derivatives

//point Derivative::operator() (const point& h) const { return f->derive(x,h);} 

/* confusion of g++
class DerivCombination : public Derivative {
private:
 const Derivative& d1,d2;
public:
  DerivCombination(const Derivative& d1,const Derivative& d2);
  point operator()(const point& h) const { return ((d1(h),d2(h)));}
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
extern RFunction Constant(const point& p, int dDef);
extern RFunction X(int i);
extern RFunction X(int i,int dim);
extern RFunction Scale(const point& diag);
inline RFunction Scale2D(double s) { return Scale(point(s,s));}
inline RFunction Scale3D(double s) { return Scale(point(s,s,s));}
extern RFunction Translate(const point& t);
inline RFunction Translate(double x, double y, double z) { return Translate(point(x,y,z));}
extern RFunction ProjectToHyperplane(const point& normal, double dist =0.0); 
extern RFunction DistanceToHyperplane(const point& normal, double dist =0.0);
extern RFunction Rotation2D(double a);
extern RFunction Rotation3D(const point& axis, double a);

// some other useful functions
extern RFunction Norm2(int dim); // || x ||^2  where x in R^dim
extern RFunction Circle(const point& m, double r); // ||x-m|| - sqrt(r)
extern RFunction Polynome1D(const point& c); // c[1] + x*c[2] + x^2*c[3] + ....
extern RFunction S3(const double& a, const double& b); // an S-shaped pw cubic polynome, 
// S3ab(a) = 0, S3ab(b) = 1, S3ab globally C^1 and constant outside [a,b].
// it is NOT required that a < b !

template<class P>
inline
RFunction LinearSegment(P const& p1, P const& p2)
{
  point q1, q2;
  assign_point(q1,p1);
  assign_point(q2,p2);
  RFunction t = X(1,1);
  return ((1.0-t)*Constant(q1,1) + t*Constant(q2,1));
}

#endif
