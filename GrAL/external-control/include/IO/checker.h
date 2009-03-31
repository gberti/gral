#ifndef GRAL_RP_EXTERNAL_CONTROL_CHECKER_H
#define GRAL_RP_EXTERNAL_CONTROL_CHECKER_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



/*! \file
    \author  Roman Putanowicz
 */

#include "IO/checker-base.h"
#include "Config/compiler-config.h"
#include "Utility/ref-ptr.h" // for null_deleter

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>



namespace GrAL {

//----------------------------------------------------------------
//
//  Some implementations of the Checker-interface
//
//----------------------------------------------------------------

/*! \brief  Range checking


    Checks if controled variable is within a given limits.
    Throws std:out_of_range exception of the limits are violated.

    \see checkercreators
 */
  template<class T>
  class limits_checker_impl : public checker_impl {
  public:
    limits_checker_impl(T& t, T l, T u, std::string const& n) 
      : controlee(&t, null_deleter()), lower(l), upper(u), name(n) {}
    
    virtual bool check() const {
      if (!(lower <= *controlee && *controlee <= upper)) {
	std::ostringstream s; 
	if (name != "") {
	  s << "Setting " << name << " to " << *controlee   
	    << " violates the bounds [" << lower<< "," << upper << "]";
	} else {
	  s << "Input parameter " << name << " " << *controlee  
	    << " violates the bounds [" << lower<< "," << upper << "]";
	}
	throw std::out_of_range(s.str());
	return false; 
      }  
      return true;
    }
  private:
    boost::shared_ptr<T> controlee; 
    T lower;
    T upper;
    std::string name;
};

/*! \brief  Upper limit checking 

    Checks if controled variable is less then or equal an upper limit.
    Throws std:out_of_range exception of the limit is violated.

    \see checkercreators
 */
  template<class T>
  class upper_limit_checker_impl : public checker_impl {
  public:
    upper_limit_checker_impl(T& t, T u, std::string const& n) 
      : controlee(&t, null_deleter()), upper(u), name(n) {}
    
    virtual bool check() const {
      if (!(*controlee<=upper)) {
	std::ostringstream s; 
	if (name != "") {
	  s << "Setting " << name << " to " << *controlee   
	    << " violates the upper limit <= " << upper;
	} else {
	  s << "Input parameter " << *controlee  
	    << " violates the upper limit <= " << upper;
	}
	throw std::out_of_range(s.str());
	return false;
      }  
      return true;
    }
  private:
    boost::shared_ptr<T> controlee; 
    T upper;
    std::string name;
  };

  /*! \brief  Lower limit checking 
    
      Checks if controled variable is greater then or equal a given limit.
      Throws std:out_of_range exception of the limit is violated.
  
      \see checkercreators
  */
  template<class T>
  class lower_limit_checker_impl : public checker_impl {
  public:
    lower_limit_checker_impl(T& t, T l, std::string const& n) 
      : controlee(&t, null_deleter()), lower(l), name(n) {}
    virtual bool check() const {
      if (!(lower <= *controlee)) {
	std::ostringstream s; 
	if (name != "") {
	  s << "Setting " << name << " to " << *controlee   
	    << " violates the lower limit >= " << lower;
	} else {
	  s << "Input parameter " << *controlee  
	    << " violates the lower limit >= " << lower;
	}
	throw std::out_of_range(s.str());
	return false;
      }  
      return true;
    }
  private:
    boost::shared_ptr<T>  controlee; 
    T lower;
    std::string name;
};

////////////////////////////////
// Checker-generating Functions
////////////////////////////////

/*! \defgroup checkercreators Creator functions for checkers
*/

  /*! \brief Creator function for UpperLimitChecker
    
    \ingroup checkercreators 
  */
  template<class T>
  inline Checker GetLimitsChecker(T& t, T lower, T upper, 
				  std::string const& name="") {
    return Checker(boost::shared_ptr<checker_impl>(new limits_checker_impl<T>(t, lower, upper, name)));
  }


  /*! \brief Creator function for UpperLimitChecker
    
    \ingroup checkercreators 
 */
  template<class T>
  inline Checker GetUpperLimitChecker(T& t, T upper, 
				      std::string const& name = "") { 
    return Checker(boost::shared_ptr<checker_impl>(new upper_limit_checker_impl<T>(t, upper, name)));
  }


  /*! \brief Creator function for LowerLimitChecker
    
    \ingroup checkercreators 
 */
  template<class T>
  inline Checker GetLowerLimitChecker(T& t, T lower, 
				      std::string const& name="") { 
    return Checker(boost::shared_ptr<checker_impl>(new lower_limit_checker_impl<T>(t, lower, name)));
  }
  
} // namespace GrAL 

#endif
