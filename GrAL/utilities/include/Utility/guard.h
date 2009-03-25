#ifndef GRAL_GB_UTILITY_GUARD_H
#define GRAL_GB_UTILITY_GUARD_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <istream>
#include <ostream>

namespace GrAL {

  /*! \brief Wrapper for a value class to check if it has been set/initialised

      \ingroup memory

      \b Example
      \code
      guard<int> var;

      assert(! var.is_set());
      flag = 1;
      assert(  var.is_set());

      \endcode
   */
  template<class T>
  class guard {
    typedef guard<T> self;
    T    m_t;
    bool m_is_set;
  public:
    /*! \brief Not initialised
      \post <tt> is_set() == false </tt>
    */
    guard() : m_is_set(false) {}
    /*! \brief Initialised if <tt> declare_as_set == true </tt>
        \post <tt> is_set() == declare_as_set && value == t </tt>
     */
    guard(T t, bool declare_as_set = true) : m_t(t), m_is_set(declare_as_set) {}
    /*! \brief Initialise.
        \post <tt> is_set() == true && value() == t </tt>
     */
    self& operator=(T const& t) { m_t = t; m_is_set = true; return *this;}
    
    //! \brief Get value (implicit conversion)
    operator T() const { return m_t;}
    //! \brief  Get value
    T const& value() const { return m_t;}

    void read (std::istream& in)        { in >> m_t; m_is_set = true;}
    void print(std::ostream& out) const { out << m_t;}

    //! \brief Query initialisation status
    bool is_set() const { return m_is_set;}
    //! \brief Force <tt> is_set() == true; </tt>
    void   set()  { m_is_set = true;}
    //! \brief Force <tt> is_set() == false; </tt>
    void unset()  { m_is_set = false;}
  };
  
  /*!
     \relates guard 
   */
  template<class T>
  inline
  std::istream& operator>>(std::istream& in,  guard<T>& g) { g.read(in); return in;}

  /*! 
    \relates guard
  */
  template<class T>
  inline
  std::ostream& operator<<(std::ostream& out, guard<T> const& g) { g.print(out); return out;}
  
  
} // namespace GrAL


#endif
