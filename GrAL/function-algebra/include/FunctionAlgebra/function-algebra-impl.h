#ifndef NMWR_GB_FUNCTIONALGEBRA_IMPL_H
#define NMWR_GB_FUNCTIONALGEBRA_IMPL_H

// $LICENSE

#include <string>

#include "Geometry/algebraic-primitives.h"
#include "Geometry/point.h"

#include "Utility/copy-traits.h"

/*! Abstract base class defining the behavior of a RFunction
    
*/
class function_algebra_impl {
private:

  // FORBIDDEN
  //  function_algebra_impl(function_algebra_impl const&);
  //  function_algebra_impl& operator=(function_algebra_impl const& );
protected:
  unsigned d_Im,d_Def;
  bool     is_affine;

  //  void vname(ostream& out,int i);
  enum { ANYDIM = 0};
public:
  typedef point                            coord_type;
  typedef point_traits        <coord_type> pt;
  typedef algebraic_primitives<coord_type> ap;

  function_algebra_impl() : is_affine(false) {}
  virtual ~function_algebra_impl() {}
  virtual function_algebra_impl* clone() const = 0;

  virtual coord_type derive(const coord_type& x, const coord_type& h) const = 0;
  virtual coord_type eval  (const coord_type& x)                 const = 0;

  unsigned  dIm()      const  {return d_Im;}
  unsigned  dDef()     const  {return d_Def;}
  bool      IsAffine() const  {return (is_affine);}

  virtual std::string name() const = 0;
  virtual std::string write_code_eval  (std::ostream& out, int& /*vnum*/, 
                                        std::string const& /*var*/) const; 
  virtual std::string write_code_derive(std::ostream& out, int& /*vnum*/, 
                                        std::string const& /*varx*/ , 
                                        std::string const& /* varh */) const;
};


template<>
struct copy_traits<function_algebra_impl>
    : public copy_traits_base<function_algebra_impl>
{
  static function_algebra_impl* clone(function_algebra_impl const& f)
  { return f.clone();}
};

#endif
