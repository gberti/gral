
// $LICENSE

inline 
EdgeOnCell2D_Iterator::EdgeOnCell2D_Iterator(CellOnCell2D_Iterator const& nb)
{ *this = nb._f;}

inline Edge2D
EdgeOnCell2D_Iterator::operator*() const { return  Edge(*this); }

inline EdgeOnCell2D_Iterator::Cell const&
EdgeOnCell2D_Iterator::TheCell() const { return C;}

inline EdgeOnCell2D_Iterator::Cell
EdgeOnCell2D_Iterator::OtherCell() const 
{ return TheGrid().cell(TheGrid()._cells[C.handle()]._neighbours[lf]);}

inline EdgeOnCell2D_Iterator
EdgeOnCell2D_Iterator::CyclicSucc() const 
{ 
  return self(( lf == TheCell().NumOfEdges()-1 ? 0 : lf+1), C);
}

inline EdgeOnCell2D_Iterator
EdgeOnCell2D_Iterator::CyclicPred() const 
{
  return self((lf == 0 ? TheCell().NumOfEdges()-1 : lf-1), C);
}

inline
void EdgeOnCell2D_Iterator::print(ostream& out)  const
{
  out << "C: " << C
      << " lf: " << lf; 
}
