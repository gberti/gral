#ifndef NMWR_GB_COMPILER_CONFIG_H
#define NMWR_GB_COMPILER_CONFIG_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// compiler specific adaptations
//
//----------------------------------------------------------------

#if defined(__GNUC__) || defined (__KCC)
/* #define MAKE_DEFAULT_OPS_SELF     */
/* #define MAKE_DEFAULT_IO_OPS_SELF  */
/* #define MAKE_REL_OPS_SELF */
/* #define MAKE_LESS_OP_SELF */
/* #define MAKE_EQUAL_OP_SELF */
/* #define DYNAMIC_CAST(type,parm) dynamic_cast<type>(parm) */
/* #define STATIC_CAST(type,parm)  static_cast<type>(parm) */
/* #define TF_INLINE */
#ifndef __KCC 
/* #define USING_NS_STD   */
/* #define USING_STD(x)   */
/* #include "stl_config.h" */

#else /* KCC */
//#define __STL_EXPLICIT_FUNCTION_TMPL_ARGS  1
//#define __STL_CLASS_PARTIAL_SPECIALIZATION 1
//#define KAI_NONSTD_FUNCTIONAL 1
// #include "KCC/stl_config.h"
//#define USING_NS_STD  using namespace std
//#define USING_STD(x)  using std:: x
//#include <algobase.h>
//using namespace std;
//using namespace std::rel_ops;
#endif

#elif defined(__xlC__)
#include "stl_config.h"
#include "vector.h"

#include "Utility/pre-post-conditions.h"

// inline decl. for friend of template classes
// this seems to be disallowed by standard
#define TF_INLINE inline

// xlC needs operators even if not used

#define MAKE_DEFAULT_OPS_SELF \
friend  bool operator== (self const&, self const&)\
  { ENSURE_ALWAYS(false,"dummy operator== called!\n",1);\
    return false;\
  }\
friend  bool operator< (self const&, self const&)\
  { ENSURE_ALWAYS(false,"dummy operator< called!\n",1);\
    return false;\
  }\
friend ostream& operator<<(ostream& o,self const&)\
  { ENSURE_ALWAYS(false,"dummy output operator called!\n",1);\
    return o;\
  }\
friend istream& operator>>(istream& i,self const&)\
  { ENSURE_ALWAYS(false,"dummy input operator called!\n",1);\
    return i;\
  }


#define MAKE_REL_OPS_SELF \
friend  bool operator== (self const&, self const&)\
  { ENSURE_ALWAYS(false,"dummy operator== called!\n",1);\
    return false;\
  }\
friend  bool operator< (self const&, self const&)\
  { ENSURE_ALWAYS(false,"dummy operator< called!\n",1);\
    return false;\
  }


#define MAKE_LESS_OP_SELF \
friend  bool operator< (self const&, self const&)\
  { ENSURE_ALWAYS(false,"dummy operator< called!\n",1);\
    return false;\
  }

#define MAKE_EQUAL_OP_SELF \
friend  bool operator< (self const&, self const&)\
  { ENSURE_ALWAYS(false,"dummy operator< called!\n",1);\
    return false;\
  }


#define MAKE_DEFAULT_IO_OPS_SELF \
friend ostream& operator<<(ostream& o,self const&)\
  { ENSURE_ALWAYS(false,"dummy output operator called!\n",1);\
    return o;\
  }\
friend istream& operator>>(istream& i,self const&)\
  { ENSURE_ALWAYS(false,"dummy input operator called!\n",1);\
    return i;\
  }

template<class T>
ostream& operator<<(ostream& o,vector<T> const&)
  { ENSURE_ALWAYS(false,"dummy output operator called!\n",1);
    return o;
  }

template<class T>
istream& operator>>(istream& i,vector<T> const&)
  { ENSURE_ALWAYS(false,"dummy input operator called!\n",1);
    return i;
  }
template<class T,class U>
ostream& operator<<(ostream& o,pair<T,U> const&)
  { ENSURE_ALWAYS(false,"dummy output operator called!\n",1);
    return o;
  }

template<class T,class U>
istream& operator>>(istream& i,pair<T,U> const&)
  { ENSURE_ALWAYS(false,"dummy input operator called!\n",1);
    return i;
  }

#define DYNAMIC_CAST(type,parm) ((type)parm)
#define STATIC_CAST(type,parm)  ((type)parm)

#else
// #include "stl_config.h"

#endif


#endif
