#ifndef GRAL_BASE_GB_MAPPED_ITERATORS_H
#define GRAL_BASE_GB_MAPPED_ITERATORS_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"


/*! \file
    \brief mapped iterators and sequences 
*/


/*! \brief Maps STL-style random-access iterator (range [b,e) pairs) 
    to IsDone()-style iterator
    \ingroup iterators
    \see \ref iterators module
     
    \b Template parameters:
    - C; container with begin()/end(), and random-access

    The class is stable with respect 
    to container changes that invalidate iterators.
*/
template<class C, class F>
class mapped_element_ra_seq_iterator {
  int b;
  int e;
  C  const* container;
  F  f;

  typedef mapped_element_ra_seq_iterator<C,F> self;
public:
  typedef typename F::grid_type      grid_type;
  typedef typename F::result_type    value_type;
  typedef typename F::argument_type  handle_type;
  
  mapped_element_ra_seq_iterator() : container(0) {}
  mapped_element_ra_seq_iterator(int bb, int ee, C const& cont, F ff) 
    : b(bb), e(ee), container(&cont), f(ff) {}

  self& operator++()           { ++b; return *this;}
  value_type operator*() const { return f(*(container->begin() + b));}
                                // note: value_type, not value_type const& !
  bool IsDone()          const { return (b == e);}

  //  handle_type handle() const { return (*(container->begin() + b));}
  handle_type handle() const { return f(*(container->begin() + b)).handle();}
  grid_type const& TheGrid() const { return f.TheGrid();}

    
 
  //friend 
  bool operator==(const self& rs) const { return (b == rs.b);}
  bool operator!=(const self& rs) const { return (b != rs.b);}
};





/*! \brief Maps STL-style Forward Iterator (range [b,e) pairs) 
    to IsDone()-style iterator
    \ingroup iterators
    \see \ref iterators module
     
    \b Template parameters:
    - SeqIt: Forward Iterator
    - F: Adaptable Unary Function, defined on SeqIt::value_type

*/
template<class SeqIt, class F>
class mapped_element_seq_iterator {
  SeqIt b;
  SeqIt e;
  F  f;

  typedef mapped_element_seq_iterator<SeqIt,F> self;
public:
  typedef typename F::grid_type      grid_type;
  typedef typename F::result_type    value_type;
  typedef typename F::argument_type  handle_type;
  
  mapped_element_seq_iterator() {}
  mapped_element_seq_iterator(SeqIt bb, SeqIt ee, F ff) : b(bb), e(ee), f(ff) {}

  self& operator++()           { ++b; return *this;}
  value_type operator*() const { return f(*b);} // note: value_type, not value_type const& !
  bool IsDone()          const { return (b == e);}

  grid_type const& TheGrid() const { return f.TheGrid();}
  //  handle_type handle() const { return (*b);}
  handle_type handle() const { return f(*b).handle();}

  //friend 
  bool operator==(const self& rs) { return (b == rs.b);}
  bool operator!=(const self& rs) { return (b != rs.b);}
};


/*! \brief Maps  IsDone()-style iterators by composing with function object
    \ingroup iterators
    \see \ref iterators module

    Template parameters:
    - F: Adaptable Unary Function, 
    <tt>result_type</tt> is a $GrAL GridElement type.
    - It: Iterator with IsDone()
 */
template<class It, class F>
class mapped_element_iterator {
private:
  It   it; // semantics: f o It
  F    f;

  typedef mapped_element_iterator<It,F> self;
public:
  typedef typename F::result_type                      Element;
  typedef typename element_traits<Element>::grid_type  grid_type;
  //  typedef typename F::grid_type      grid_type;
  typedef typename F::result_type    value_type;

  //  mapped_cell_iterator() {}
  mapped_element_iterator(const It& i, const F& ff) : it(i), f(ff) {}  

  self&       operator++()      { ++it; return *this;}
  value_type  operator*() const { return f(*it);}
  bool        IsDone()    const { return it.IsDone();}

  
  // friend 
  bool operator==(const self& rs) { return (it == rs.it);}
  bool operator!=(const self& rs) { return (it != rs.it);}
};


/*! \brief A cell sequence defined by another cell sequence and a mapping
    \ingroup subranges
    \see \ref subranges module    

    <b> Template parameters: </b>
    - G: $Gral CellGridRange
    - F: Adaptable Unary Function,
    <tt>result_type</tt> is a $GrAL GridCell type.
 */
template<class G, class F>
class mapped_cell_sequence {
private:
  const G* g;
  F        f;

  typedef grid_types<G>                 src_gt;
  typedef typename src_gt::CellIterator src_iter;

  typedef typename F::result_type                   Cell;
  typedef typename element_traits<Cell>::grid_type  grid_type;
public:
  mapped_cell_sequence(const G& gg, const F& ff) : g(&gg), f(ff) {}

  typedef mapped_element_iterator<src_iter,F> CellIterator;

  CellIterator FirstCell()  const { return CellIterator(g->FirstCell(),f);}
  unsigned     NumOfCells() const { return (g->NumOfCells()); }
};

/*! \brief Creator function for mapped_cell_sequence

    \ingroup subranges
    \see \ref subranges module    

    \relates mapped_cell_sequence
*/
template<class G, class F>
inline mapped_cell_sequence<G,F> map_cell_sequence(const G& g, const F& f)
{ return mapped_cell_sequence<G,F>(g,f); }



#endif
