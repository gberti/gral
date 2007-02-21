#ifndef GRAL_GB_COMPLEX2D_VERTEX2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_VERTEX2D_ITERATOR_H



// $LICENSE


#include "Gral/Grids/Complex2D/complex2d.h"

namespace GrAL {

class Vertex2D_Iterator : public complex2d_types {
  friend class Complex2D;
  typedef Vertex2D_Iterator self;
private:
  //----  DATA  ------
  vertex_handle    _pos;
  vertex_handle    _endpos;
  Complex2D const* _cc;

public:
  typedef grid_sequence_iterator_category_d<0>  category;
  typedef Vertex    value_type;
  typedef grid_type anchor_type;
public:

  //------  constructors ---------------

  Vertex2D_Iterator() : _pos(-1), _endpos(-1),  _cc(0)  {}
  explicit Vertex2D_Iterator(Complex2D const& CC)
    : _pos(0), _cc(&CC) { _endpos = CC.NumOfVertices(); }
  Vertex2D_Iterator(Complex const& CC, const vertex_handle& v)
    : _pos(v) , _cc(&CC) {  _endpos = CC.NumOfVertices(); }

  ~Vertex2D_Iterator() {}

  //------ iterator operations -----------------

  self& operator++()  {++_pos; return (*this);} 
  Vertex operator*() const { return Vertex2D(*_cc,_pos);}
  bool IsDone() const {return (_pos == _endpos);}
  operator bool() const { return !IsDone();}

  vertex_handle handle() const { return _pos;}
  bool valid() const { return 0 <= _pos && _pos < _endpos;}

  //------------ comparision  -------------------

  friend bool operator==(const self& v, const self& w) {
    REQUIRE((v._cc == w._cc), "Grids don't match!\n",1);
    return (v._pos == w._pos);
  }
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

  friend bool operator<(const self& v, const self& w){
    REQUIRE((v._cc == w._cc), "Grids don't match!\n",1);
    return (v._pos < w._pos);
  }

  //-------------  anchor ------------------------

  const Complex2D& TheGrid  () const { return (*_cc);}
  const Complex2D& TheAnchor() const { return (*_cc);}
};

} // namespace GrAL {

 
#endif
