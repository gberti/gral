#ifndef NMWR_GB_EDGE2D_ITERATOR_H
#define NMWR_GB_EDGE2D_ITERATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Grids/Complex2D/complex2d.h"

class Edge2D_Iterator  : public complex2d_types {
  typedef Edge2D_Iterator       self;
private:
  //----- DATA --------
  GridCellIterator      _c;  // current cell
  FacetOnCellIterator   _fc; // current edge == current facet of cell
  // invariant: (*(_c) == _fc.TheCell()) || IsDone()
public:
  typedef edge_handle   handle_type;
  typedef Edge          element_type;
  typedef Edge          value_type;
  typedef grid_type     anchor_type;
  
  //--------- constructors -------------------------

  Edge2D_Iterator() {}
  Edge2D_Iterator(GridCellIterator    const& c, 
		  FacetOnCellIterator const& nb);
  Edge2D_Iterator(Complex2D const&);

  Edge2D_Iterator(const self& e) : _c(e._c), _fc(e._fc) {}
  self& operator=(const self& e) 
    { if (this != &e) { _c = e._c; _fc = e._fc; } return (*this);}

  ~Edge2D_Iterator() {}

  
  //---------- iterator operations ------------------
  inline self& operator++();
  Edge operator*() const {return (Edge(_fc));} 
  bool  IsDone() const {return (_c.IsDone());}

  handle_type    handle()  const { return TheGrid().handle(this->operator*());}
  Complex const& TheGrid() const {return (_c.TheGrid());}

  friend bool operator==(const self& ls, const self& rs) {
    // relies on shortcircuit to prevent (ls._fc == rs._fc) from
    // being executed if (ls._c != rs._c), thus violating the precondition
    // of the first comparision.
    // Also, this relies on Edge2D_Iterator visiting edges always from
    // one and the same side -- the `inverse' iterator won't compare equal.
    return (    (ls.IsDone() && rs.IsDone()) 
	     || ((ls._c == rs._c) && (ls._fc == rs._fc)));
  }

private:
  //---------- helper functions for iteration -------
  inline void _advance_till_valid();
  inline void _advance_local();
  inline bool _is_new_edge();
};


inline Edge2D_Iterator::Edge2D_Iterator(Complex2D const& CC)
  : _c(CC)
{
  if(! _c.IsDone())
    _fc = (*_c).FirstFacet();
  _advance_till_valid();
}

inline Edge2D_Iterator::Edge2D_Iterator(const Cell2D_Iterator& c,
					const EdgeOnCell2D_Iterator& nb)
 : _c(c), _fc(nb) 
{
  REQUIRE( ( (_c.IsDone()) || (*_c) == _fc.TheCell()), 
	   "Edge2D_Iterator: Constructor called with nb not on c!",1); 

  _advance_till_valid();
}

inline void Edge2D_Iterator::_advance_till_valid()
{
  while( ! IsDone() && ! _is_new_edge())
    ++(*this);
}

inline bool Edge2D_Iterator::_is_new_edge()
  // check, if the current edge (facet) is not
  // already visited from the other side.
{ 
  return (   TheGrid().IsOnBoundary(_fc)
	     || TheGrid().handle(this->operator*().C2()) <
	        TheGrid().handle(_c));
}

inline void Edge2D_Iterator::_advance_local()
{
 while(! _fc.IsDone()
       && ! _is_new_edge()) 
   ++_fc;
}

inline  Edge2D_Iterator& Edge2D_Iterator::operator++()
{ 
  ++_fc;
  _advance_local();
  while(_fc.IsDone() && ! _c.IsDone()) {
    ++_c;
    if(! _c.IsDone()){
      _fc = (*_c).FirstFacet();
      _advance_local();
    }
  }
    
  return (*this); 
}




#endif
