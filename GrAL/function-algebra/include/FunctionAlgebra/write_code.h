#ifndef NMWR_GB_WRITE_CODE_H
#define NMWR_GB_WRITE_CODE_H

#include <strstream.h>

static inline string makename(const string& base, int i)
{
  ostrstream s; 
  s << base << i; 
  string r(s.str());
  return(r);
}

inline string write_code_eval_libfct(ostream& out, int& vnum, const string& var, const string& fct)
{
    string r(makename(fct,vnum++)); 
    out << "double " << r << "(  " << fct << "(" << var << "));\n ";
    return r;
  } 

inline string write_code_derive_libfct(ostream& out, int& vnum, const string& varx,const string& varh,
				       const string& d_fct)
{
    string r(makename(d_fct,vnum++)); 
    out << "double " << r << "(  " << d_fct << "(" << varx << ") *  " << varh << ");\n ";
    return r;
  } 



#endif
