#ifndef NMWR_GB_RANGE_LAYERS_H
#define NMWR_GB_RANGE_LAYERS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "compiler-config.h"
#include "Grids/enumerated-subrange.h"

//----------------------------------------------------------------
// 
// class range_layers<E>;
//
// class allowing economic storage of layered overlap ranges
// and efficient combination of adjacent ranges
// like exported = exposed + shared
//      imported = shared  + copied
//
// CONTENTS:
// ---------
//  [1] template<class E> class range_layers;
//  [2a] template<class EltRange>
//       void write_rge(EltRange const& rge, ostream & out);
//  [2b] template<class OVRange>
//       void write_ovrge(OVRange const& rge, ostream & out);
// 
// DESCRIPTION:
// -----------
//  [1] gives economic storage for adjacent ranges of elements of type E:
//       private   exposed    shared    copied
//      +--------+----------+---------+---------+
//      |-     owned       -|-    imported     -|
//               |-      exported    -|
//
//  [2a] writes an element range to a stream
//  [2b] writes an overlap range to a stream (with range layers name as above)
// 
// CONSTRAINTS on template parameters:
// -----------------------------------
//
//  [1] E is a grid element type, element_traits<E> must be specialized.
//
//----------------------------------------------------------------



//----------------------------------------------------------------
//   [1]     class range_layers<E>
//----------------------------------------------------------------

template<class E>
class range_layers {
  typedef range_layers<E>            self;
  MAKE_DEFAULT_OPS_SELF
public:
  typedef element_traits<E>          et;
  typedef typename et::grid_type     grid_type;
  typedef grid_types<grid_type>      gt;
  typedef typename et::handle_type   element_handle;
  typedef E                          Element;

  typedef vector<element_handle>               element_seq;
  typedef typename element_seq::iterator       e_iterator;
  typedef typename element_seq::const_iterator const_e_iterator;
  typedef element_handle                       value_type;
  typedef element_handle  const                const_reference;


  typedef element_range_ref<E,element_seq>     range_ref;

private:
  //----- DATA -----

  const grid_type*  the_grid;
  element_seq       the_range;
 
  //  private   exposed    shared    copied
  // ---------+----------+---------+---------+
  // |-     owned       -|-    imported     -|
  //          |-      exported    -|

  int exposed_beg;
  int shared_beg;
  int copied_beg;

public:
  //------------------- construction ------------------------

  range_layers() : the_grid(0) { init();}
  range_layers(const grid_type& g) : the_grid(&g) { init();}

  void set_grid(const grid_type& g) {
    REQUIRE(the_range.empty(),"set_grid(): range not empty!",1);
    the_grid = &g;
  }

  void init() { 
    REQUIRE((the_range.size() == 0), "init(): range not empty!\n",1);

    exposed_beg = shared_beg = copied_beg = 0;
  }


  void set_exposed_begin() { exposed_beg = the_range.size();}
  void set_shared_begin()  { shared_beg  = the_range.size();}
  void set_copied_begin()  { copied_beg  = the_range.size();}

  void push_back(element_handle eh) {
    REQUIRE( (the_grid != 0), "push_back(): no grid!\n",1);
    the_range.push_back(eh);
  }


  //------------------ component access  ----------------------

  const grid_type& TheGrid() const { 
    REQUIRE((the_grid != 0), "no grid!\n",1);
    return *the_grid;
  }

  element_seq const& TheContainer() const { return the_range;}
  element_seq      & TheContainer()       { return the_range;}
 
  //------------------ element ranges  ----------------------

  bool empty() const 
    { return (the_range.size() == 0);}

  size_t size() const { return the_range.size();}

  // "atomic"
  // private: really store the whole private range?
  
  int privee_begin() const { return 0;}
  int privee_end()   const { return exposed_beg;}
  range_ref     privee() const 
    { return range_ref(privee_begin(),privee_end(), the_range, *the_grid); }
  
  int exposed_begin() const { return exposed_beg;}
  int exposed_end()   const { return shared_beg;}
  range_ref exposed() const 
   { return range_ref(exposed_begin(), exposed_end(), the_range, *the_grid);}

  int shared_begin() const { return shared_beg;}
  int shared_end()   const { return copied_beg;}
  range_ref shared() const 
   { return range_ref(shared_begin(), shared_end(), the_range, *the_grid);}

  int copied_begin()  const { return copied_beg;}
  int copied_end()    const { return the_range.size();}
  range_ref copied() const 
    { return range_ref(copied_begin(), copied_end(), the_range, *the_grid);}



  // "composed"
  
  range_ref local() const 
    { return range_ref(privee_begin(),shared_end(), the_range, *the_grid); }

  int owned_begin() const { return privee_begin();}
  int owned_end()   const { return exposed_end();}
  range_ref owned() const 
    { return range_ref(owned_begin(), owned_end(), the_range, *the_grid);}
  

  int exported_begin() const { return exposed_begin();}
  int exported_end()   const { return shared_end();}
  range_ref   exported() const 
    { return range_ref(exported_begin(),exported_end(), the_range, *the_grid);}

  int imported_begin() const { return shared_begin();}
  int imported_end()   const { return copied_end();}
  range_ref   imported() const 
    { return range_ref(imported_begin(),imported_end(), the_range, *the_grid);}


  // all of the overlap
  range_ref whole_overlap() const 
    { return range_ref(exported_begin(),copied_end(), the_range, *the_grid);}


};

//----------------------------------------------------------------
//   [2a]   void write_rge<EltRange>
//----------------------------------------------------------------

template<class EltRange>
void write_rge(EltRange const& rge, ostream & out);

//----------------------------------------------------------------
//   [2b]   void write_ovrge<OVRange>
//----------------------------------------------------------------

template<class OVRange>
void write_ovrge(OVRange const& rnge, ostream& out);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Distributed/generic/range-layers.C"
#endif


#endif
