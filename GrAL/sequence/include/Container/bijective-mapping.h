#ifndef NMWR_GB_BIJECTIVE_MAPPING_H
#define NMWR_GB_BIJECTIVE_MAPPING_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

// namespace sgistl {
#include <hash_map.h> // STL
//}
//using namespace std;
// using sgistl::hash;
// using sgistl::hash_map;


#include "Container/mapped-value-iterator.h"
#include "Utility/pre-post-conditions.h"

//----------------------------------------------------------------
/*!
  \brief  Classes for representing bijective mappings of finite sets.

  Contents:
  ---------
  [1] class bijective_mapping<T1,T2>;           (short bm)
  [2] class inverse_mapping<T1,T2>;             (short im)
  [3] class domain_of_bijective_mapping<T1,T2>; (short dom)
  [4] class range_of_bijective_mapping<T1,T2>;  (short ran)


  Description:
  ------------
  [1] class bijective_mapping<T1,T2>;
      - the master class with value semantics, 
        giving an interface to a mapping T1 -> T2
      CONSTRAINTS on T1, T2:
      - hash<Ti> must be defined. (default definitions in STL only for integral types)
  [2-4] The other 3 classes have reference semantics w/r to bm:

  [2] class inverse_mapping<T1,T2> 
       - gives a function  interface to the inverse 
         (which is really calculated and stored in bm<T1,T2>)
  [3] class domain_of_bijective_mapping<T1,T2> (dom)
       - gives a set interface to the domain (of definition) of bm;
  [4] class range_of_bijective_mapping<T1,T2> (ran)
       - gives a set interface to the range (or image) of bm;

  The following INVARIANTS hold:
  - T1 x: im(bm(x)) == x 
  - T2 y: bm(im(y)) == y
  - T1 x: bm.defined(x) <=> dom(bm).is_member(x) 
  - T2 y: im.defined(y) <=> ran(bm).is_member(y)
  - dom(bm).size() == ran(bm).size()

*/
//----------------------------------------------------------------

template<class T1, class T2>
class bijective_mapping;

template<class T1, class T2>
class inverse_mapping;

template<class T1, class T2>
class domain_of_bijective_mapping;

template<class T1, class T2>
class range_of_bijective_mapping;

//----------------------------------------------------------------
//          [0] output facilities
//----------------------------------------------------------------

template<class T1, class T2>
void write_bm(bijective_mapping<T1,T2> const& m, ostream& out);

template<class T1, class T2>
void read_bm(bijective_mapping<T1,T2>       & m, istream& in);

template<class T1, class T2>
class printer_of_bij_mapping {
private:
  bijective_mapping<T1,T2> const* mp;
public:
  printer_of_bij_mapping(bijective_mapping<T1,T2> const& m) : mp(&m) {}

  friend ostream& operator<<(ostream& out, printer_of_bij_mapping<T1,T2> const& p)
    { write_bm(*(p.mp),out); return out;}

};

template<class T1, class T2>
inline printer_of_bij_mapping<T1,T2>
Printer(bijective_mapping<T1,T2> const& m)
{ return printer_of_bij_mapping<T1,T2>(m);}

//----------------------------------------------------------------
//          [1] class bijective_mapping<T1,T2>
//----------------------------------------------------------------

template<class T1, class T2>
class bijective_mapping {
public:
  typedef inverse_mapping<T1,T2>              inverse_type;
  typedef domain_of_bijective_mapping<T1,T2>  domain_type;
  typedef range_of_bijective_mapping<T1,T2>   range_type;

  // friend class inverse_mapping<T1,T2>;
  // friend class domain_of_bijective_mapping<T1,T2>;
  // friend class range_of_bijective_mapping<T1,T2>;
  // friend class bijective_mapping<T2,T1>;
  // friend class inverse_mapping<T2,T1>;
  // STL unary function conformance
  typedef T1                         argument_type;
  typedef T2                         result_type;

  typedef hash_map<T1,T2, hash<T1>,equal_to<T1> >  map_table_type;
  typedef hash_map<T2,T1, hash<T2>,equal_to<T2> >  inv_table_type; 
  //--------------- DATA -------------------------
public:
  // get rid of !!! warnings of multiple friendship
  map_table_type         the_map;
  mutable inv_table_type the_inverse_map;
private:
  // flag if the_inverse_map really reflects the inverse mapping
  mutable bool                       inverse_ok;

  //-------  internal types ----------------

  typedef typename map_table_type::const_iterator map_iterator;
public:
  typedef bijective_mapping<T1,T2>   self;
  
  //---------- construction ----------------
  
  bijective_mapping() : inverse_ok(false) {}
  bijective_mapping(unsigned sz) : the_map(sz), the_inverse_map(sz), inverse_ok(false) {}
  bijective_mapping(const inverse_mapping<T2,T1> & inv);

  //---------- data access -----------------

  const T2& operator()(const T1& t1) const {
    REQUIRE( (the_map.find(t1) != the_map.end()), 
	     "map not defined for item " << t1 << '\n',1);
	     //	     << "  " << Printer(*this) << '\n',1);
    return (*(the_map.find(t1))).second;
  }

  const T2& operator[](const T1& t1) const {
    REQUIRE( (the_map.find(t1) != the_map.end()), 
	     "map not defined for item " << t1 << '\n',1);
	     //  << "  " << Printer(*this) << '\n',1);
    return (*(the_map.find(t1))).second;
  }

  T2& operator[](const T1& t1) { return the_map[t1];}

  bool defined(const T1& t1) const { return (the_map.find(t1) != the_map.end());}


  //-------------- related functions and sets ----------------

  // give inverse mapping
  inverse_type inverse() const;

  // access to sets domain of definition and range of image
  domain_type domain() const;
  range_type  range()  const;

  typedef typename map_table_type::size_type  size_type;
  size_type size() const { return domain().size();}

  //--------------- consistency -------------------------------

  void update_inverse() const {
    if(! inverse_ok) {
      calculate_inverse();
      (bool&)inverse_ok = true;
    }

  }
  void calculate_inverse() const;
};


//-----------------------------------------------------------------
//          [2]  class inverse_mapping<T1,T2>
//-----------------------------------------------------------------

// mapping signature: T2 --> T1
// just a reference to the real mapping.
template<class T1, class T2>
class inverse_mapping {
private:
  typedef bijective_mapping<T1,T2> mapping_type;
  // friend  class bijective_mapping<T1,T2>;
  //friend  class bijective_mapping<T2,T1>;

  //-------- DATA ----------
public:
  const  mapping_type*    bmap; // reference
public:
  inverse_mapping(const mapping_type& tm) : bmap(&tm) {}

  const T1& operator()(const T2& t2) const { 
    bmap->update_inverse();
    REQUIRE( (defined(t2)),
	    "inverse map not defined for item " << t2 << '\n',1);
    return (*(bmap->the_inverse_map.find(t2))).second;
  }

  bool defined(const T2& t2) const {
    bmap->update_inverse();
    return (bmap->the_inverse_map.find(t2) != bmap->the_inverse_map.end());
  }

  typedef typename mapping_type::domain_type range_type;
  inline range_type   range() const;
  typedef typename mapping_type::range_type  domain_type;
  inline domain_type  domain() const;
};

//-----------------------------------------------------------------
//           [3]  class domain_of_bijective_mapping<T1,T2>
//-----------------------------------------------------------------

template<class T1, class T2>
class domain_of_bijective_mapping {
public:
  typedef bijective_mapping<T1,T2>                         mapping_type;
  typedef typename mapping_type::map_table_type            map_table_type;
  typedef typename map_table_type::value_type              base_value_type;
  typedef typename map_table_type::const_iterator          base_iter_type;
  typedef mapped_value_const_iterator<base_iter_type,
                                      get_first<base_value_type> > const_iterator;

  typedef T1                                                    value_type;

private:
  const mapping_type*  bmap; // reference
public:
  domain_of_bijective_mapping(const mapping_type& tm) : bmap(&tm) {}

  unsigned size() const { return bmap->the_map.size();}
  bool is_member(const value_type& x) const { return bmap->defined(x);}  

  const_iterator begin() const { return const_iterator(bmap->the_map.begin());}
  const_iterator end()   const { return const_iterator(bmap->the_map.end());}
  
  T1 const& front() const { return *begin();}
};

//-----------------------------------------------------------------
//          [4]   class range_of_bijective_mapping<T1,T2>
//-----------------------------------------------------------------

template<class T1, class T2>
class range_of_bijective_mapping {
public:
  typedef bijective_mapping<T1,T2>                         mapping_type;
  typedef typename mapping_type::map_table_type            map_table_type;
  typedef typename map_table_type::value_type              base_value_type;
  typedef typename map_table_type::const_iterator          base_iter_type;
  typedef mapped_value_const_iterator<base_iter_type,
                                      get_second<base_value_type> > const_iterator;

  typedef T2                                                     value_type;

private:
  const mapping_type*  bmap; // reference
public:
  range_of_bijective_mapping(const mapping_type& tm)  : bmap(&tm) {}

  unsigned size() const { return bmap->the_map.size();}
  bool is_member(const value_type& x) const { 
    bmap->update_inverse();    
    return (bmap->the_inverse_map.find(x) != bmap->the_inverse_map.end());
  }  

  const_iterator begin() const { return const_iterator(bmap->the_map.begin());}
  const_iterator end()   const { return const_iterator(bmap->the_map.end());}
  T2 const& front() const { return *begin();}
};


//-----------------------------------------------------------------
//         inline functions of bijective_mapping<T1,T2>
//-----------------------------------------------------------------


template<class T1, class T2>
inline
inverse_mapping<T1,T2> bijective_mapping<T1,T2>::inverse() const 
{ return inverse_type(*this);}

template<class T1, class T2>
inline
domain_of_bijective_mapping<T1,T2> bijective_mapping<T1,T2>::domain() const 
{ return domain_type(*this);}
 
template<class T1, class T2>
inline
range_of_bijective_mapping<T1,T2> bijective_mapping<T1,T2>::range() const 
{ return range_type(*this);}

template<class T1, class T2>
inline
//domain_of_bijective_mapping<T1,T2>
inverse_mapping<T1,T2>::range_type
inverse_mapping<T1,T2>::range() const { return bmap->domain();}

template<class T1, class T2>
inline
inverse_mapping<T1,T2>::domain_type //range_of_bijective_mapping<T1,T2>
inverse_mapping<T1,T2>::domain() const { return bmap->range();}


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "bijective-mapping.C"
#endif

#endif
