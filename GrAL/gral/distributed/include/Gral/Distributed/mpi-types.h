#ifndef NMWR_GB_MPI_TYPES_H
#define NMWR_GB_MPI_TYPES_H



// $LICENSE


#include <mpi.h>


/*! \brief traits class to help calling MPI-routines
    from template functions.
    \ingroup mpidistributedgrids   

   Specializations exist for int, short, double, float, long double.
*/

template<class T> struct mpi_types {};

template<>
struct mpi_types<int> { 
  static MPI_Datatype tag() { return MPI_INTEGER; }
};

template<>
struct mpi_types<short> { 
  static MPI_Datatype tag() { return MPI_SHORT; }
};


template<>
struct mpi_types<float> { 
  static MPI_Datatype tag() { return MPI_FLOAT; }
};

template<>
struct mpi_types<double> { 
  static MPI_Datatype tag() { return MPI_DOUBLE; }
};

template<>
struct mpi_types<long double> { 
  static MPI_Datatype tag() { return MPI_LONG_DOUBLE; }
};


#endif
