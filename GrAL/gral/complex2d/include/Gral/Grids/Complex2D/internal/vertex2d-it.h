#ifndef NMWR_GB_VERTEX2D_ITERATOR_H
#define NMWR_GB_VERTEX2D_ITERATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Gral/Grids/Complex2D/complex2d.h"


class Vertex2D_Iterator : public complex2d_types {
  friend class Complex2D;
  typedef Vertex2D_Iterator self;
private:
  //----  DATA  ------
  vertex_handle    _pos;
  vertex_handle    _endpos;
  Complex2D const* _cc;

public:

  //------  constructors ---------------

  Vertex2D_Iterator() : _pos(-1), _endpos(-1),  _cc(0)  {}
  Vertex2D_Iterator(const vertex_handle& v, Complex const& CC) 
    : _pos(v) , _cc(&CC) {  _endpos = CC.NumOfVertices(); }
  Vertex2D_Iterator(Complex2D const& CC)
    : _pos(0), _cc(&CC) { _endpos = CC.NumOfVertices(); }

  Vertex2D_Iterator(const self& s)
     { _pos=s._pos; _endpos = s._endpos; _cc = s._cc;}
  self& operator=(const self& s)
     { if (this != &s) { _pos=s._pos; _endpos = s._endpos; _cc = s._cc;} return (*this);}

  ~Vertex2D_Iterator() {}

  //------ iterator operations -----------------

  self& operator++()  {++_pos; return (*this);} 
  Vertex operator*() const { return Vertex2D(_pos,*_cc);}
  bool IsDone() const {return (_pos == _endpos);}

  vertex_handle handle() const { return _pos;}

  //------------ comparision  -------------------

  friend bool operator==(const self& v, const self& w) {
    REQUIRE((v._cc == w._cc), "Grids don't match!\n",1);
    return (v._pos == w._pos);
  }

  friend bool operator<(const self& v, const self& w){
    REQUIRE((v._cc == w._cc), "Grids don't match!\n",1);
    return (v._pos < w._pos);
  }

  //-------------  anchor ------------------------

  const Complex2D& TheGrid() const { return (*_cc);}
};

 
#endif
