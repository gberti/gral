#include <strstream.h>

#include "RFunction/RFunction.h"
#include "RFunction/misc.h"
#include "RFunction/write_code.h"
#include "Geometry/algebraic-primitives.h"


// some concrete classes defining often used RFunctions, mainly
// geometric transformations and projections. See end of file
// for list of defined RFunctions

class fct_constant: public fct {
private:
  point  derive(const point& /*x*/, const point& /*h*/) const { return (0.0);}
protected:
  point c;
public:
  fct_constant(const point& cc, int ddef) : fct(), c(cc)
  {d_Def = ddef; d_Im = c.dim(), is_affine = true;}
  ~fct_constant() {}
  string name() const { return "fct_constant";}
  point  eval(const point& /*x*/) const {return c;}
  string  write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				   const string& varx, const string& varh) const;
};


string  fct_constant::write_code_eval(ostream&, int& /*vnum*/, const string& /*var*/) const
{ 
 strstream s;
 if (dIm() == 1)
   s << c[1];
 else {
   s << "point((const double[]){";
   for(int i =1; i<=dIm(); i++)
     s << c[i] << ",";
   s << "}, "  << dIm() << ")";
 }
 return s.str();
}
 
string fct_constant::write_code_derive(ostream&, int& /*vnum*/, 
				   const string& /*varx*/, const string& /*varh*/) const
{
 strstream s;
 if (dIm() == 0)
   s << "0.0";
 else
   s << "point(" << dIm() << ", zero_init)";
 return s.str();
}

class fct_coordinate : public fct {
private: 
  point derive(const point& /*x*/, const point& h) const { return h[i];}
protected:
  int i;
public:
  fct_coordinate(int ii) : i(ii) {is_affine=true;d_Im=1;d_Def=ANYDIM;} 
  ~fct_coordinate() {}
  virtual string name() const {return "fct_coordinate";}
  point eval(const point& x) const {return x[i];}
  string  write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

string  fct_coordinate::write_code_eval(ostream&, int& /*vnum*/, 
						const string& var) const
{ 
  ostrstream s;
  if (d_Def == 1) {s << var;} else  {s << var << "[" << i <<"]";} 
  return(s.str());
}

string fct_coordinate::write_code_derive(ostream& , int& /*vnum*/, 
					       const string& /*varx*/, const string& varh) const
{ 
  ostrstream s;
  if (d_Def == 1) {s << varh;} else  {s << varh << "[" << i <<"]";} 
  return(s.str());
}

class fct_coordinate_with_dim : public fct_coordinate {
private: 
public:
  fct_coordinate_with_dim(int ii, int dim) : fct_coordinate(ii) {d_Im=1;d_Def=dim;} 
  ~fct_coordinate_with_dim() {}
  virtual string name() const {return "fct_coordinate_with_dim";}
};




// an S-shaped function: f(x) = 0 on ]-infty,min(a,b)], f(x) = 1 on [max(a,b),infty[
// f cubic between a and b with f' = 0 at a and b

class fct_hermite: public fct { 
private:
  double a,b;
  point derive(const point& x, const point& h) const;
public:
  fct_hermite(const double& aa, const double& bb) : a(aa), b(bb) {d_Im=d_Def=1;}
  ~fct_hermite() {}
  virtual string name() const {return "fct_hermite";}
  point eval(const point& x) const;
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};


point fct_hermite::eval(const point& x) const
{ return hermite(a,b,x[1]);}

string fct_hermite::write_code_eval(ostream& out, int& vnum, const string& var) const
{ 
  string r(makename("herm",vnum++));
  out << "double " << r << "( hermite(" << a << "," << b << "," << var << "));\n";
  return r;
}


string fct_hermite::write_code_derive(ostream& out, int& vnum, 
				     const string& varx, const string& varh) const
{
  string r(makename("d_herm",vnum++));
  out << "double " << r << "( d_hermite(" << a << "," << b << "," << varx << ") * " << varh << ");\n";
  return r;
}

point fct_hermite::derive(const point& x,const point& h) const
{
  double t = (x[1]-a)/(b-a);
  if (t <= 0.0) return 0.0;
  else if (t<= 1.0) return (h*(6.0*t*(1.0-t)/(b-a)) );
  else return 0.0;
}




// a 1-dimensional polynome, initialized with a coefficient vector c,
// polynome = c[n] x^{n-1} + ... + c[2] x + c[1]

class fct_polynome1D : public fct {
private:
  point coeffs;
  point derive(const point& x, const point& h) const; // may also be defined for doubles
public:
  fct_polynome1D(const point& c) : coeffs(c) {d_Def=1;d_Im=1;}
  ~fct_polynome1D() {}
  virtual string name() const {return "fct_polynome1D";}
  int degree() const { return (coeffs.dim() -1);}
  point eval(const point& x) const; 
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_deriv(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

string fct_polynome1D::write_code_eval(ostream& out, int& vnum, const string& var) const
{
  string bk(makename("bk",vnum++));
  string x1(makename("xtmp",vnum));
  string r(makename("poly",vnum));

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

point fct_polynome1D::eval(const point& x) const
{
  int n= degree();
  double bk=coeffs[n+1],x1=x[1];
  for(int k=1;k<=n;k++) {
    bk = x1*bk +coeffs[n+1-k];
  }
  return bk;
}

point fct_polynome1D::derive(const point& x, const point& h) const
{
  int n= degree();
  double bk=n*coeffs[n+1],x1=x[1];
  for(int k=1;k<=n-1;k++) {
    bk = x1*bk +(n-k)*coeffs[n+1-k];
  }
  return bk*h[1];
}

string fct_polynome1D::write_code_deriv(ostream& out, int& vnum, 
					const string& varx, const string& varh) const
{
  string bk(makename("bk",vnum++));
  string x1(makename("xtmp",vnum));
  string r(makename("poly",vnum));

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



class fct_norm2: public fct { // works for every dimension!
private:
  point derive(const point& x,const point& h) const {return 2*(x*h);}
public:
  fct_norm2(int n) {d_Def = n; d_Im = 1;}
  virtual string name() const {return "fct_norm2";}
  point eval(const point& x) const { return (x.length2());}
};


////////////////////////////////////////////////////////////////////////
//
//   some special affine-linear functions
//
////////////////////////////////////////////////////////////////////////


class fct_scale: public fct {  // works for every dimension!
private:
 point diag;
  point derive(const point& /*x*/, const point& h) const { return eval(h);}
public:
  fct_scale(const point& d) : diag(d) {is_affine=true;d_Def = d_Im = diag.dim();}
  virtual string name() const {return "fct_scale";}
  inline point eval(const point& x) const;
};

inline  point fct_scale::eval(const point& x) const
{
  point p(x); 
  for(int i=1;i<=p.dim();i++)
    p[i]*=diag[i];
  return p;
}

class fct_translate : public fct {
private:
 point t;
  point derive(const point& /*x*/, const point& h) const { return h;}
public:
  fct_translate(const point& tt) : t(tt)  {is_affine=true; d_Def = d_Im = t.dim();}
  virtual string name() const {return "fct_translate";}
  point eval(const point& x) const {return (x+t);}
};



// project to hyperplane {x | <x,normal> = d}
// !! the derivative has rank x.dim() - 1 everywhere !!
class fct_project: public fct {  // works for every dimension!
private:
  point normal;
  double dist;
  point derive(const point& /*x*/, const point& h) const { return(h-(h*normal)*normal);}
public:
  fct_project(const point& n, double d) : normal(n), dist(d)
  { is_affine=true;d_Def = d_Im = normal.dim();}
  virtual string name() const {return "fct_project";}
  point eval(const point& x) const { return (x - (x*normal-dist)*normal);}
};

// distance to hyperplane {x | <x,normal> = dist}
class fct_distance: public fct {  // works for every dimension!
private:
 point normal;
 double dist;
  point derive(const point& /*x*/, const point& h) const { return(h*normal);}
public:
  fct_distance(const point& n, double d) : normal(n), dist(d)
  {is_affine=true;d_Def = normal.dim();d_Im = 1;}
  virtual string name() const {return "fct_distance";}
  point eval(const point& x) const { return (x*normal-dist);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  string write_code_derive(ostream& out, int& vnum, 
			   const string& varx, const string& varh) const;
};

string fct_distance::write_code_eval(ostream& out, int& vnum, const string& var) const
{
  string r(makename("dist2hyp",vnum++));
  string nrm(makename("n",vnum++));
  ostrstream coords;coords << normal;
  int dim = normal.dim();
  out << "point "  << nrm << "(makepoint(" << dim << "," << '"' << coords.str() << '"' << "));\n"
      << "double " << r << "( " << nrm << " * " << var << " - " << dist << ");\n"; 
  return r;
}


string fct_distance::write_code_derive(ostream& out, int& vnum, 
				       const string& /*varx*/,const string& varh) const
{
  string r(makename("d_dist2hyp",vnum++));
  string nrm(makename("n",vnum++));
  ostrstream coords;coords << normal;
  int dim = normal.dim();
  out << "point "  << nrm << "(makepoint(" << dim << "," << '"' << coords.str() << '"' << "));\n"
      << "double " << r << "( " << nrm << " * " << varh << " - " << dist << ");\n"; 
  return r;
}

template<class T>
static inline  T min(T i, T k) { return (i<k?i:k);}

static int min_index(const point& p) // index with |p[i]| = min p.dim() = 3
{
  double p1 =fabs(p[1]), p2 = fabs(p[2]), p3 = fabs(p[3]);
  if(p1 <= min(p2,p3))
    return 1;
  else if (p2 <= min(p1,p3))
    return 2;
  else 
    return 3;
}

class fct_rotation3D : public fct {
private:
  point axis; 
  point e1;
  point e2;
  double alpha;
  double cosa;
  double sina;
  
  point derive(const point& /*x*/, const point& h) const { return eval(h);}
  //  typedef algebraic_primitives<point> algebra;
  typedef dimension_dependent_primitives_3d<point> algebra;
public:
  fct_rotation3D(const point& ax, double a) 
    : axis(algebra::normalization(ax)), 
      e1(3,no_init), e2(3,no_init), 
      alpha(a), cosa(cos(a)), sina(sin(a)) 
  {
    d_Def = 3;d_Im = 3;
    is_affine = true;
    int i = min_index(axis);
    int j=(i==1 ? 2 : 1);
    e2[i] = 0;
    e2[j] = -axis[6-i-j];
    e2[6-i-j] = axis[j]; // now e2*axis = 0
    e1 = algebra::vectorproduct(e2,axis);
    if(algebra::det3(e1,e2,axis) < 0)
      e1 *= -1;
}

  virtual string name() const {return "fct_rotation3D";}
  point eval(const point& x) const 
  {
    double x1 = x*e1;
    double x2 = x*e2;
    double xa = x*axis;
    return (xa* axis + (sina*x1+cosa*x2)*e2 + (cosa*x1 - sina*x2)*e1);
  }
};





class fct_circle : public fct {
private:
  point m;
  double r;
  point derive(const point& x, const point& h) const { return ((x-m)*h)/(x-m).length();}
public:
  fct_circle(const point& mm, double rr) : m(mm), r(rr) {d_Def = m.dim();d_Im = 1;}
  virtual string name() const {return "fct_circle";}
  point eval(const point& x) const {return ((x-m).length() - r);}
};




RFunction Constant(const point& p, int dDef) { return RFunction(new fct_constant(p,dDef));}
RFunction X(int i) {return RFunction(new fct_coordinate(i));}
RFunction X(int i,int dim) {return RFunction(new fct_coordinate_with_dim(i,dim));}
RFunction Norm2(int n) { return RFunction(new fct_norm2(n));}
RFunction Scale(const point& diag) { return RFunction(new fct_scale(diag));}
RFunction Translate(const point& t) { return RFunction(new fct_translate(t));}
RFunction Circle(const point& m, double r) {return RFunction(new fct_circle(m,r));}
RFunction ProjectToHyperplane(const point& normal, double dist) 
{return RFunction(new fct_project(normal,dist));}
RFunction DistanceToHyperplane(const point& normal, double dist) 
{return RFunction(new fct_distance(normal,dist));}
RFunction Rotation3D(const point& axis, double angle)
{ return RFunction(new fct_rotation3D(axis,angle));}

// this could be implemented more effectively
RFunction Rotation2D(double a) 
{
 double c = cos(a);
 double s = sin(a);
 RFunction X1(X(1,2));
 RFunction X2(X(2,2));
 return ((X1*c-X2*s,X1*s+X2*c));
}

RFunction S3(const double& a, const double& b) { return RFunction(new fct_hermite(a,b));}
RFunction Polynome1D(const point& c) { return RFunction(new fct_polynome1D(c));}

