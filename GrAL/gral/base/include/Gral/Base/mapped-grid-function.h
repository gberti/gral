#ifndef GRAL_GB_BASE_MAPPED_GRID_FUNCTION_H
#define GRAL_GB_BASE_MAPPED_GRID_FUNCTION_H

// $LICENSE_NEC_2005

#include "Gral/Base/common-grid-basics.h"
#include "Utility/ref-ptr.h"
#include "Container/mapped-value-iterator.h"


namespace GrAL {
  
  /*! \brief View to a value-mapped grid function
   
      \ingroup gridfunctions
      
      Model of $GrAL TotalGridFunction or $GrAL PartialGridFunction,
      depending on \c GF.
     
      \see Tested in \ref test-mapped-grid-function.C
  */
  template<class GF, class F>
  class mapped_grid_function {
    typedef GF base_grid_function;
    typedef typename GF::const_iterator       base_const_iterator;
    
  public:
    typedef F                                 mapping_type;
    typedef typename F::result_type           value_type;
    typedef value_type&                       reference;
    typedef value_type                        const_reference; // will return by value
    typedef typename GF::element_type         element_type;
    typedef typename GF::size_type            size_type;
    typedef typename GF::grid_type            grid_type;
    typedef mapped_value_const_iterator<base_const_iterator, mapping_type> 
                                              const_iterator;

    // STL unary function
    typedef value_type   result_type;
    typedef element_type argument_type;
  private:
    ref_ptr<base_grid_function> gf;
    ref_ptr<mapping_type>        f;
  public:
    mapped_grid_function() {}
    mapped_grid_function(base_grid_function & gf1, mapping_type & f1)
      : gf(gf1), f(f1) {}
    mapped_grid_function(ref_ptr<base_grid_function> gf1, 
			 ref_ptr<mapping_type>       f1)
      : gf(gf1), f(f1) {}
    
    void init(base_grid_function & gf1, mapping_type & f1)
    {
      gf = ref_ptr<base_grid_function>(gf1);
      f  = ref_ptr<mapping_type>      (f);
    }
    void init(ref_ptr<base_grid_function> gf1, 
	      ref_ptr<mapping_type>       f1)
    { 
      gf = gf1;
      f  = f1;
    }

    value_type operator()(element_type const& e) const { return (*f)((*gf)(e));}

    size_type      size()  const { return gf->size();}
    const_iterator begin() const { return const_iterator(gf->begin(), f);}
    const_iterator end  () const { return const_iterator(gf->end  (), f);}

    // Only if GF is a partial grid function
    bool       defined(element_type const& e) const { return gf->defined(e);}

    grid_type    const& TheGrid()    const { return gf->TheGrid();}
    mapping_type const& TheMapping() const { return *f;}
  };

} // namespace GrAL 

#endif
