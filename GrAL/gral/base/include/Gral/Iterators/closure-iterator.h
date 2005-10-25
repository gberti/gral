#ifndef GRAL_BASE_GB_CLOSURE_ITERATOR_H
#define GRAL_BASE_GB_CLOSURE_ITERATOR_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

namespace GrAL {


/*! \brief  Iterator over the k-elements incident to a set of n-elements
    \ingroup iterators
    
    \templateparams
     - \c MajorIt: Model of $GrAL SequenceIterator for dimension n
     - \c MinorIt: Model of $GrAL IncidenceIterator on elements of dimension n
          - constructible from \c *major, where \c major is of type MAJORIT
          - typedef value_type
     - GT
       - typedef grid_type
       - 
    \see Test in \ref  test-closure-iterator.C
    \see \ref iterators module  

*/

  template<class MAJORIT, class MINORIT, class GT = grid_types<typename MAJORIT::grid_type> >
  class closure_iterator {
    typedef closure_iterator<MAJORIT, MINORIT, GT> self;
  public:
    typedef GT                                gt;
    typedef typename gt::grid_type            grid_type;
    typedef MAJORIT                           major_iterator_type;
    typedef MINORIT                           minor_iterator_type;
    typedef typename MAJORIT::anchor_type     anchor_type;
    typedef typename MINORIT::value_type      value_type;
    typedef typename MINORIT::value_type      element_type;
    typedef element_traits<value_type>        vet;
    
  private:
    major_iterator_type major_it;
    minor_iterator_type minor_it;
    partial_grid_function<value_type,bool> visited;
    
  public:
    closure_iterator() {} 
    closure_iterator(anchor_type         const& g)  : major_it(g),  visited(TheGrid()) { init();}
    closure_iterator(ref_ptr<anchor_type const> g)  : major_it(*g), visited(TheGrid()) { init();}
    closure_iterator(major_iterator_type const& mj) : major_it(mj), visited(TheGrid()) { init(); }
    
    self&   operator++() {
      cv();
      advance();
      while(! IsDone() && visited(value()))
	advance();
      return *this;
    }
    value_type operator*()             const { cv(); return value();}
    bool  IsDone()                     const { cb(); return major_it.IsDone();}
    typename vet::handle_type handle() const { cv(); return minor_it.handle();}
    
    ref_ptr<grid_type const> TheGrid() const { cb(); return ref_ptr<grid_type const>(major_it.TheGrid());}
    
    bool operator==(self const& rhs)
    { rhs.cb(); cb(); return (rhs.major_it == major_it) && (rhs.minor_it == minor_it);}
    bool operator!=(self const& rhs) { return !(*this==rhs);}
    
    bool bound() const { return major_it.bound();}
    bool valid() const { return bound() && minor_it.valid();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}
  private:
    value_type  value() const { cv(); return (*minor_it);}
    
    void init()  {
      if(! major_it.IsDone())
	minor_it = minor_iterator_type(*major_it);
    }
    
    void advance() { 
      visited[value()] = true;
      ++minor_it; 
      if(minor_it.IsDone()) {
	++major_it;
	if(! major_it.IsDone())
	  minor_it =  minor_iterator_type(*major_it);
      } 
    }
  };

} // namespace GrAL 

#endif
