
// $LICENSE

inline 
EdgeOnCell2D_Iterator::EdgeOnCell2D_Iterator(CellOnCell2D_Iterator const& nb)
{ *this = nb._f;}

inline Edge2D
EdgeOnCell2D_Iterator::operator*() const { return  Edge(*this); }

inline  EdgeOnCell2D_Iterator::vertex_handle 
EdgeOnCell2D_Iterator::v1() const { return TheCell().vertex(1,*this);}

inline  EdgeOnCell2D_Iterator::vertex_handle 
EdgeOnCell2D_Iterator::v2() const { return TheCell().vertex(2,*this);}

inline  EdgeOnCell2D_Iterator::cell_handle   
EdgeOnCell2D_Iterator::the_cell()   const { return C.handle();}
  
inline  EdgeOnCell2D_Iterator::cell_handle   
EdgeOnCell2D_Iterator::other_cell() const 
{ return TheGrid()._cells[C.handle()]._neighbours[lf];}

inline   Cell2D const& 
EdgeOnCell2D_Iterator::TheCell()   const { return C;}

inline  Cell2D        
EdgeOnCell2D_Iterator::OtherCell() const { return Cell(TheGrid(),other_cell()); }

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
void EdgeOnCell2D_Iterator::print(std::ostream& out)  const
{
  out << "C: " << C
      << " lf: " << lf; 
}
