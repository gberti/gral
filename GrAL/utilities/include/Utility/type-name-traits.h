#ifndef NMWR_GB_TYPE_NAME_TRAITS_H
#define NMWR_GB_TYPE_NAME_TRAITS_H


// $LICENSE


#include "Forward/string_fwd.h"

namespace GrAL { 

//----------------------------------------------------------------

/*! \file

 \brief Deduce a string with the type name from the type.

 This allows a generic implementation of some routines 
 where the typename must be written out.

 This is specialized for basic types, and must be
 specialized for other types if needed.

*///--------------------------------------------------------------

  /*! \defgroup typetraits Type traits
   */

  /*! \defgroup typetraitsspec Specializations of the GrAL::type_traits template
      \ingroup typetraits
      
       
   */


/*! \brief The general case: Name not known, return empty string. 

    Specialize this primary template for your class.
    \ingroup typetraits
    \see Module \ref typetraits
*/

template<class T>
struct type_name_traits {
  static const char* name() { return "";}
};

/*! \brief specialization for bool 

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<bool> {
  static const char* name() { return "bool";}
};


/*! \brief specialization for int

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<int> {
  static const char* name() { return "int";}
};

/*! \brief specialization for unsigned 

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<unsigned> {
  static const char* name() { return "unsigned";}
};

/*! \brief specialization for short

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<short> {
  static const char* name() { return "short";}
};

/*! \brief specialization for long

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<long> {
  static const char* name() { return "long";}
};

/*! \brief specialization for char 

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<char> {
  static const char* name() { return "char";}
};


/*! \brief specialization for float 

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<float> {
  static const char* name() { return "float";}
};

/*! \brief specialization for double

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<double> {
  static const char* name() { return "double";}
};

/*! \brief specialization for std::string

    \ingroup typetraitsspec
    \see Module \ref typetraitsspec
*/
template<>
struct type_name_traits<std::string> {
  static const char* name() { return "string";}
};


} // namespace GrAL 

#endif
