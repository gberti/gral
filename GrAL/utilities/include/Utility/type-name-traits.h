#ifndef NMWR_GB_TYPE_NAME_TRAITS_H
#define NMWR_GB_TYPE_NAME_TRAITS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Forward/string_fwd.h"

//----------------------------------------------------------------

/*! \file

 \brief Deduce a string with the type name from the type.

 This allows a generic implementation of some routines 
 where the typename must be written out.

 This is specialized for basic types, and must be
 specialized for other types if needed.

*///--------------------------------------------------------------

//@{
/*!   \brief The general case: Name not known, return empty string. */
template<class T>
struct type_name_traits {
  static const char* name() { return "";}
};

/*! \brief specialization for bool */
struct type_name_traits<bool> {
  static const char* name() { return "bool";}
};


struct type_name_traits<int> {
  static const char* name() { return "int";}
};

struct type_name_traits<unsigned> {
  static const char* name() { return "unsigned";}
};

struct type_name_traits<short> {
  static const char* name() { return "short";}
};

struct type_name_traits<long> {
  static const char* name() { return "long";}
};

struct type_name_traits<char> {
  static const char* name() { return "char";}
};


struct type_name_traits<float> {
  static const char* name() { return "float";}
};

struct type_name_traits<double> {
  static const char* name() { return "double";}
};

struct type_name_traits<string> {
  static const char* name() { return "string";}
};

//@}
#endif
