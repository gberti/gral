#include "FunctionAlgebra/function-algebra-impl.h"

std::string 
function_algebra_impl::write_code_eval(std::ostream     & out, 
                                       int              & /*vnum*/, 
                                       std::string const& /*var*/) const 
{  
  out << name() 
      << "::write_code_eval: not implemented for this derived class!\n"; 
  return "??";
}

std::string 
function_algebra_impl::write_code_derive(std::ostream     & out, 
                                         int              & /*vnum*/, 
                                         std::string const& /*varx*/ , 
                                         std::string const& /* varh */) const 
{
  out << name() 
      << "::write_code_deriv: not implemented for this derived class!\n"; 
  return "d??";
}
