#ifndef GRAL_BASE_GB_CLOSURE_ITERATOR_H
#define GRAL_BASE_GB_CLOSURE_ITERATOR_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include <map>

namespace GrAL {

  namespace closure_iterator_detail {

    template<class VAL_CAT, class ANCHOR_CAT, int ANCHOR_IS_GRID>
    struct get_cat_aux 
      : get_sequence_iterator_category <VAL_CAT> {};
    template<class VAL_CAT, class ANCHOR_CAT>
    struct get_cat_aux<VAL_CAT, ANCHOR_CAT,0>
      : get_incidence_iterator_category<VAL_CAT, ANCHOR_CAT> {};

    template<class VAL, class ANCHOR>
    struct get_category 
      : get_cat_aux<typename category<VAL>   ::type,
		    typename category<ANCHOR>::type, 
		    is_grid_range<ANCHOR>::value> {};

  } //  namespace closure_iterator_detail

/*! \brief  Iterator over the k-elements incident to a set of n-elements
    \ingroup iterators
    
    \templateparams
     - \c MajorIt: Model of $GrAL SequenceIterator for dimension n
     - \c MinorIt: Model of $GrAL IncidenceIterator on elements of dimension n
          - constructible from \c *major, where \c major is of type MAJORIT
          - nested type \c value_type is a k-dimensional $GrAL GridElement
     - \c GT
       - type \c GT::grid_type 
       - type \c GT::size_type
    \see Test in \ref  test-closure-iterator.C
    \see \ref iterators module  

    \todo Copying may be expensive. By introducing a shared table of numbered visited nodes 
     and adding an extra position parameter, copying may be made cheep while still fulfilling 
     the forward iterator requirements.
*/

  template<class MAJORIT, class MINORIT, class GT = grid_types<typename MAJORIT::grid_type> >
  class closure_iterator {
    typedef closure_iterator<MAJORIT, MINORIT, GT> self;
  public:
    typedef GT                                gt;
    typedef typename gt::grid_type            grid_type;
    typedef typename gt::size_type            size_type;
    typedef MAJORIT                           major_iterator_type;
    typedef MINORIT                           minor_iterator_type;
    typedef typename MAJORIT::anchor_type     anchor_type;
    typedef typename MAJORIT::value_type      major_element_type;
    typedef typename MINORIT::value_type      value_type;
    typedef typename MINORIT::value_type      element_type;
    typedef element_traits<value_type>         min_et;
    typedef element_traits<major_element_type> maj_et;    

    typedef typename closure_iterator_detail::get_category<value_type, anchor_type>::type category;
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
    typename min_et::handle_type handle() const { cv(); return minor_it.handle();}
    
    ref_ptr<grid_type   const> TheGrid()   const { cb(); return ref_ptr<grid_type   const>(major_it.TheGrid());}
    ref_ptr<anchor_type const> TheAnchor() const { cb(); return ref_ptr<anchor_type const>(major_it.TheAnchor());}

    bool operator==(self const& rhs) { 
      rhs.cb(); cb(); 
      // two past-the-end iterators always compare equal
      return ((rhs.IsDone() && IsDone()) || ((rhs.major_it == major_it) && (rhs.minor_it == minor_it)));
    }
    bool operator!=(self const& rhs) { return !(*this==rhs);}
    
    bool bound() const { return major_it.bound();}
    bool valid() const { return bound() && minor_it.valid();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}

    static self      begin(anchor_type const& a) { return self(GrAL::begin<major_iterator_type>(a));}
    static self      end  (anchor_type const& a) { return self(GrAL::end  <major_iterator_type>(a));}
    static size_type size (anchor_type const& a) { init_sizes(a); return (*sizes)[&a];}

    typedef std::map<anchor_type const*, size_type> size_table;
    static  size_table * sizes;
  private:
    static void init_sizes() {
      if(sizes == 0)
	sizes = new size_table;
    }
    static void init_sizes(anchor_type const& g) {
      init_sizes();
      if(sizes->find(&g) == sizes->end())
	(*sizes)[&g] = compute_size(g);
    }
    static size_type compute_size(anchor_type const& g) {
      size_type sz = 0;
      for(self it = begin(g); !it.IsDone(); ++it)
	++sz;
      return sz;
    }

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
  }; // class closure_iterator

  template<class MAJORIT, class MINORIT, class GT>
  typename closure_iterator<MAJORIT, MINORIT, GT>::size_table 
  * closure_iterator<MAJORIT, MINORIT, GT>::sizes;



  template<class MAJORIT, class MINORIT, class GT>
  closure_iterator<MAJORIT, MINORIT, GT>
  gral_begin(typename MAJORIT::anchor_type const& a, 
	     closure_iterator<MAJORIT, MINORIT, GT>) 
  { return closure_iterator<MAJORIT, MINORIT, GT>(a);}

  template<class MAJORIT, class MINORIT, class GT>
  closure_iterator<MAJORIT, MINORIT, GT>
  gral_end (typename MAJORIT::anchor_type const& a, 
	     closure_iterator<MAJORIT, MINORIT, GT>) 
  { return closure_iterator<MAJORIT, MINORIT, GT>(GrAL::end<MAJORIT>(a));}


  // size is difficult, because O(n) -> better handled in grid
  template<class MAJORIT, class MINORIT, class GT>
  typename GT::size_type
  gral_size(typename MAJORIT::anchor_type const& a, 
	    closure_iterator<MAJORIT, MINORIT, GT>) 
  { return closure_iterator<MAJORIT, MINORIT, GT>::size(a);}




} // namespace GrAL 

#endif
