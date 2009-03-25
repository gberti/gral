
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include "FunctionAlgebra/function-algebra.h"
#include "./write-code.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {


// concrete classes implementing the operations on RFunctions


class function_algebra_null: public function_algebra_impl {
private:
  typedef function_algebra_null self;
  virtual coord_type derive(const coord_type& /*x*/, const coord_type& /*h*/) const 
  {return coord_type(d_Im, coord_type::zero_init);}
public:
  function_algebra_null(int dD=1, int dI=1) {d_Def= dD;d_Im=dI;}
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& /*x*/) const { return coord_type(d_Im, coord_type::zero_init);}

  virtual ::std::string name() const { return "function_algebra_null";} 
  virtual ::std::string write_code_eval(::std::ostream&,int&, 
                                      ::std::string const&) const
  { 
    if (d_Im == 1)
      return( "0.0");
    else {
#ifdef GRAL_HAS_SSTREAM
      ::std::ostringstream s; 
#else
      ::std::ostrstream s; 
#endif
      s << "coord_type(" << d_Im << ",zero_init)";
      return s.str();
    }
  }

  virtual ::std::string write_code_derive(::std::ostream&,int& , 
                                        ::std::string const&, 
                                        ::std::string const&) const
  { 
    if (d_Im == 1)
      return( "0.0");
    else {
#ifdef GRAL_HAS_SSTREAM
      ::std::ostringstream s; 
#else
      ::std::ostrstream s; 
#endif
      s << "coord_type(" << d_Im << ",zero_init)";
      return s.str();
    }
  } 
};

class function_algebra_composition : public function_algebra_impl {
private:
  typedef function_algebra_composition self;
  RFunction F1,F2; 

protected:
  virtual coord_type derive(const coord_type& x, const coord_type& h) const
  {return (F1.derive(F2(x),F2.derive(x,h)) );}
public:
  function_algebra_composition(const RFunction& Fa, const RFunction& Fb) // fa o fb
    : F1(Fa), F2(Fb)
  {
    d_Def = Fb.dDef(); 
    d_Im  = Fa.dIm(); 
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  } 
  virtual self* clone() const { return new self(*this);}

  coord_type eval(const coord_type& x) const { return F1(F2(x));}

  virtual ::std::string name() const {return "function_algebra_composition";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum,
                                      ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const;

};


class function_algebra_combination : public function_algebra_impl {
private:
  typedef function_algebra_combination self;
  RFunction F1,F2;
  virtual coord_type derive(const coord_type& x, const coord_type& h) const 
  { return combine(F1.derive(x,h),F2.derive(x,h));}
public:
  function_algebra_combination(const RFunction& Fa, const RFunction& Fb) // (fa,fb)
    :  F1(Fa), F2(Fb) 
  {
    d_Def = Fa.dDef();
    d_Im  = Fa.dIm() + Fb.dIm();
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  } 
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return combine(F1(x),F2(x));}

  virtual ::std::string name() const {return "function_algebra_combination";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
				 ::std::string const& varx, ::std::string const& varh) const;
};

class function_algebra_multiplication : public function_algebra_impl {
private:
  typedef function_algebra_multiplication self;
  RFunction F1,F2;

  coord_type derive(const coord_type& x, const coord_type& h) const 
  { return coord_type(F1.derive(x,h)*F2(x) + F1(x)*F2.derive(x,h));}
public:
  function_algebra_multiplication(const RFunction& Fa, const RFunction& Fb) :  F1(Fa), F2(Fb) 
  {
    REQUIRE((F1.dDef() == F2.dDef()),
	    "F1*F2: F1.dDef() = " << F1.dDef() << "  != " << F2.dDef() << " = F2.dDef()!\n",1);
    d_Def = F1.dDef();
    if(F1.dIm() == 1)
      d_Im = F2.dIm();
    else if(F2.dIm() == 1)
      d_Im = F1.dIm();
    else if(F1.dIm() == F2.dIm())
      d_Im = F1.dIm();
    else
      REQUIRE((F1.dIm() == F2.dIm()),
	      "F1*F2: F1.dIm() = " << F1.dIm() << "  != " << F2.dIm() << " = F2.dIm()!\n",1);
  }

  virtual self* clone() const { return new self(*this);}

  coord_type eval(const coord_type& x) const 
  {
    if(F1.dIm() == 1)
      return (F1(x)[1]*F2(x)); // operator * (double, coord_type)
    if(F2.dIm() == 1)
      return (F1(x)*F2(x)[1]); // operator * (coord_type, double)
    else
      return coord_type(F1(x)*F2(x));    // operator * (coord_type, coord_type)
  }

  virtual ::std::string name() const {return "function_algebra_multiplication";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const;
};

class function_algebra_division : public function_algebra_impl { // F1 / F2
private:
  typedef function_algebra_division self;
  RFunction F1,F2;
  coord_type derive(const coord_type& x, const coord_type& h) const 
  { 
    coord_type  F2x(F2(x)[1]);  
    return coord_type((F1.derive(x,h)*F2x - F2.derive(x,h)*F1(x))/ (F2x*F2x));
  }
public:
  function_algebra_division(const RFunction& Fa, const RFunction& Fb) : F1(Fa), F2(Fb) { 
    REQUIRE((Fb.dIm() == 1 ), "fct: Fa/Fb : dim Im(Fb) = " << Fb.dIm() << " != 1!\n",1);
    d_Def = Fa.dDef();d_Im = Fa.dIm();
  }
  virtual self* clone() const { return new self(*this);}

  coord_type eval(const coord_type& x) const {
    coord_type F1x(F1(x)); 
    double F2x = F2(x)[1];
    return (F1x/F2x);
  }

  virtual ::std::string name() const {return "function_algebra_division";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
				 ::std::string const& varx, ::std::string const& varh) const;
};

class function_algebra_div : public function_algebra_impl { // 1 / F
private:
  typedef function_algebra_div self;
  RFunction F;
  coord_type derive(const coord_type& x, const coord_type& h) const 
  { coord_type Fx(F(x)[1]);  return (- d(F,x)(h) / (Fx*Fx));}
public:
  function_algebra_div(const RFunction& Fa) : F(Fa) {
    REQUIRE((F.dIm() == 1 ),"fct: 1/F : dim Im(F) = " <<  F.dIm() << " > 1!",1);
    d_Def = F.dDef();d_Im = F.dIm();
  }
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return coord_type(1.0/(F(x))[1]);}

  virtual ::std::string name() const {return "function_algebra_inverse";}

  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
				 ::std::string const& varx, ::std::string const& varh) const;
};


class function_algebra_addition : public function_algebra_impl {
private:
  typedef function_algebra_addition self;
  RFunction F1,F2;
  coord_type derive(const coord_type& x, const coord_type& h) const 
  { return (F1.derive(x,h)+F2.derive(x,h));}
public:
  function_algebra_addition(const RFunction& Fa, const RFunction& Fb)
    : F1(Fa), F2(Fb) 
  {
    d_Def = Fa.dDef();
    d_Im = Fa.dIm();
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  }
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return (F1(x)+F2(x));}

  virtual ::std::string name() const {return "function_algebra_addition";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const;

};



class function_algebra_subtraction : public function_algebra_impl {
private:
  typedef function_algebra_subtraction self;
  RFunction F1,F2;
  coord_type derive(const coord_type& x, const coord_type& h) const 
  { return (F1.derive(x,h)-F2.derive(x,h));}
public:
  function_algebra_subtraction(const RFunction& Fa, const RFunction& Fb) 
    :  F1(Fa), F2(Fb) 
  {
    d_Def = Fa.dDef();
    d_Im = Fa.dIm();
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  }
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const {return (F1(x)-F2(x));}

  virtual ::std::string name() const {return "function_algebra_subtraction";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, 
                                      ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
                                        ::std::string const& varx, ::std::string const& varh) const;
};



class function_algebra_piecewise: public function_algebra_impl {
private:
  typedef function_algebra_piecewise self;
  RFunction F0,Fneg,Fpos;
  coord_type derive(const coord_type& x, const coord_type& h) const 
  { return(F0(x)[1] > 0 ? d(Fpos,x)(h) : d(Fneg,x)(h));}
public:
  function_algebra_piecewise(const RFunction& f0,const RFunction& fneg,const RFunction& fpos);
  virtual self* clone() const { return new self(*this);}

  virtual coord_type eval(const coord_type& x) const { return(F0(x)[1] > 0 ? Fpos(x) : Fneg(x));}

  virtual ::std::string name() const {return "function_algebra_piecewise";}
  virtual ::std::string write_code_eval(::std::ostream& out, int& vnum, ::std::string const& var) const;
  virtual ::std::string write_code_derive(::std::ostream& out, int& vnum, 
				 ::std::string const& varx, ::std::string const& varh) const;
};

inline  function_algebra_piecewise::function_algebra_piecewise(const RFunction& f0,
                                                               const RFunction& fneg,
                                                               const RFunction& fpos) 
  :  F0(f0), Fneg(fneg), Fpos(fpos) 
{
  REQUIRE(((F0.dIm() == 1) && (Fneg.dIm() == Fpos.dIm()) && (Fneg.dDef() == Fpos.dDef())),
	  "function_algebra_piecewise: dimensions do not correspond!\n",1);
  d_Def=Fneg.dDef();d_Im=Fneg.dIm();
}






//////////////////////////////////////////////////////////////////////
//
//      implementation of output-methods
//


static ::std::string write_code_eval_bin_op(const RFunction& F1, const RFunction F2,
                                          ::std::ostream& out, int& vnum, 
                                          ::std::string const& varbasename, char op,
                                          ::std::string const& var, int dIm)
{
  ::std::string r1( F1.write_code_eval(out,vnum,var));
  ::std::string r2( F2.write_code_eval(out,vnum,var));
  ::std::string r3(makename(varbasename,vnum++));
  if(dIm == 1)
    out << "double ";
  else
    out << "coord_type ";
  out << r3 << "( " << r1 << " " << op << " " << r2 << " );\n"; 
 return r3;
}




std::string function_algebra_div::write_code_eval(::std::ostream& out, int& vnum, 
                                                  ::std::string const& var) const
{
 ::std::string r1( F.write_code_eval(out,vnum,var));
 ::std::string r(makename("inv",vnum++));
 out << "double " << r << "(1.0/(" << r1 << "));\n";
 return r;
}

std::string function_algebra_div::write_code_derive(::std::ostream& out, int& vnum,
                                                    ::std::string const& varx, ::std::string const& varh) const
{
 ::std::string df( F.write_code_derive(out,vnum,varx,varh));
 ::std::string f( F.write_code_eval(out,vnum,varx));
 ::std::string r(makename("d_inv",vnum++));
 out << "double " << r << "(-" << df << "/(sqr(" << f << ")));\n";
 return r;
}

std::string function_algebra_multiplication::write_code_eval(::std::ostream& out, int& vnum, 
                                                             ::std::string const& var) const
{ return write_code_eval_bin_op(F1,F2,out,vnum,"prod",'*',var,dIm());}

std::string function_algebra_multiplication::write_code_derive(::std::ostream& out, int& vnum, 
                                                               ::std::string const& varx, 
                                                               ::std::string const& varh) const
{
 ::std::string f1( F1.write_code_eval(out,vnum,varx));
 ::std::string df1( F1.write_code_derive(out,vnum,varx,varh));
 ::std::string f2( F2.write_code_eval(out,vnum,varx));
 ::std::string df2( F2.write_code_derive(out,vnum,varx,varh));
 ::std::string rtype = ( dIm() == 1? "double" : "coord_type");
 ::std::string r(makename("d_mult",vnum++));
 out << rtype << " " << r << "(" << f1 << "*" << df2 << " + " << df1 << "*" << f2 << ");\n";
 return r;
}

std::string function_algebra_division::write_code_eval(::std::ostream& out, int& vnum, 
                                                       ::std::string const& var) const
{ return write_code_eval_bin_op(F1,F2,out,vnum,"quot",'/',var,dIm());}

std::string function_algebra_division::write_code_derive(::std::ostream& out, int& vnum, 
                                                         ::std::string const& varx, 
                                                         ::std::string const& varh) const
{
 ::std::string f1( F1.write_code_eval(out,vnum,varx));
 ::std::string df1( F1.write_code_derive(out,vnum,varx,varh));
 ::std::string f2( F2.write_code_eval(out,vnum,varx));
 ::std::string df2( F2.write_code_derive(out,vnum,varx,varh));
 ::std::string rtype = ( dIm() == 1? "double" : "coord_type");
 ::std::string r(makename("d_div",vnum++));
 out << rtype << " " << r << "((-" << f1 << "*" << df2 
     << " + " << df1 << "*" << f2 << ")/sqr(" << f2 << "));\n";
 return r;
}


std::string function_algebra_addition::write_code_eval(::std::ostream& out, int& vnum, 
                                                       ::std::string const& var) const 
{ return write_code_eval_bin_op(F1,F2,out,vnum,"sum",'+',var,dIm());}

std::string function_algebra_addition::write_code_derive(::std::ostream& out, int& vnum, 
                                                         ::std::string const& varx, 
                                                         ::std::string const& varh) const
{
  ::std::string df1( F1.write_code_derive(out,vnum,varx,varh));
  ::std::string df2( F2.write_code_derive(out,vnum,varx,varh));
  ::std::string rtype = ( dIm() == 1? "double" : "coord_type");
  ::std::string r(makename("d_add",vnum++));
  out << rtype << " " << r << "(" << df1 << " + " << df2 << ");\n";
  return r;
}


std::string function_algebra_subtraction::write_code_eval(::std::ostream& out, int& vnum, 
                                                          ::std::string const& var) const
{ return write_code_eval_bin_op(F1,F2,out,vnum,"diff",'-',var,dIm());}

std::string function_algebra_subtraction::write_code_derive(::std::ostream& out, int& vnum, 
                                                            ::std::string const& varx, 
                                                            ::std::string const& varh) const
{
 ::std::string df1( F1.write_code_derive(out,vnum,varx,varh));
 ::std::string df2( F2.write_code_derive(out,vnum,varx,varh));
 ::std::string rtype = ( dIm() == 1? "double" : "coord_type");
 ::std::string r(makename("d_sub",vnum++));
 out << rtype << " " << r << "(" << df1 << " - " << df2 << ");\n";
 return r;
}



std::string function_algebra_piecewise::write_code_eval(::std::ostream& out, int& vnum, 
                                                        ::std::string const& var) const
{
  ::std::string r0(     F0.write_code_eval(out,vnum,var));
  ::std::string rneg( Fneg.write_code_eval(out,vnum,var));
  ::std::string rpos( Fpos.write_code_eval(out,vnum,var));
  ::std::string r(makename("pw",vnum++));
  ::std::string rtype( (dIm() == 1? "double" : "coord_type"));
  out << rtype << " " << r << "( (" << r0 << " < 0.0 ? " << rneg << " : " << rpos << "));\n"; 
  return r;
}


std::string function_algebra_piecewise::write_code_derive(::std::ostream& out, int& vnum, 
                                                          ::std::string const& varx, 
                                                          ::std::string const& varh) const
{
  ::std::string r0(     F0.write_code_eval(out,vnum,varx));
  ::std::string drneg( Fneg.write_code_derive(out,vnum,varx,varh));
  ::std::string drpos( Fpos.write_code_derive(out,vnum,varx,varh));
  ::std::string r(makename("d_pw",vnum++));
  ::std::string rtype( (dIm() == 1? "double" : "coord_type"));
  out << rtype << " " << r << "( (" << r0 << " < 0.0 ? " << drneg << " : " << drpos << "));\n"; 
  return r;
}

std::string function_algebra_composition::write_code_eval(::std::ostream& out, int& vnum, 
                                                          ::std::string const& var) const
{ return F1.write_code_eval(out,vnum, F2.write_code_eval(out,vnum,var));}

std::string function_algebra_composition::write_code_derive(::std::ostream& out, int& vnum, 
                                                            ::std::string const& varx, 
                                                            ::std::string const& varh) const
{
  ::std::string df2(F2.write_code_derive(out,vnum,varx,varh));
  return F1.write_code_derive(out,vnum,F2.write_code_eval(out,vnum,varx),df2);
}


std::string function_algebra_combination::write_code_eval(::std::ostream& out, int& vnum, 
                                                          ::std::string const& var) const
{
  ::std::string r1( F1.write_code_eval(out,vnum,var));
  ::std::string r2( F2.write_code_eval(out,vnum,var));
  ::std::string r3(makename("comb",vnum++));
  ::std::string rtype = ( dIm() == 1? "double" : "coord_type");
  out << rtype << " " << r3 << "( (coord_type(" << r1 << ") , coord_type(" << r2 << ")) );\n"; 
  return r3;
}


std::string function_algebra_combination::write_code_derive(::std::ostream& out, int& vnum,
                                                            ::std::string const& varx, 
                                                            ::std::string const& varh) const
{
  ::std::string dr1( F1.write_code_derive(out,vnum,varx,varh));
  ::std::string dr2( F2.write_code_derive(out,vnum,varx,varh));
  ::std::string r3(makename("d_comb",vnum++));
  ::std::string rtype = ( dIm() == 1? "double" : "coord_type");
  out << rtype << " " << r3 << "( (coord_type(" << dr1 << ") , coord_type(" << dr2 << ")) );\n"; 
  return r3;
}




// operators on RFunctions, returning appropriate subclasses
// of class RFunction implementing the operation 


RFunction NullFct(int dD,int dI) { return RFunction(new function_algebra_null(dD,dI));}

RFunction::RFunction()  : ff(new function_algebra_null(0,0)), a(1.0), b(0)  {}
  
RFunction RFunction::operator() (const RFunction& g) const
{ return RFunction(new function_algebra_composition(*this,g));}

/*
RFunction operator ,  (const RFunction& f, const RFunction& g)
{ return RFunction(new function_algebra_combination(f,g));}
*/

RFunction combine(const RFunction& f, const RFunction& g)
{ return RFunction(new function_algebra_combination(f,g));}


RFunction operator *  (const RFunction& f, const RFunction& g)
  { RFunction ff(new function_algebra_multiplication(f,g));
  return ff;}

RFunction operator * (const RFunction::coord_type& p, const RFunction& f)
{ return (f*Constant(p,f.dDef()));}

RFunction operator * (const RFunction& f, const RFunction::coord_type& p)
{ return (f*Constant(p,f.dDef()));}

RFunction operator /  (const RFunction& f, const RFunction& g) // f/g
{ return RFunction(new function_algebra_division(f,g));}

RFunction operator /  (const double& a, const RFunction& g) // a/g
{   RFunction f(new function_algebra_div(g));  return (f*a);}

RFunction operator +  (const RFunction& f, const RFunction& g)
  { return RFunction(new function_algebra_addition(f,g));}

RFunction operator - (const RFunction& f, const RFunction& g)
  { return RFunction(new function_algebra_subtraction(f,g));}

RFunction Piecewise(const RFunction& F0, const RFunction& Fneg, const RFunction& Fpos)
{return RFunction(new function_algebra_piecewise(F0,Fneg,Fpos));}

} // namespace GrAL 
