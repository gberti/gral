#ifndef NMWR_GB_TYPE_NAME_TRAITS_H
#define NMWR_GB_TYPE_NAME_TRAITS_H


// $LICENSE


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
template<>
struct type_name_traits<bool> {
  static const char* name() { return "bool";}
};


template<>
struct type_name_traits<int> {
  static const char* name() { return "int";}
};

template<>
struct type_name_traits<unsigned> {
  static const char* name() { return "unsigned";}
};

template<>
struct type_name_traits<short> {
  static const char* name() { return "short";}
};

template<>
struct type_name_traits<long> {
  static const char* name() { return "long";}
};

template<>
struct type_name_traits<char> {
  static const char* name() { return "char";}
};


template<>
struct type_name_traits<float> {
  static const char* name() { return "float";}
};

template<>
struct type_name_traits<double> {
  static const char* name() { return "double";}
};

template<>
struct type_name_traits<std::string> {
  static const char* name() { return "string";}
};

//@}
#endif
