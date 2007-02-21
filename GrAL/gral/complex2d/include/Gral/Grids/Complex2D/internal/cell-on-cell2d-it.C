#ifndef GRAL_GB_COMPLEX2D_CELL_ON_CELL2D_INLINE_H
#define GRAL_GB_COMPLEX2D_CELL_ON_CELL2D_INLINE_H

// $LICENSE

namespace GrAL {

inline 
CellOnCell2D_Iterator::CellOnCell2D_Iterator(Cell2D const& C)
     : _f(C.FirstFacet())
{
  make_valid(); 
}

inline CellOnCell2D_Iterator
CellOnCell2D_Iterator::CyclicSucc() const 
{ 
  EdgeOnCell2D_Iterator f = _f.CyclicSucc();
  while(TheGrid().IsOnBoundary(f))
    f = f.CyclicSucc();
  return self(f);
}

inline CellOnCell2D_Iterator
CellOnCell2D_Iterator::CyclicPred() const 
{
  EdgeOnCell2D_Iterator f = _f.CyclicPred();
  while(TheGrid().IsOnBoundary(f))
    f = f.CyclicPred();
  return self(f);
}

inline void CellOnCell2D_Iterator::make_valid()
{
  while( (! _f.IsDone()) && TheGrid().IsOnBoundary(_f))
    ++_f;
}

inline 
CellOnCell2D_Iterator& 
CellOnCell2D_Iterator::operator--()
{
  do {
    --_f;
  } while (  TheGrid().IsOnBoundary(_f) && ! IsDone());
  return (*this);
}

} // namespace GrAL {

#endif
