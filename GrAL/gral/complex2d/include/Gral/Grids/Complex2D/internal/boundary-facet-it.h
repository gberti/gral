#ifndef NMWR_GB_BOUNDARY_FACET_2D_ITERATOR
#define NMWR_GB_BOUNDARY_FACET_2D_ITERATOR



// $LICENSE


class BoundaryFacet2D_Iterator {
public:
  typedef Complex2D Complex;
  typedef  Complex::Facet Facet;
  typedef  Complex::boundary_facet_list bf_list;
  typedef  bf_list::const_iterator BfIt;
  typedef BoundaryFacet2D_Iterator self;


  //  BoundaryFacet2D_Iterator() {}
  BoundaryFacet2D_Iterator() : _cc(0) {} 
  BoundaryFacet2D_Iterator(const BfIt& bf, const Complex* cc) 
    : _bf(bf), _cc(cc) {}
  self& operator++() { ++_bf; return(*this);}
  self  operator++(int) {self tmp(*this); ++(*this); return tmp;}

  Facet operator*() const { return Facet(*_bf);}
  
  bool IsDone() const { return (_bf == TheGrid()._boundary.end());}
  const Complex& TheGrid() const {
    REQUIRE((_cc != 0), "no grid!\n",1);
    return (*_cc);
  }

private:
  //----- DATA -----
  BfIt           _bf;
  const Complex* _cc;

public:
  friend std::ostream& operator<<(std::ostream& out, const BoundaryFacet2D_Iterator& it)
    { return (out << *(it._bf)); }
};

#endif
