#ifndef NMWR_GB_WRITE_CODE_H
#define NMWR_GB_WRITE_CODE_H

#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include <string>


inline std::string makename(std::string const& base, int i)
{
#ifdef GRAL_HAS_SSTREAM
      std::ostringstream s; 
#else
      std::ostrstream s; 
#endif
  s << base << i; 
  std::string r(s.str());
  return(r);
}

// FIXME: works only for Geometry::point
template<class P>
inline P makepoint(int dim, std::string const& coords)
{
#ifdef GRAL_HAS_SSTREAM
      std::ostringstream s; 
#else
      std::ostrstream s; 
#endif
 s  << coords;
 P p(dim, point::no_init);
 s >> p;
 return p;
}

inline std::string write_code_eval_libfct(std::ostream     & out, 
                                          int              & vnum, 
                                          std::string const& var,
                                          std::string const& fct)
{
    std::string r(makename(fct,vnum++)); 
    out << "double " << r << "(  " << fct << "(" << var << "));\n ";
    return r;
} 

inline std::string write_code_derive_libfct(std::ostream     & out, 
					    int              & vnum, 
					    std::string const& varx,
					    std::string const& varh,
					    std::string const& d_fct)
{
    std::string r(makename(d_fct,vnum++)); 
    out << "double " << r << "(  " << d_fct << "(" << varx << ") *  " << varh << ");\n ";
    return r;
} 



#endif
