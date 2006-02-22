#ifndef GRAL_GB_SEQUENCE_SEQUENCE_IO_H
#define GRAL_GB_SEQUENCE_SEQUENCE_IO_H

// $LICENSE_NEC_2006

#include "Utility/ref-ptr.h"

#include <iostream>

namespace GrAL {

  /*! \defgroup sequence_io_grp Simple I/O of sequences
      \ingroup helperfunctors

      Input of a sequence from a stream containing other items than
      sequence members is not directly supported for STL sequences.
      This is solved by a simple utility class reading (and writing)
      from (to) a stream explicitly stating the number of elements.

      \see Module \ref iteratorrange 

      \b Example
      \code
      // stream 'in' contains:
      // 3 Paul Peter Michael
      // 4 Jane Anne Maria Betty
      
      
      std::vector boys_names;
      std::vector girls_names;
      in >> sequence_io(boys_names) >> sequence_io(girls_names);
      cout << "Boys: "  << sequence_io(boys_names)  << "\n" 
           << "Girls: " << sequence_io(girls_names) << endl;
      /_* prints:
      Boys:  3 Paul Peter Michael
      Girls: 4 Jane Anne Maria Betty
      *_/
      \endcode
  */    

  /*! \brief Simple wrapper to allow I/O of sequences
    \ingroup sequence_io_grp

    \templateparams \c  C is a model of STL Back Insertion Sequence 
         (only \c push_back() is needed)

     Operator \c >> and \c << are overloaded.
   */
  template<class C>
  class sequence_io_helper {
  private:
    GrAL::ref_ptr<C> cont;
  public:
    sequence_io_helper() {}
    sequence_io_helper(C & c) : cont(c) {}
    sequence_io_helper(GrAL::ref_ptr<C> c) : cont(c) {}
    
    void read(std::istream& in) {
      int n;
      in >> n;
      for(int i = 0; i < n; ++i) {
	typename C::value_type t;
	bool good = (in >> t);
	REQUIRE_ALWAYS(good, "could not read item " << i << " of " << n, 1);
	cont->push_back(t);
      }
    }
    void write(std::ostream& out) {
      out << cont->size();
      for(typename C::const_iterator i = cont->begin(); i != cont->end(); ++i)
	out << " " << *i;
    }
  };

  /*! \brief Read from a stream of the form \f$ n c_1 \ldots c_n \f$

     \ingroup sequence_io_grp
  */
  template<class C>
  inline std::istream& operator>>(std::istream& in,  sequence_io_helper<C> s) 
  { s.read(in);  return in;}

  /*! \brief Write C in a form that can be read back via operator>>
      \ingroup sequence_io_grp
  */
  template<class C>
  inline std::ostream& operator<<(std::ostream& out, sequence_io_helper<C> s) 
  { s.write(out); return out;}

  /*! \brief Creator function for sequence_io_helper
    \ingroup sequence_io_grp
  */
  template<class C>
  inline sequence_io_helper<C> sequence_io(C & c)        { return  sequence_io_helper<C>(c);}
  /*! \brief Creator function for sequence_io_helper
    \ingroup sequence_io_grp
  */
  template<class C>
  inline sequence_io_helper<C> sequence_io(ref_ptr<C> c) { return  sequence_io_helper<C>(c);}

} // namespace GrAL 



#endif
