#ifndef NMWR_GB_FCT_H
#define NMWR_GB_FCT_H

#include <strstream.h>
#include "mystring.h"

#include "RFunction/my_handle_types.h"
#include "LinAlg/point.h"


const int ANYDIM = -1;

// an abstract base class defining the behavior of a RFunction

class fct : public my_handle_rep{
private:
  fct(const fct&) {}
  fct& operator=(const fct& ) {return *this;}
protected:
  int d_Im,d_Def;
  bool is_affine;
  void vname(ostream& out,int i);
public:
  fct() : is_affine(false) {}
  virtual ~fct() {}
  virtual point derive(const point& x, const point& h) const = 0;
  virtual point eval(const point&) const = 0;
  int dIm() const {return d_Im;}
  int dDef() const  {return d_Def;}
  bool IsAffine() const {return (is_affine);}
  virtual string name() const = 0;
  virtual string write_code_eval(ostream& out, int& /*vnum*/, const string& /*var*/) const 
  {out << name() << "::write_code_eval: not implemented for this derived class!\n"; return "??";}
  virtual string write_code_derive(ostream& out, int& /*vnum*/, 
				  const string& /*varx*/ , const string& /* varh */) const 
  {out << name() << "::write_code_deriv: not implemented for this derived class!\n"; return "d??";}
};

#endif
