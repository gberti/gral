#ifndef NMWR_GB_SAFE_FILEIO_H
#define NMWR_GB_SAFE_FILEIO_H



// $LICENSE


#include <fstream>
#include "Forward/string_fwd.h"

namespace GrAL {



/*! \brief Try to open a file and ask if it does not exist.
 
    \ingroup io

   This class povides some functions to open and close
   files in a robust (?) manner.
   If a file with a given name does not exist,
   the user is prompted for a valid filename.
   Also, the class may search for a compressed file with a .gz suffix.

   The opened ifstream in will be returned, so that 
   the following usages are possible:

   \code
   ifstream in;
   file_interactive::open(in,"xyz.dat") >> x >> y >> z;
   \endcode

   or

   \code
   file_interactive::open_gz(in,"xyz.dat");
   in >> x >> y >> z;
   \endcode
   This tries to open \c "xyz.dat" or, failing this, \c "xyz.dat.gz".
*/ 



class file_interactive {
public:
  enum { is_open,     //!< open ok
         is_open_gz , //!< open ok, file was gzipped
         insist,      //!< if open fails: Ask while no valid file name
         no_insist,   //!< ask only once, then proceed.
         failed       //!< open has failed
       };
  //! Try to open file  filename.  
  /*! If strictness = insist, do ask interactively
      if file could not be opened.

      Result is in { is_open, failed }. */
  static int open(::std::ifstream& in,  ::std::string const& filename);

  ///! Try to open also filename.gz if filename does not exist.
  /*! If strictness = insist, do ask interactively
      if file could not be opened.

      Result is in { is_open, is_open_gz, failed }.
  */
  static int open_gz(::std::ifstream& in,  ::std::string const& filename, int strictness = insist);
  //! Close ifstream. If gz = is_open_gz, gzip the file.
  static void       close(::std::ifstream& in, ::std::string const& nm, int gz = is_open);

  //! Open ofstream.
  static int        open (::std::ofstream& out, ::std::string const& filename);
    // FIXME: does not work for both gcc 2.95 and 3.0.1  
  //	  ::std::ios_base::openmode mode = ::std::ios_base::out);


  //! close ofstream.
  /*! gz in { is_open, is_open_gz }.

      Parameter nm needed only if gz == is_open_gz (cannot deduce file name from stream).
  */
  static void       close(::std::ofstream& in, ::std::string const& nm, int gz = is_open);
};

} // namespace GrAL 

#endif
