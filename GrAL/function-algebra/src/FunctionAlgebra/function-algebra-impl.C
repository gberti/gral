
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "FunctionAlgebra/function-algebra-impl.h"

namespace GrAL {

std::string 
function_algebra_impl::write_code_eval(::std::ostream     & out, 
                                       int              & /*vnum*/, 
                                       ::std::string const& /*var*/) const 
{  
  out << name() 
      << "::write_code_eval: not implemented for this derived class!\n"; 
  return "??";
}

std::string 
function_algebra_impl::write_code_derive(::std::ostream     & out, 
                                         int              & /*vnum*/, 
                                         ::std::string const& /*varx*/ , 
                                         ::std::string const& /* varh */) const 
{
  out << name() 
      << "::write_code_deriv: not implemented for this derived class!\n"; 
  return "d??";
}

} // namespace GrAL 
