#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include "FunctionAlgebra/function-algebra.h"

#include "./misc.h"
#include "./write-code.h"

#include "Geometry/algebraic-primitives.h"


// some concrete classes defining often used RFunctions, mainly
// geometric transformations and projections. See end of file
// for list of defined RFunctions

class function_algebra_constant: public function_algebra_impl {
private:
  typedef function_algebra_constant self;
  coord_type  derive(const coord_type& /*x*/, const coord_type& /*h*/) const { return coord_type(0.0);}
protected:
  coord_type c;
public:
  function_algebra_constant(const coord_type& cc, int ddef) : c(cc)
  {d_Def = ddef; d_Im = c.dim(), is_affine = true;}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type  eval(const coord_type& /*x*/) const {return c;}

  virtual std::string name() const { return "function_algebra_constant";}
  virtual std::string write_code_eval(std::ostream& out, int& vnum, 
                                      std::string const& var) const;
  virtual std::string write_code_derive(std::ostream& out, int& vnum, 
                                        std::string const& varx, 
                                        std::string const& varh) const;
};


std::string  function_algebra_constant::write_code_eval(std::ostream&, int& /*vnum*/, 
                                                        std::string const& /*var*/) const
{ 
#ifdef GRAL_HAS_SSTREAM
      std::ostringstream s; 
#else
      std::ostrstream s; 
#endif
 if (dIm() == 1)
   s << c[1];
 else {
   s << "coord_type((const double[]){";
   for(unsigned i =1; i<=dIm(); i++)
     s << c[i] << ",";
   s << "}, "  << dIm() << ")";
 }
 return s.str();
}
 

std::string function_algebra_constant::write_code_derive(std::ostream&, int& /*vnum*/, 
                                                         std::string const& /*varx*/, 
                                                         std::string const& /*varh*/) const
{
#ifdef GRAL_HAS_SSTREAM
      std::ostringstream s; 
#else
      std::ostrstream s; 
#endif
 if (dIm() == 0)
   s << "0.0";
 else
   s << "coord_type(" << dIm() << ", zero_init)";
 return s.str();
}




class function_algebra_coordinate : public function_algebra_impl {
private: 
  typedef function_algebra_coordinate  self;
  coord_type derive(const coord_type& /*x*/, const coord_type& h) const { return coord_type(h[i]);}
protected:
  int i;
public:
  function_algebra_coordinate(int ii) : i(ii) {is_affine=true;d_Im=1; d_Def=ANYDIM;} 
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return coord_type(x[i]);}

  virtual std::string name() const {return "function_algebra_coordinate";}
  virtual std::string  write_code_eval(std::ostream& out, int& vnum, 
                                       std::string const& var) const;
  virtual std::string write_code_derive(std::ostream& out, int& vnum, 
                                        std::string const& varx, 
                                        std::string const& varh) const;
};


std::string  function_algebra_coordinate::write_code_eval(std::ostream&, int& /*vnum*/, 
                                                          std::string const& var) const
{ 
#ifdef GRAL_HAS_SSTREAM
      std::ostringstream s; 
#else
      std::ostrstream s; 
#endif
  if (d_Def == 1) {s << var;} else  {s << var << "[" << i <<"]";} 
  return(s.str());
}

std::string function_algebra_coordinate::write_code_derive(std::ostream& , int& /*vnum*/, 
                                                           std::string const& /*varx*/, 
                                                           std::string const& varh) const
{ 
#ifdef GRAL_HAS_SSTREAM
      std::ostringstream s; 
#else
      std::ostrstream s; 
#endif
 if (d_Def == 1) {s << varh;} else  {s << varh << "[" << i <<"]";} 
  return(s.str());
}




class function_algebra_coordinate_with_dim : public function_algebra_coordinate {
private: 
  typedef function_algebra_coordinate_with_dim  self;
public:
  function_algebra_coordinate_with_dim(int ii, int dim) : function_algebra_coordinate(ii) {d_Im=1;d_Def=dim;} 
  virtual self* clone() const { return new self(*this);}

  virtual std::string name() const {return "function_algebra_coordinate_with_dim";}
};





/*! \brief An S-shaped function
   
   f(x) = 0 on ]-\infty,min(a,b)], f(x) = 1 on [max(a,b),\infty[
   f cubic between a and b with f' = 0 at a and b
*/

class function_algebra_hermite: public function_algebra_impl { 
  typedef function_algebra_hermite self;
private:
  double a,b;
  coord_type derive(const coord_type& x, const coord_type& h) const;
public:
  function_algebra_hermite(const double& aa, const double& bb) : a(aa), b(bb) {d_Im=d_Def=1;}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const
  { return coord_type(hermite(a,b,x[1]));}

  virtual std::string name() const {return "function_algebra_hermite";}
  virtual std::string write_code_eval(std::ostream& out, int& vnum, 
                                      std::string const& var) const;
  virtual std::string write_code_derive(std::ostream& out, int& vnum, 
                                        std::string const& varx, 
                                        std::string const& varh) const;
};


std::string function_algebra_hermite::write_code_eval(std::ostream& out, int& vnum, 
                                                      std::string const& var) const
{ 
  std::string r(makename("herm",vnum++));
  out << "double " << r << "( hermite(" << a << "," << b << "," << var << "));\n";
  return r;
}


std::string function_algebra_hermite::write_code_derive(std::ostream& out, int& vnum, 
                                                        std::string const& varx, 
                                                        std::string const& varh) const
{
  std::string r(makename("d_herm",vnum++));
  out << "double " << r << "( d_hermite(" << a << "," << b << "," << varx << ") * " << varh << ");\n";
  return r;
}


RFunction::coord_type 
function_algebra_hermite::derive(const RFunction::coord_type& x,
				 const RFunction::coord_type& h) const
{
  double t = (x[1]-a)/(b-a);
  if (t <= 0.0) return coord_type(0.0);
  else if (t<= 1.0) return coord_type(h*(6.0*t*(1.0-t)/(b-a)) );
  else return coord_type(0.0);
}




// a 1-dimensional polynome, initialized with a coefficient vector c,
// polynome = c[n] x^{n-1} + ... + c[2] x + c[1]

class function_algebra_polynomial1D : public function_algebra_impl {
  typedef function_algebra_polynomial1D self;
private:
  coord_type coeffs;
  coord_type derive(const coord_type& x, const coord_type& h) const; // may also be defined for doubles
public:
  function_algebra_polynomial1D(const coord_type& c) : coeffs(c) {d_Def=1;d_Im=1;}
  ~function_algebra_polynomial1D() {}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const; 

  int degree() const { return (coeffs.dim() -1);}
  virtual std::string name() const {return "function_algebra_polynomial1D";}
  virtual std::string write_code_eval(std::ostream& out, int& vnum, 
                                      std::string const& var) const;
  virtual std::string write_code_deriv(std::ostream& out, int& vnum, 
                                       std::string const& varx, 
                                       std::string const& varh) const;
};

std::string function_algebra_polynomial1D::write_code_eval(std::ostream& out, int& vnum, 
                                                           std::string const& var) const
{
  std::string bk(makename("bk",vnum++));
  std::string x1(makename("xtmp",vnum));
  std::string r(makename("poly",vnum));

  int n = degree();
  out << "double " << bk << "(" << coeffs[n+1] << ");\n"
      << "double " << x1 << "(" << var << ");\n"; 
  for(int k =1;k<=degree();k++){
    out << bk << " *= " << x1 << ";  " 
	<< bk << " += " << coeffs[n+1-k] << ";\n";
  }
  out << "double " << r << "(" << bk << ");\n";
  return r;
}

function_algebra_impl::coord_type 
function_algebra_polynomial1D::eval(const coord_type& x) const
{
  int n= degree();
  double bk=coeffs[n+1],x1=x[1];
  for(int k=1;k<=n;k++) {
    bk = x1*bk +coeffs[n+1-k];
  }
  return coord_type(bk);
}

function_algebra_impl::coord_type 
function_algebra_polynomial1D::derive(const coord_type& x, const coord_type& h) const
{
  int n= degree();
  double bk=n*coeffs[n+1],x1=x[1];
  for(int k=1;k<=n-1;k++) {
    bk = x1*bk +(n-k)*coeffs[n+1-k];
  }
  return coord_type(bk*h[1]);
}

std::string function_algebra_polynomial1D::write_code_deriv(std::ostream& out, int& vnum, 
                                                            std::string const& varx, 
                                                            std::string const& varh) const
{
  std::string bk(makename("bk",vnum++));
  std::string x1(makename("xtmp",vnum));
  std::string r(makename("poly",vnum));

  int n = degree();
  out << "double " << bk << "(" << n*coeffs[n+1] << ");\n"
      << "double " << x1 << "(" << varx << ");\n"; 
  for(int k =1;k<=degree()-1;k++){
    out << bk << " *= " << x1 << ";  " 
	<< bk << " += " << (n-k)*coeffs[n+1-k] << ";\n";
  }
  out << "double " << r << "(" << bk << " * " << varh << ");\n";
  return r;
}



class function_algebra_norm2: public function_algebra_impl { // works for every dimension!
  typedef function_algebra_norm2 self;
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {return coord_type(2*(x*h));}
public:
  function_algebra_norm2(int n) {d_Def = n; d_Im = 1;}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const 
   { return coord_type(ap::squared_norm_2(x)); }

  virtual std::string name() const {return "function_algebra_norm2";}
};


////////////////////////////////////////////////////////////////////////
//
//   some special affine-linear functions
//
////////////////////////////////////////////////////////////////////////


class function_algebra_scale: public function_algebra_impl {  // works for every dimension!
  typedef function_algebra_scale self;
private:
  coord_type diag;
  coord_type derive(const coord_type& /*x*/, const coord_type& h) const { return eval(h);}
public:
  function_algebra_scale(const coord_type& d) : diag(d) 
  { is_affine=true; d_Def = d_Im = diag.dim();}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const;

  virtual std::string name() const {return "function_algebra_scale";}
};

function_algebra_impl::coord_type 
function_algebra_scale::eval(const coord_type& x) const
{
  coord_type p(x); 
  for(int i=1;i<=p.dim();i++)
    p[i]*=diag[i];
  return p;
}




class function_algebra_translate : public function_algebra_impl {
  typedef function_algebra_translate self;
private:
 coord_type t;
  coord_type derive(const coord_type& /*x*/, const coord_type& h) const { return h;}
public:
  function_algebra_translate(const coord_type& tt) : t(tt)  {is_affine=true; d_Def = d_Im = t.dim();}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return (x+t);}

  virtual std::string name() const {return "function_algebra_translate";}
};



// project to hyperplane {x | <x,normal> = d}
// !! the derivative has rank x.dim() - 1 everywhere !!
class function_algebra_project: public function_algebra_impl {  // works for every dimension!
  typedef function_algebra_project self;
private:
  coord_type normal;
  double dist;
  coord_type derive(const coord_type& /*x*/, const coord_type& h) const { return(h-(h*normal)*normal);}
public:
  function_algebra_project(const coord_type& n, double d) : normal(n), dist(d)
  { is_affine=true;d_Def = d_Im = normal.dim();}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const { return (x - (x*normal-dist)*normal);}

  virtual std::string name() const {return "function_algebra_project";}
};

// distance to hyperplane {x | <x,normal> = dist}
class function_algebra_distance: public function_algebra_impl {  // works for every dimension!
  typedef function_algebra_distance self;
private:
 coord_type normal;
 double dist;
  coord_type derive(const coord_type& /*x*/, const coord_type& h) const { return coord_type(h*normal);}
public:
  function_algebra_distance(const coord_type& n, double d) : normal(n), dist(d)
  {is_affine=true;d_Def = normal.dim();d_Im = 1;}
  virtual self* clone() const { return new self(*this);}
  virtual coord_type eval(const coord_type& x) const { return coord_type(x*normal-dist);}

  virtual std::string name() const {return "function_algebra_distance";}
  virtual std::string write_code_eval(std::ostream& out, int& vnum, 
                                      std::string const& var) const;
  virtual std::string write_code_derive(std::ostream& out, int& vnum, 
                                        std::string const& varx,
                                        std::string const& varh) const;
};

std::string function_algebra_distance::write_code_eval(std::ostream& out, int& vnum, 
                                                       std::string const& var) const
{
  std::string r(makename("dist2hyp",vnum++));
  std::string nrm(makename("n",vnum++));
#ifdef GRAL_HAS_SSTREAM
  std::ostringstream coords; 
#else
  std::ostrstream coords; 
#endif
  coords << normal;
  int dim = normal.dim();
  out << "coord_type "  << nrm << "(makepoint(" << dim << "," << '"' << coords.str() << '"' << "));\n"
      << "double " << r << "( " << nrm << " * " << var << " - " << dist << ");\n"; 
  return r;
}


std::string function_algebra_distance::write_code_derive(std::ostream& out, int& vnum, 
                                                         std::string const& /*varx*/,
                                                         std::string const& varh) const
{
  std::string r(makename("d_dist2hyp",vnum++));
  std::string nrm(makename("n",vnum++));
#ifdef GRAL_HAS_SSTREAM
  std::ostringstream coords; 
#else
  std::ostrstream coords; 
#endif
  coords << normal;
  int dim = normal.dim();
  out << "coord_type "  << nrm << "(makepoint(" << dim << "," << '"' << coords.str() << '"' << "));\n"
      << "double " << r << "( " << nrm << " * " << varh << " - " << dist << ");\n"; 
  return r;
}

template<class T>
inline  T mymin(T i, T k) { return (i<k?i:k);}

static int min_index(const RFunction::coord_type& p) // index with |p[i]| = min p.dim() = 3
{
  double p1 =fabs(p[1]), p2 = fabs(p[2]), p3 = fabs(p[3]);
  if(p1 <= mymin(p2,p3))
    return 1;
  else if (p2 <= mymin(p1,p3))
    return 2;
  else 
    return 3;
}



class function_algebra_rotation3D : public function_algebra_impl {
  typedef function_algebra_rotation3D self;
private:
  coord_type axis; 
  coord_type e1;
  coord_type e2;
  double alpha;
  double cosa;
  double sina;
  
  coord_type derive(const RFunction::coord_type& /*x*/, 
		    const RFunction::coord_type& h) const { return eval(h);}
  //  typedef algebraic_primitives<coord_type> algebra;
  typedef dimension_dependent_primitives_3d<coord_type> ap3d;
public:
  function_algebra_rotation3D(const coord_type& ax, double a) 
    : axis(ap3d::normalization(ax)), 
      e1(3, coord_type::no_init), e2(3, coord_type::no_init), 
      alpha(a), cosa(cos(a)), sina(sin(a)) 
  {
    d_Def = 3;d_Im = 3;
    is_affine = true;
    int i = min_index(axis);
    int j=(i==1 ? 2 : 1);
    e2[i] = 0;
    e2[j] = -axis[6-i-j];
    e2[6-i-j] = axis[j]; // now e2*axis = 0
    e1 = ap3d::vectorproduct(e2,axis);
    if(ap3d::det3(e1,e2,axis) < 0)
      e1 *= -1;
  }
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const 
  {
    double x1 = x*e1;
    double x2 = x*e2;
    double xa = x*axis;
    return (xa* axis + (sina*x1+cosa*x2)*e2 + (cosa*x1 - sina*x2)*e1);
  }

  virtual std::string name() const {return "function_algebra_rotation3D";}
};





class function_algebra_circle : public function_algebra_impl {
  typedef function_algebra_circle self;
private:
  coord_type m;
  double r;
  coord_type derive(const coord_type& x, const coord_type& h) const { return coord_type(((x-m)*h)/(x-m).length());}
public:
  function_algebra_circle(const coord_type& mm, double rr) : m(mm), r(rr) {d_Def = m.dim();d_Im = 1;}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return coord_type((x-m).length() - r);}

  virtual std::string name() const {return "function_algebra_circle";}
};



typedef function_algebra_impl::coord_type coord_type;

RFunction Constant(const coord_type& p, int dDef) { return RFunction(new function_algebra_constant(p,dDef));}
RFunction X(int i)         { return RFunction(new function_algebra_coordinate(i));}
RFunction X(int i,int dim) { return RFunction(new function_algebra_coordinate_with_dim(i,dim));}
RFunction Norm2(int n)     { return RFunction(new function_algebra_norm2(n));}

RFunction Scale    (const coord_type& diag)        { return RFunction(new function_algebra_scale(diag));}
RFunction Translate(const coord_type& t)           { return RFunction(new function_algebra_translate(t));}
RFunction Circle   (const coord_type& m, double r) {return RFunction(new function_algebra_circle(m,r));}

RFunction ProjectToHyperplane (const coord_type& normal, double dist) 
  { return RFunction(new function_algebra_project(normal,dist));}
RFunction DistanceToHyperplane(const coord_type& normal, double dist) 
  { return RFunction(new function_algebra_distance(normal,dist));}
RFunction Rotation3D          (const coord_type& axis, double angle)
  { return RFunction(new function_algebra_rotation3D(axis,angle));}

// this could be implemented more effectively
RFunction Rotation2D(double a) 
{
 double c = cos(a);
 double s = sin(a);
 RFunction X1(X(1,2));
 RFunction X2(X(2,2));
 return ((X1*c-X2*s,X1*s+X2*c));
}

RFunction S3(const double& a, const double& b) { return RFunction(new function_algebra_hermite(a,b));}
RFunction Polynomial1D(const coord_type& c)    { return RFunction(new function_algebra_polynomial1D(c));}

