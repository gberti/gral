#ifndef GRAL_RP_EXTERNAL_CONTROL_CHECKER_BASE_H
#define GRAL_RP_EXTERNAL_CONTROL_CHECKER_BASE_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
    \author: Roman Putanowicz
*/


namespace GrAL {



  /*! Interface to checker objects that can be installed in mutators
      in order to check value of input parameters.
   */

  struct checker_impl {
    virtual bool check() const { return true; }  
    virtual ~checker_impl() {}
  };

  /*! \brief Checker class (envelope)

  */
  class Checker {
  private:
    boost::shared_ptr<checker_impl> impl;
  public:
    Checker() {}
    Checker(boost::shared_ptr<checker_impl> p) : impl(p) {}
    bool check() const { return( impl ? impl->check() : true);}
  };

} // namespace GrAL 

#endif
