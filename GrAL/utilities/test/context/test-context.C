/*! \file

*/

#include "Utility/context.h"


int main() {

  typedef GrAL::io_context ctxt;

  ctxt::info()    << "Info."    << std::endl;
  ctxt::warning() << "Warning." << std::endl;
  ctxt::skip()    << "Null (ignored)."  << std::endl;
  ctxt::error()   << "Error."   << std::endl;
}
