#include <math.h>
#include <strstream.h>

#include "RFunction/RFunction.h"
#include "RFunction/write_code.h"

point Derivative::operator() (const point& h) const { return f->derive(x,h);} 

class fct_identity : public fct {
private: 
  point derive(const point& /*x*/,const point& h) const {return (h);}
public:
  fct_identity(int d) {d_Im=d_Def=d; is_affine=true;}  
  virtual string name() const {return ("fct_identity");}
  point eval(const point& x) const {return(x);}
  string write_code_eval(ostream& , int& /*vnum*/, const string& var) const
  {return (var);}
  string write_code_derive(ostream&, int& /*vnum*/, const string& /*varx*/, const string& varh) const
  {return (varh);}

};


class fct_sin : public fct {
private:
  point derive(const point& x,const point& h) const {return cos(x[1])*h[1];}
public:
  fct_sin() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_sin";}
  point eval(const point& x) const { return sin(x[1]);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const
  {return write_code_eval_libfct(out,vnum,var,"sin");}
 string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {return write_code_derive_libfct(out,vnum,varx,varh,"cos");}
};

class fct_cos : public fct {
private:
  point derive(const point& x,const point& h) const {return -sin(x[1])*h[1];}
public:
  fct_cos() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_cos";}
  point eval(const point& x) const { return cos(x[1]);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const
  {return write_code_eval_libfct(out,vnum,var,"cos");}
 string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {return write_code_derive_libfct(out,vnum,varx,varh,"-sin");}
};

class fct_atan : public fct {
private:
  point derive(const point& x,const point& h) const {return 1.0/(1.0+x*x)*h[1];}
public:
  fct_atan() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_atan";}
  point eval(const point& x) const { return atan(x[1]);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const
  {return write_code_eval_libfct(out,vnum,var,"atan");}
 string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {
    string r(makename("d_atan",vnum++)); 
    out << "double " << r << "(" << varh << "/(1+sqr( " << varx << ")));\n ";
    return r;
  } 
};

class fct_atan2 : public fct { // = arctan(y/x) = angle[ (1,0),(x,y) ]
private:
  point derive(const point& x,const point& h) const 
  {
   double n= x.length2(); 
   return (point(-x[2]/n,x[1]/n)*h) ;}
public:
  fct_atan2() {d_Im=1;d_Def=2;}
  virtual string name() const {return "fct_atan2";}
  point eval(const point& x) const { return atan2(x[2],x[1]);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const
  { string r(makename("atan2_",vnum++));
   out << "double " << r << "( atan2(" << var << "[2]," << var << "[1]));\n"; 
   return r;}
  string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {
    string norm(makename("norm",vnum++)); 
    string r(makename("d_atan2_",vnum++));
    out << "double " << norm << "(sqr(" << varx << "[1]) + sqr(" << varx << "[2]));\n" 
	<< "double " << r << "((-" << varx << "[2] * " << varh << "[1] + "
	<<  varx << "[1] * " << varh << "[2])/" << norm << ");\n";
    return r;
  }

};


class fct_exp : public fct {
private:
  point derive(const point& x,const point& h) const {return exp(x[1])*h[1];}
public:
  fct_exp() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_exp";}
  point eval(const point& x) const { return exp(x[1]);}
   string write_code_eval(ostream& out, int& vnum, const string& var) const
  {return write_code_eval_libfct(out,vnum,var,"exp");}
 string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {return write_code_derive_libfct(out,vnum,varx,varh,"exp");}
};

class fct_log : public fct {
private:
  point derive(const point& x,const point& h) const {return 1.0/(x[1])*h[1];}
public:
  fct_log() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_log";}
  point eval(const point& x) const { return log(x[1]);}
   string write_code_eval(ostream& out, int& vnum, const string& var) const
  {return write_code_eval_libfct(out,vnum,var,"log");}
 string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {
    string r(makename("d_log",vnum++)); 
    out << "double " << r << "(" << varh << "/("<< varx << "));\n ";
    return r;
  }

};

class fct_sqrt : public fct{
private:
  point derive(const point& x,const point& h) const {return (h[1] *0.5/(eval(x[1]))[1]);}
public:
  fct_sqrt() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_sqrt";}
  point eval(const point& x) const { return sqrt(x[1]);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const
  {return write_code_eval_libfct(out,vnum,var,"sqrt");}
 string write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
  {
    string r(makename("d_sqrt",vnum++)); 
    out << "double " << r << "(" << varh << " * 0.5/(sqrt("<< varx << ")));\n ";
    return r;
  }

};



class fct_sqr : public fct{
private:
  point derive(const point& x,const point& h) const {return (2.0*(x*h));}
public:
  fct_sqr() {d_Im=d_Def=1;}
  virtual string name() const {return "fct_sqr";}
  point eval(const point& x) const { return (x*x);}
  string write_code_eval(ostream& , int& /*vnum*/, const string& var) const
  { ostrstream s; s << "sqr(" << var <<")"; return (s.str());}
  string write_code_derive(ostream& /*out*/, int& /*vnum*/, 
			  const string& varx, const string& varh) const
  { ostrstream s; s << "2.0 * " << varx << " * " << varh; return (s.str());}
};


const RFunction Identity1D(new fct_identity(1));
const RFunction Identity2D(new fct_identity(2));
const RFunction Identity3D(new fct_identity(3));
      RFunction Identity(int  dim)
{return RFunction(new fct_identity(dim));}

const RFunction Sin(new fct_sin());
const RFunction Cos(new fct_cos());
const RFunction Atan(new fct_atan());
const RFunction Atan2(new fct_atan2());
const RFunction Exp(new fct_exp());
const RFunction Log(new fct_log());
const RFunction Sqr(new fct_sqr());
const RFunction Sqrt(new fct_sqrt());
