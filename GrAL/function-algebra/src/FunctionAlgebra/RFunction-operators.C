#include <strstream.h>

#include "RFunction/RFunction.h"
#include "RFunction/write_code.h"
#include "Utility/pre-post-conditions.h"

//////////////////////////////////////////////////////////////
//
// concrete classes implementing the operations on RFunctions
//
//////////////////////////////////////////////////////////////


class fct_composition : public fct {
private:
  RFunction F1,F2; // we could use fct* here, but then we would have to
                  // reimplement the handle-mechanism of handle-base
                  // in a sort of binary version (in contrast to the 
                  // unary vers. provided by handle_base

protected:
  virtual point derive(const point& x, const point& h) const
  {return (F1.derive(F2(x),F2.derive(x,h)) );}
public:
  fct_composition(const RFunction& Fa, const RFunction& Fb) // fa o fb
    : fct(), F1(Fa), F2(Fb)
  {
    d_Def = Fb.dDef(); 
    d_Im  = Fa.dIm(); 
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  } 
  virtual ~fct_composition() {} 
  virtual string  name() const {return "fct_composition";}
  point eval(const point& x) const { return F1(F2(x));}
  virtual string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;

};


class fct_combination : public fct {
private:
  RFunction F1,F2;
  point derive(const point& x, const point& h) const 
  { return combine(F1.derive(x,h),F2.derive(x,h));}
public:
  fct_combination(const RFunction& Fa, const RFunction& Fb) // (fa,fb)
    : fct(), F1(Fa), F2(Fb) 
  {
    d_Def = Fa.dDef();
    d_Im  = Fa.dIm() + Fb.dIm();
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  } 
  virtual ~fct_combination() {}
  virtual string name() const {return "fct_combination";}
  point eval(const point& x) const {return combine(F1(x),F2(x));}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

class fct_multiplication : public fct {
private:
  RFunction F1,F2;
  point derive(const point& x, const point& h) const 
  { return (F1.derive(x,h)*F2(x) + F1(x)*F2.derive(x,h));}
public:
  fct_multiplication(const RFunction& Fa, const RFunction& Fb) : fct(), F1(Fa), F2(Fb) 
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

  virtual ~fct_multiplication() {} 
  virtual string name() const {return "fct_multiplication";}
  point eval(const point& x) const 
  {
    if(F1.dIm() == 1)
      return (F1(x)[1]*F2(x)); // operator * (double, point)
    if(F2.dIm() == 1)
      return (F1(x)*F2(x)[1]); // operator * (point, double)
    else
      return (F1(x)*F2(x));    // operator * (point, point)
  }

  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

class fct_division : public fct { // F1 / F2
private:
  RFunction F1,F2;
  point derive(const point& x, const point& h) const 
  { point  F2x(F2(x)[1]);  return ((F1.derive(x,h)*F2x - F2.derive(x,h)*F1(x))/ (F2x*F2x));}
public:
  fct_division(const RFunction& Fa, const RFunction& Fb) : fct(), F1(Fa), F2(Fb) { 
    REQUIRE((Fb.dIm() == 1 ), "fct: Fa/Fb : dim Im(Fb) = " << Fb.dIm() << " != 1!\n",1);
    d_Def = Fa.dDef();d_Im = Fa.dIm();
  }
  virtual ~fct_division() {} 
  virtual string name() const {return "fct_division";}

  point eval(const point& x) const {point F1x(F1(x)); double F2x = F2(x)[1];
                                    return (F1x/F2x);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

class fct_div : public fct { // 1 / F
private:
  RFunction F;
  point derive(const point& x, const point& h) const 
  { point Fx(F(x)[1]);  return (- d(F,x)(h) / (Fx*Fx));}
public:
  fct_div(const RFunction& Fa) : fct(), F(Fa) {
    REQUIRE((F.dIm() == 1 ),"fct: 1/F : dim Im(F) = " <<  F.dIm() << " > 1!",1);
    d_Def = F.dDef();d_Im = F.dIm();
  }
  virtual ~fct_div() {}
  virtual string name() const {return "fct_inverse";}

  point eval(const point& x) const {return (1.0/(F(x))[1]);}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};


class fct_addition : public fct {
private:
  RFunction F1,F2;
  point derive(const point& x, const point& h) const 
  { return (F1.derive(x,h)+F2.derive(x,h));}
public:
  fct_addition(const RFunction& Fa, const RFunction& Fb)
    : fct(), F1(Fa), F2(Fb) 
  {
    d_Def = Fa.dDef();
    d_Im = Fa.dIm();
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  }
  virtual ~fct_addition() {}
  virtual string name() const {return "fct_addition";}
  point eval(const point& x) const {return (F1(x)+F2(x));}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;

};

class fct_subtraction : public fct {
private:
  RFunction F1,F2;
  point derive(const point& x, const point& h) const 
  { return (F1.derive(x,h)-F2.derive(x,h));}
public:
  fct_subtraction(const RFunction& Fa, const RFunction& Fb) 
    : fct(), F1(Fa), F2(Fb) 
  {
    d_Def = Fa.dDef();
    d_Im = Fa.dIm();
    is_affine = (Fa.IsAffine() && Fb.IsAffine());
  }
  virtual ~fct_subtraction() {}
  virtual string name() const {return "fct_subtraction";}
  point eval(const point& x) const {return (F1(x)-F2(x));}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

class fct_piecewise: public fct {
private:
  RFunction F0,Fneg,Fpos;
  point derive(const point& x, const point& h) const 
  { return(F0(x)[1] > 0 ? d(Fpos,x)(h) : d(Fneg,x)(h));}
public:
  fct_piecewise(const RFunction& f0,const RFunction& fneg,const RFunction& fpos);
  point eval(const point& x) const { return(F0(x)[1] > 0 ? Fpos(x) : Fneg(x));}
  ~fct_piecewise() {}
  virtual string name() const {return "fct_piecewise";}
  string write_code_eval(ostream& out, int& vnum, const string& var) const;
  virtual string write_code_derive(ostream& out, int& vnum, 
				 const string& varx, const string& varh) const;
};

inline  fct_piecewise::fct_piecewise(const RFunction& f0,
				     const RFunction& fneg,const RFunction& fpos) :
  fct(), F0(f0), Fneg(fneg), Fpos(fpos) 
{
  REQUIRE(((F0.dIm() == 1) && (Fneg.dIm() == Fpos.dIm()) && (Fneg.dDef() == Fpos.dDef())),
	  "fct_piecewise: dimensions do not correspond!\n",1);
  d_Def=Fneg.dDef();d_Im=Fneg.dIm();
}






//////////////////////////////////////////////////////////////////////
//
//      implementation of output-methods
//


static string write_code_eval_bin_op(const RFunction& F1, const RFunction F2,
				ostream& out, int& vnum, string varbasename, char op,
				const string& var, int dIm)
{
  string r1( F1.write_code_eval(out,vnum,var));
  string r2( F2.write_code_eval(out,vnum,var));
  string r3(makename(varbasename,vnum++));
  if(dIm == 1)
    out << "double ";
  else
    out << "point ";
  out << r3 << "( " << r1 << " " << op << " " << r2 << " );\n"; 
 return r3;
}




string fct_div::write_code_eval(ostream& out, int& vnum, const string& var) const
{
 string r1( F.write_code_eval(out,vnum,var));
 string r(makename("inv",vnum++));
 out << "double " << r << "(1.0/(" << r1 << "));\n";
 return r;
}

string fct_div::write_code_derive(ostream& out, int& vnum, const string& varx, const string& varh) const
{
 string df( F.write_code_derive(out,vnum,varx,varh));
 string f( F.write_code_eval(out,vnum,varx));
 string r(makename("d_inv",vnum++));
 out << "double " << r << "(-" << df << "/(sqr(" << f << ")));\n";
 return r;
}

string fct_multiplication::write_code_eval(ostream& out, int& vnum, const string& var) const
{ return write_code_eval_bin_op(F1,F2,out,vnum,"prod",'*',var,dIm());}

string fct_multiplication::write_code_derive(ostream& out, int& vnum, 
					   const string& varx, const string& varh) const
{
 string f1( F1.write_code_eval(out,vnum,varx));
 string df1( F1.write_code_derive(out,vnum,varx,varh));
 string f2( F2.write_code_eval(out,vnum,varx));
 string df2( F2.write_code_derive(out,vnum,varx,varh));
 string rtype = ( dIm() == 1? "double" : "point");
 string r(makename("d_mult",vnum++));
 out << rtype << " " << r << "(" << f1 << "*" << df2 << " + " << df1 << "*" << f2 << ");\n";
 return r;
}

string fct_division::write_code_eval(ostream& out, int& vnum, const string& var) const
{ return write_code_eval_bin_op(F1,F2,out,vnum,"quot",'/',var,dIm());}

string fct_division::write_code_derive(ostream& out, int& vnum, 
				     const string& varx, const string& varh) const
{
 string f1( F1.write_code_eval(out,vnum,varx));
 string df1( F1.write_code_derive(out,vnum,varx,varh));
 string f2( F2.write_code_eval(out,vnum,varx));
 string df2( F2.write_code_derive(out,vnum,varx,varh));
 string rtype = ( dIm() == 1? "double" : "point");
 string r(makename("d_div",vnum++));
 out << rtype << " " << r << "((-" << f1 << "*" << df2 
     << " + " << df1 << "*" << f2 << ")/sqr(" << f2 << "));\n";
 return r;
}


string fct_addition::write_code_eval(ostream& out, int& vnum, const string& var) const 
{ return write_code_eval_bin_op(F1,F2,out,vnum,"sum",'+',var,dIm());}

string fct_addition::write_code_derive(ostream& out, int& vnum, 
				     const string& varx, const string& varh) const
{
 string df1( F1.write_code_derive(out,vnum,varx,varh));
 string df2( F2.write_code_derive(out,vnum,varx,varh));
 string rtype = ( dIm() == 1? "double" : "point");
 string r(makename("d_add",vnum++));
 out << rtype << " " << r << "(" << df1 << " + " << df2 << ");\n";
 return r;
}


string fct_subtraction::write_code_eval(ostream& out, int& vnum, const string& var) const
{ return write_code_eval_bin_op(F1,F2,out,vnum,"diff",'-',var,dIm());}

string fct_subtraction::write_code_derive(ostream& out, int& vnum, 
				     const string& varx, const string& varh) const
{
 string df1( F1.write_code_derive(out,vnum,varx,varh));
 string df2( F2.write_code_derive(out,vnum,varx,varh));
 string rtype = ( dIm() == 1? "double" : "point");
 string r(makename("d_sub",vnum++));
 out << rtype << " " << r << "(" << df1 << " - " << df2 << ");\n";
 return r;
}



string fct_piecewise::write_code_eval(ostream& out, int& vnum, const string& var) const
{
 string r0(     F0.write_code_eval(out,vnum,var));
 string rneg( Fneg.write_code_eval(out,vnum,var));
 string rpos( Fpos.write_code_eval(out,vnum,var));
 string r(makename("pw",vnum++));
 string rtype( (dIm() == 1? "double" : "point"));
 out << rtype << " " << r << "( (" << r0 << " < 0.0 ? " << rneg << " : " << rpos << "));\n"; 
 return r;
}


string fct_piecewise::write_code_derive(ostream& out, int& vnum, 
				       const string& varx, const string& varh) const
{
 string r0(     F0.write_code_eval(out,vnum,varx));
 string drneg( Fneg.write_code_derive(out,vnum,varx,varh));
 string drpos( Fpos.write_code_derive(out,vnum,varx,varh));
 string r(makename("d_pw",vnum++));
 string rtype( (dIm() == 1? "double" : "point"));
 out << rtype << " " << r << "( (" << r0 << " < 0.0 ? " << drneg << " : " << drpos << "));\n"; 
 return r;
}

string fct_composition::write_code_eval(ostream& out, int& vnum, const string& var) const
{ return F1.write_code_eval(out,vnum, F2.write_code_eval(out,vnum,var));}

string fct_composition::write_code_derive(ostream& out, int& vnum, 
					const string& varx, const string& varh) const
{
 string df2(F2.write_code_derive(out,vnum,varx,varh));
 return F1.write_code_derive(out,vnum,F2.write_code_eval(out,vnum,varx),df2);
}


string fct_combination::write_code_eval(ostream& out, int& vnum, const string& var) const
{
  string r1( F1.write_code_eval(out,vnum,var));
  string r2( F2.write_code_eval(out,vnum,var));
  string r3(makename("comb",vnum++));
  string rtype = ( dIm() == 1? "double" : "point");
  out << rtype << " " << r3 << "( (point(" << r1 << ") , point(" << r2 << ")) );\n"; 
  return r3;
}


string fct_combination::write_code_derive(ostream& out, int& vnum,
					 const string& varx, const string& varh) const
{
  string dr1( F1.write_code_derive(out,vnum,varx,varh));
  string dr2( F2.write_code_derive(out,vnum,varx,varh));
  string r3(makename("d_comb",vnum++));
  string rtype = ( dIm() == 1? "double" : "point");
  out << rtype << " " << r3 << "( (point(" << dr1 << ") , point(" << dr2 << ")) );\n"; 
  return r3;
}




// operators on RFunctions, returning appropriate subclasses
// of class RFunction implementing the operation 

class fct_null: public fct {
private:
  point derive(const point& /*x*/, const point& /*h*/) const {return point(d_Im,zero_init);}
public:
  fct_null(int dD=1, int dI=1) {d_Def= dD;d_Im=dI;}
  ~fct_null() {}
  virtual string name() const { return "fct_null";} 
  point eval(const point& /*x*/) const { return point(d_Im,zero_init);}
  string write_code_eval(ostream&,int&, const string&) const
  { if (d_Im == 1)
    return( "0.0");
  else {
    ostrstream s; 
    s << "point(" << d_Im << ",zero_init)";
    return s.str();
  }
  }
  string write_code_derive(ostream&,int& , const string&, const string&) const
  { if (d_Im == 1)
    return( "0.0");
  else {
    ostrstream s; 
    s << "point(" << d_Im << ",zero_init)";
    return s.str();
  }
  } 
};

RFunction NullFct(int dD,int dI) { return RFunction(new fct_null(dD,dI));}


RFunction::RFunction()  : my_handle_base(new fct_null(0,0)), a(1.0), b(0)  {}
  
RFunction RFunction::operator() (const RFunction& g) const
  { return RFunction(new fct_composition(*this,g));}

/*
RFunction operator ,  (const RFunction& f, const RFunction& g)
{ return RFunction(new fct_combination(f,g));}
*/

RFunction combine(const RFunction& f, const RFunction& g)
{ return RFunction(new fct_combination(f,g));}


RFunction operator *  (const RFunction& f, const RFunction& g)
  { RFunction ff(new fct_multiplication(f,g));
  return ff;}

RFunction operator * (const point& p, const RFunction& f)
{ return (f*Constant(p,f.dDef()));}

RFunction operator * (const RFunction& f, const point& p)
{ return (f*Constant(p,f.dDef()));}

RFunction operator /  (const RFunction& f, const RFunction& g) // f/g
{ return RFunction(new fct_division(f,g));}

RFunction operator /  (const double& a, const RFunction& g) // a/g
{   RFunction f(new fct_div(g));  return (f*a);}

RFunction operator +  (const RFunction& f, const RFunction& g)
  { return RFunction(new fct_addition(f,g));}

RFunction operator - (const RFunction& f, const RFunction& g)
  { return RFunction(new fct_subtraction(f,g));}

RFunction Piecewise(const RFunction& F0, const RFunction& Fneg, const RFunction& Fpos)
{return RFunction(new fct_piecewise(F0,Fneg,Fpos));}



void RFunction::write_class(ostream& out, const string& name) const
{
 string varx("x");
 string varh("h");
 out << "////////////////////////////////////////////////\n"
     << "//\n"
     << "// Code automatically generated by class RFunction\n"
     << "//\n"
     << "////////////////////////////////////////////////\n\n\n"

     << "#include " << '"' << "RFunction.h" << '"' << "\n\n\n"
     << "#include " << '"' << "write_code.h" << '"' << "\n"

     << "class fct_" << name << " : public fct {\n"
     << "private:\n"
     << "  point derive(const point& x, const point& h) const;\n"
     << "public:\n"
     << "  fct_" << name << "() {d_Def = " << dDef() << "; d_Im = " << dIm() <<";}\n"
     << "  string name() const { return " << '"' << "fct_" << name << '"' << ";}\n"
     << "  point eval(const point& x) const;\n"
     << "  string write_code_eval(ostream& out, int& vnum, const string& var) const\n"
     << "  {return write_code_eval_libfct(out,vnum,var," << '"' << name << '"' << ");}\n"
     << "  string write_code_derive(ostream& out, int& vnum,\n"
     << "                          const string& varx, const string& varh) const\n"
     << "  {return write_code_derive_libfct(out,vnum,varx,varh," 
     << '"' << "d_" << name << '"' << ");}\n"
     << "};\n\n"
     << "inline point " << "fct_" << name << "::eval(const point& " << varx << ") const\n"
     << "{\n";
 int vnum = 0;
 string result(write_code_eval(out,vnum,varx));
 out << "\n return(" << result << ");\n}\n\n";

 out << "inline point " << "fct_" << name << "::derive(const point& " << varx 
     << ", const point& " << varh << ") const\n"
     << "{\n";
 vnum = 0;
 string d_result(write_code_derive(out,vnum,varx,varh));
 out << "\n return(" << d_result << ");\n}\n\n";
}

string RFunction::write_code_eval(ostream& out, int& vnum, const string& var) const
{
  string r(f()->write_code_eval(out,vnum,var));
  if(a!= 1.0 || ! IsNullvector(b)){
    string rr(makename("tmp",vnum++));
    string rtype( (dIm() == 1? "double" : "point"));
    out << rtype << " " << rr << "(" << r << ");\n";
    if (a!= 1.0)
      out << rr << " *= " << a << ";\n";
    for (int i =1;i<= b.dim();i++)
      if( b[i] != 0)
	if(dIm() == 1)
	  out << rr << " += " << b[i] << ";\n";
	else
	  out << rr << "[" << i << "] += " << b[i] << ";\n";
    return rr;
  }
  else
    return  r;
}


string RFunction::write_code_derive(ostream& out, int& vnum, 
				   const string& varx, const string& varh) const
{
  string r(f()->write_code_derive(out,vnum,varx,varh));
  if(a != 1.0){
    string rr(makename("tmp",vnum++));
    string rtype( (dIm() == 1? "double" : "point"));
    out << rtype << " " << rr << "(" << r << ");\n";
    out << rr << " *= " << a << ";\n";
    return rr;
  }
  return r;
}
