#include <math.h>

#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include "FunctionAlgebra/function-algebra.h"
#include "./write-code.h"

namespace GrAL {


point Derivative::operator() (const point& h) const { return f->derive(x,h);} 

class function_algebra_identity : public function_algebra_impl {
private: 
  coord_type derive(const coord_type& /*x*/,const coord_type& h) const {return (h);}
public:
  function_algebra_identity(int d) {d_Im=d_Def=d; is_affine=true;}  
  function_algebra_identity  * clone() const { return new function_algebra_identity(*this);}

  virtual coord_type eval(const coord_type& x) const {return(x);}

  virtual ::std::string name() const {return ("function_algebra_identity");}
  virtual ::std::string write_code_eval(::std::ostream& , 
                                      int& /*vnum*/, 
                                      ::std::string const& var) const
  {return var;}

  virtual ::std::string write_code_derive(::std::ostream&, 
                                        int& /*vnum*/, 
                                        ::std::string const& /*varx*/, 
                                        ::std::string const& varh) const
  {return varh;}

};


class function_algebra_sin : public function_algebra_impl {
private:
  coord_type derive(const coord_type& x,const coord_type& h) const 
  {return coord_type(cos(x[1])*h[1]);}
public:
  function_algebra_sin() {d_Im=d_Def=1;}
  function_algebra_sin  * clone() const { return new function_algebra_sin(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(sin(x[1]));}

  virtual ::std::string name() const {return "function_algebra_sin";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const
   { return write_code_eval_libfct(out,vnum,var,"sin"); }
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const
  { return write_code_derive_libfct(out,vnum,varx,varh,"cos");}
};



class function_algebra_cos : public function_algebra_impl {
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {return coord_type(-sin(x[1])*h[1]);}
public:
  function_algebra_cos() {d_Im=d_Def=1;}
  function_algebra_cos  * clone() const { return new function_algebra_cos(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(cos(x[1]));}


  virtual ::std::string name() const {return "function_algebra_cos";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const
  {return write_code_eval_libfct(out,vnum,var,"cos");}
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const
  {return write_code_derive_libfct(out,vnum,varx,varh,"-sin");}
};



class function_algebra_atan : public function_algebra_impl {
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {
    return coord_type(1.0/(1.0+x*x)*h[1]);
  }
public:
  function_algebra_atan() {d_Im=d_Def=1;}
  function_algebra_atan  * clone() const { return new function_algebra_atan(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(atan(x[1]));}

  virtual ::std::string name() const {return "function_algebra_atan";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const
  {return write_code_eval_libfct(out,vnum,var,"atan");}

  virtual  ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                         ::std::string const& varx, ::std::string const& varh) const
  {
    ::std::string r(makename("d_atan",vnum++)); 
    out << "double " << r << "(" << varh << "/(1+sqr( " << varx << ")));\n ";
    return r;
  } 
};



class function_algebra_atan2 : public function_algebra_impl { // = arctan(y/x) = angle[ (1,0),(x,y) ]
private:
  coord_type derive(const coord_type& x,const coord_type& h) const 
  {
    double n= x.length2(); 
    return coord_type(coord_type(-x[2]/n,x[1]/n)*h);
  }
public:
  function_algebra_atan2() {d_Im=1;d_Def=2;}
  function_algebra_atan2  * clone() const { return new function_algebra_atan2(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(atan2(x[2],x[1]));}


  virtual ::std::string name() const {return "function_algebra_atan2";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const
  { 
    ::std::string r(makename("atan2_",vnum++));
    out << "double " << r << "( atan2(" << var << "[2]," << var << "[1]));\n"; 
    return r;
  }
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const
  {
    ::std::string norm(makename("norm",vnum++)); 
    ::std::string r   (makename("d_atan2_",vnum++));
    out << "double " << norm << "(sqr(" << varx << "[1]) + sqr(" << varx << "[2]));\n" 
	<< "double " << r << "((-" << varx << "[2] * " << varh << "[1] + "
	<<  varx << "[1] * " << varh << "[2])/" << norm << ");\n";
    return r;
  }

};


class function_algebra_exp : public function_algebra_impl {
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {return coord_type(exp(x[1])*h[1]);}
public:
  function_algebra_exp() {d_Im=d_Def=1;}
  function_algebra_exp  * clone() const { return new  function_algebra_exp(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(exp(x[1]));}


  virtual ::std::string name() const {return "function_algebra_exp";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const
  {return write_code_eval_libfct(out,vnum,var,"exp");}

  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const
  {return write_code_derive_libfct(out,vnum,varx,varh,"exp");}
};



class function_algebra_log : public function_algebra_impl {
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {return coord_type(1.0/(x[1])*h[1]);}
public:
  function_algebra_log() {d_Im=d_Def=1;}
  function_algebra_log  * clone() const { return  new function_algebra_log(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(log(x[1]));}

  virtual ::std::string name() const {return "function_algebra_log";}
  virtual  ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                       ::std::string const& var) const
  {return write_code_eval_libfct(out,vnum,var,"log");}
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const
  {
    ::std::string r(makename("d_log",vnum++)); 
    out << "double " << r << "(" << varh << "/("<< varx << "));\n ";
    return r;
  }

};



class function_algebra_sqrt : public function_algebra_impl{
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {
    return coord_type(h[1] *0.5/(eval(coord_type(x[1])))[1]);}
public:
  function_algebra_sqrt() {d_Im=d_Def=1;}
  function_algebra_sqrt  * clone() const { return new function_algebra_sqrt(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(sqrt(x[1]));}

  virtual ::std::string name() const {return "function_algebra_sqrt";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const
  {return write_code_eval_libfct(out,vnum,var,"sqrt");}
  virtual  ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                         ::std::string const& varx, ::std::string const& varh) const
  {
    ::std::string r(makename("d_sqrt",vnum++)); 
    out << "double " << r << "(" << varh << " * 0.5/(sqrt("<< varx << ")));\n ";
    return r;
  }

};



class function_algebra_sqr : public function_algebra_impl{
private:
  coord_type derive(const coord_type& x,const coord_type& h) const {return coord_type((2.0*(x*h)));}
public:
  function_algebra_sqr() {d_Im=d_Def=1;}
  function_algebra_sqr  * clone() const { return new  function_algebra_sqr(*this);}

  virtual coord_type eval(const coord_type& x) const { return coord_type(x*x);}

  virtual ::std::string name() const {return "function_algebra_sqr";}
  virtual ::std::string write_code_eval(::std::ostream& , int& /*vnum*/, 
                                      ::std::string const& var) const
  { 
#ifdef GRAL_HAS_SSTREAM
    ::std::ostringstream s; 
#else
    ::std::ostrstream s; 
#endif
    s << "sqr(" << var <<")"; return (s.str());
  }
  virtual  ::std::string write_code_derive(::std::ostream& /*out*/, int& /*vnum*/, 
                                         ::std::string const& varx, ::std::string const& varh) const
  { 
#ifdef GRAL_HAS_SSTREAM
    ::std::ostringstream s; 
#else
    ::std::ostrstream s; 
#endif  
    s << "2.0 * " << varx << " * " << varh; return (s.str());}
};


const RFunction Identity1D(new function_algebra_identity(1));
const RFunction Identity2D(new function_algebra_identity(2));
const RFunction Identity3D(new function_algebra_identity(3));
      RFunction Identity(int  dim)
{return RFunction(new function_algebra_identity(dim));}

const RFunction Sin(new function_algebra_sin());
const RFunction Cos(new function_algebra_cos());
const RFunction Atan(new function_algebra_atan());
const RFunction Atan2(new function_algebra_atan2());
const RFunction Exp(new function_algebra_exp());
const RFunction Log(new function_algebra_log());
const RFunction Sqr(new function_algebra_sqr());
const RFunction Sqrt(new function_algebra_sqrt());

} // namespace GrAL 
