#ifndef NMWR_GB_GRID_FUNCTIONS_C
#define NMWR_GB_GRID_FUNCTIONS_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Complex2D/generic/grid-functions.C"
#include <iostream.h>

template<class E, class T, class hashE>
inline void 
grid_function_complex2d_hash<E,T,hashE>::read(istream& in)
{
  ElementIterator e = et::FirstElement(TheGrid()); 
  while( (! e.IsDone()) && (in >> table[*e]))
    ++e;
  if (! e.IsDone())  { // could not read all elements
    cerr << "Bad input stream, current element: " << TheGrid().handle(*e) << endl;
    abort();
  }
}

template<class E, class T, class hashE>
inline void 
grid_function_complex2d_hash<E,T,hashE>::write(ostream& out) const
{
  ElementIterator e = et::FirstElement(TheGrid()); 
  while( (! e.IsDone()) && (out << (*this)(*e) ))
    ++e;
  if (! e.IsDone())  { // could not write all elements
    cerr << "Bad output stream, current element: " << TheGrid().handle(*e) << endl;
    abort();
  }
}


template<class E, class T>
inline void 
grid_function_complex2d_vector<E,T>::read(istream& in)
{
  iterator e = table.begin();
  while( (e != table.end()) && (in >> *e))
    ++e;
  if (e != table.end() )  { // could not read all elements
    cerr << "Bad input stream, current element no.  " 
	 << (e - table.begin()) << ", size = " << table.size() << endl;
    abort();
  }
}

template<class E, class T>
inline void 
grid_function_complex2d_vector<E,T>::write(ostream& out) const
{
  const_iterator e = table.begin();
  while( (e != table.end()) && (out << *e))
    ++e;
  if (e != table.end() )  { // could not read all elements
    cerr << "Bad output stream, current element no. " <<  (e - table.begin()) << endl;
    abort();
  }
}

#endif
