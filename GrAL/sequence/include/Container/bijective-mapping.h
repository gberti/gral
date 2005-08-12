#ifndef NMWR_GB_BIJECTIVE_MAPPING_H
#define NMWR_GB_BIJECTIVE_MAPPING_H


// $LICENSE


#include "Container/my-hash-map.h" 
#include "Container/mapped-value-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {


/*! \defgroup bijectivemapping Representations for one-to-one mappings 

    \ingroup mappings

  \b Contents

  This module contains class templates for representing bijective mappings:
   - class \ref bijective_mapping<T1,T2,Policy> : A one-to-one mapping (short f)
   - class \ref inverse_mapping<T1,T2> : Inverse of a bijective_mapping (short inv)
   - class \ref domain_of_bijective_mapping<T1,T2>: 
     domain of definition of  a  bijective mapping (short dom)
   - class \ref range_of_bijective_mapping<T1,T2>: 
       range (image) of a bijective mapping. (short ran)
   - some helper classes and functions for I/O: 
     \ref read_bm, \ref write_bm, \ref printer_of_bij_mapping, \ref Printer

     The extra \c Policy parameter (which has been omitted for clarity except in the first
     occurence) permits to control the behaviour of bijective_mapping if evaluated for an item
     which has not been set explictly. If \c Policy is \c fail_if_undefined  (the default),
     than accessing an undefined item is treated as an error.
     If  \c  Policy is \c identity_if_undefined  and <code> T1 == T2 </code>,
     then the item itself is returned. Thus, simular to partial_mapping,
     evaluation on an unbounded set of items is possible.
     However, this policy leads to possibly undetected violations of bijectivity,
     which would otherwise be detected when the inverse is built. 
     For instance, if we say <code> m[1] = 2 </code>, the mapping \c m is \e not bijective any longer
     because <code> m(1) == 2 || m(2) == 2 </code>, the latter by the implicit identity extension.

     \b Template \b parameters

   (same for bijective_mapping<T1,T2>, inverse_mapping<T1,T2>,
     domain_of_bijective_mapping<T1,T2>, range_of_bijective_mapping<T1,T2>)
  - T1 is Hashable, that is,  <TT> hash<T1> </TT> is defined.
  - T1 is Equality Comparable, that is, <TT> equal_to<T1> </TT> is defined.
  - T2 is Assignable and Default Constructible.


  \b Description:

  - class bijective_mapping<T1,T2>;
      - the master class with value semantics, 
        giving an interface to a mapping T1 -> T2
      - Template parameters:
         - T1, T2: hash<T> must be defined. (default definitions in STL only for integral types)
      - class printer_of_bij_mapping<T1,T2> is used for output
      - functions \ref read_bm, \ref write_bm are used for I/O
  - The other 3 classes have reference semantics with respect to bm:
    - class inverse_mapping<T1,T2> 
       - gives a function  interface to the inverse 
         (which is really calculated and stored in bm<T1,T2>)
    - class domain_of_bijective_mapping<T1,T2> (dom)
       - gives a set interface to the domain (of definition) of bm;
    - class range_of_bijective_mapping<T1,T2> (ran)
       - gives a set interface to the range (or image) of bm;

  \b Invariants:
  - T1 x: inv(f(x)) == x 
  - T2 y: f(inv(y)) == y
  - T1 x: f.defined(x) <=> dom(f).is_member(x) 
  - T2 y: inv.defined(y) <=> ran(f).is_member(y)
  - dom(f).size() == ran(f).size()
  - f(* (dom(f).begin() + n) == * (ran(f).begin() + n), \f$ 0 \leq n \leq |\dom(f)| \f$
  
  \b Example:
    - see test-bijective.C

  \see bijective-mapping.h
*/

/*! \file
  \brief  Classes for representing bijective mappings of finite sets.
*/


  /*! \brief Policy to fail when undefined item is queried

     \ingroup bijectivemapping

     This class can serve as third (\c Policy) parameter to bijective_mapping.
  */
  struct fail_if_undefined {};

   /*! \brief Policy to return identity when undefined item is queried

     \ingroup bijectivemapping

     This class can serve as third (\c Policy) parameter to bijective_mapping.
  */
  struct identity_if_undefined {};


  template<class T1, class T2, class Policy = fail_if_undefined>
  class bijective_mapping;

  template<class T1, class T2, class Policy = fail_if_undefined>
  class inverse_mapping;

  template<class T1, class T2, class Policy = fail_if_undefined>
  class domain_of_bijective_mapping;

  template<class T1, class T2, class Policy = fail_if_undefined>
  class range_of_bijective_mapping;

//----------------------------------------------------------------
//          [0] output facilities
//----------------------------------------------------------------

/*! \brief write a bijective map to ostream
    \ingroup bijectivemapping 
 */
  template<class T1, class T2,  class Policy>
void write_bm(bijective_mapping<T1,T2,Policy> const& m, ::std::ostream& out);

/*! \brief read a bijective map from istream
    \ingroup bijectivemapping 
 */
  template<class T1, class T2, class Policy>
  void read_bm(bijective_mapping<T1,T2,Policy>       & m, ::std::istream& in);

/*! \brief helper class for custom output of  a bijective map 
    \ingroup bijectivemapping 
 */
template<class T1, class T2, class Policy>
class printer_of_bij_mapping {
private:
  bijective_mapping<T1,T2,Policy> const* mp;
public:
  printer_of_bij_mapping(bijective_mapping<T1,T2,Policy> const& m) : mp(&m) {}

  friend ::std::ostream& operator<<(::std::ostream& out, printer_of_bij_mapping<T1,T2,Policy> const& p)
    { write_bm(*(p.mp),out); return out;}

};

/*! \brief helper function for custom output of  a bijective map 

    Usage:
    \verbatim 
    bijective_mapping<int,int> bm;
     ...
    cout << Printer(bm) << endl;
    \endverbatim

    \ingroup bijectivemapping  
*/
  template<class T1, class T2, class Policy>
inline printer_of_bij_mapping<T1,T2,Policy>
Printer(bijective_mapping<T1,T2,Policy> const& m)
{ return printer_of_bij_mapping<T1,T2,Policy>(m);}



  template<class T1, class T2, class MAP, class Policy> 
  struct map_accessor 
  {
    static T2 const& get(MAP const& m, T1 const& t1) { 
      REQUIRE(m.find(t1) != m.end(), "", 1);
      return m.find(t1)->second;
    }
  };

  template<class T1, class MAP> 
  struct map_accessor<T1,T1,MAP,identity_if_undefined>  
  {
    static T1 const& get(MAP const& m, T1 const& t1) { 
      return (m.find(t1) == m.end() ? t1 : m.find(t1)->second);
    }
  };

//----------------------------------------------------------------
//          [1] class bijective_mapping<T1,T2>
//----------------------------------------------------------------

/*! \brief Represents a one-to-one mapping T1 -> T2
    \ingroup bijectivemapping
    \see bijectivemapping, bijective-mapping.h
*/
  template<class T1, class T2, class Policy>
  class bijective_mapping {
public:
  typedef inverse_mapping<T1,T2,Policy>              inverse_type;
  typedef domain_of_bijective_mapping<T1,T2,Policy>  domain_type;
  typedef range_of_bijective_mapping <T1,T2,Policy>  range_type;

  template<class U, class R, class P> friend class bijective_mapping;
  template<class U, class R, class P> friend class inverse_mapping;
  template<class U, class R, class P> friend class domain_of_bijective_mapping;
  template<class U, class R, class P> friend class range_of_bijective_mapping;
  
  // STL unary function conformance
  typedef T1                         argument_type;
  typedef T2                         result_type;

private:
  // hash_map not yet in ::std::
  typedef STDHASH::hash_map<T1,T2>  map_table_type;
  typedef STDHASH::hash_map<T2,T1>  inv_table_type; 
  typedef map_accessor<T1,T2,map_table_type,Policy>  map_accessor_type;
  typedef map_accessor<T2,T1,inv_table_type,Policy>  inv_accessor_type;
  //--------------- DATA -------------------------

  map_table_type         the_map;
  mutable inv_table_type the_inverse_map;

  // flag if the_inverse_map really reflects the inverse mapping
  mutable bool                       inverse_ok;

  //-------  internal types ----------------

  typedef typename map_table_type::const_iterator map_iterator;
public:
    typedef bijective_mapping<T1,T2,Policy>   self;
  
  //---------- construction ----------------

  //! Empty mapping, \f$ \dom(f) = \emptyset \f$  
  bijective_mapping() : inverse_ok(false) {}
  //! Empty mapping, \f$ \dom(f) = \emptyset \f$, internal storage allocated  
  bijective_mapping(unsigned sz) : the_map(sz), the_inverse_map(sz), inverse_ok(false) {}
  //! \f$ \dom(f) = \dom(inv), f(x) = inv(x) \, \forall x \in \dom(inv) \f$
    bijective_mapping(inverse_mapping<T2,T1,Policy> const& inv);

  //---------- data access -----------------

  //! returns \f$ f(t_1) \f$,  Pre: \f$ t_1 \in \dom(f) \f$
  const T2& operator()(const T1& t1) const {
    /*
    REQUIRE( (the_map.find(t1) != the_map.end()), 
	     "map not defined for item " << t1 << '\n',1);
    return (*(the_map.find(t1))).second;
    */
    return map_accessor_type::get(the_map,t1);
  }
  //! returns \$f f(t_1) \$f,  Pre: \f$ t_1 \in \dom(f) \f$
  const T2& operator[](const T1& t1) const {
    /*
    REQUIRE( (the_map.find(t1) != the_map.end()), 
	     "map not defined for item " << t1 << '\n',1);
    return (*(the_map.find(t1))).second;
    */
    return map_accessor_type::get(the_map,t1);
  }
  //! returns \f$ f(t_1) \f$, else an uninitialized value.
  T2& operator[](const T1& t1) { inverse_ok = false; return the_map[t1];}

  //! returns true iff \f$ t_1 \in \dom(f) \f$ 
  bool defined  (const T1& t1) const { return (the_map.find(t1) != the_map.end());}
  //! returns true iff \f$ t_1 \notin \dom(f) \f$ 
  bool undefined(const T1& t1) const { return !defined(t1); }


  //-------------- related functions and sets ----------------

  //! returns the  inverse mapping \f$ f^{-1} \f$
  inverse_type inverse() const;

  //! returns the domain of definition \f$ \dom(f) \f$
  domain_type domain() const;
  //! returns the range (image) \f$ f(\dom(f)) \f$
  range_type  range()  const;

  typedef typename map_table_type::size_type  size_type;
  //! returns \f$ |\dom(f)| \f$
  size_type size() const { return domain().size();}


private:
  //--------------- consistency -------------------------------

  void update_inverse() const {
    if(! inverse_ok) {
      calculate_inverse();
      inverse_ok = true;
    }

  }
  void calculate_inverse() const;
};


//-----------------------------------------------------------------
//          [2]  class inverse_mapping<T1,T2>
//-----------------------------------------------------------------

/*! \brief Represents a  mapping T2 --> T1
    which is the inverse of some \ref bijective_mapping<T1,T2>.
    \ingroup bijectivemapping

   This class has reference semantics with respect to
   bijective_mapping.

    \see bijectivemapping, bijective-mapping.h
*/
  template<class T1, class T2, class Policy>
class inverse_mapping {
private:
  typedef bijective_mapping<T1,T2, Policy>   mapping_type;
  typedef typename mapping_type::inv_accessor_type accessor_type;
  template<class U, class R, class P> friend class bijective_mapping;

  //-------- DATA ----------
  const  mapping_type*    bmap; // reference

public:
  typedef T1 result_type;
  typedef T2 argument_type;

  inverse_mapping(const mapping_type& tm) : bmap(&tm) {}

  const T1& operator()(const T2& t2) const { 
    bmap->update_inverse();
    REQUIRE( (defined(t2)),
	    "inverse map not defined for item " << t2 << '\n',1);
    return (*(bmap->the_inverse_map.find(t2))).second;
    return accessor_type::get(bmap->the_inverse_map, t2);
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

/*! \brief Represents the domain of definition of a mapping.
    \ingroup bijectivemapping

 This class allows STL-style iteration over the items of type T1.


 This class has reference semantics with respect to
 bijective_mapping.

 Notation:
 - \f$ f \f$ is the underlying mapping
 - \f$ \dom(f) \f$ is the domain of \f$ f \f$

 \see bijectivemapping, bijective-mapping.h
 */
  template<class T1, class T2, class Policy>
class domain_of_bijective_mapping {
public:
  typedef bijective_mapping<T1,T2, Policy>                 mapping_type;
  typedef typename mapping_type::map_table_type            map_table_type;
  typedef typename map_table_type::value_type              base_value_type;
  typedef typename map_table_type::const_iterator          base_iter_type;
  typedef mapped_value_const_iterator<base_iter_type,
				      get_first<base_value_type> const> const_iterator;

  typedef T1                                                value_type;

private:
  const mapping_type*  bmap; // reference to mapping f
public:
  domain_of_bijective_mapping(const mapping_type& tm) : bmap(&tm) {}

  unsigned size() const { return bmap->the_map.size();}

  //! true if \f$ x \in \dom(f) \f$ (domain of underlying mapping \f$ f \f$)
  bool is_member(const value_type& x) const { return bmap->defined(x);}  

  const_iterator begin() const { return const_iterator(bmap->the_map.begin());}
  const_iterator end()   const { return const_iterator(bmap->the_map.end());}
  T1 const& front() const { return *begin();}
};

//-----------------------------------------------------------------
//          [4]   class range_of_bijective_mapping<T1,T2>
//-----------------------------------------------------------------

/*! \brief 
     Represents the range (image) of a bijective mapping \f$ f \f$.
    \ingroup bijectivemapping

     This class allows STL-style iteration over the items of type T2.

    This class has reference semantics with respect to
    bijective_mapping.

    \see bijectivemapping, bijective-mapping.h
 */
  template<class T1, class T2, class Policy>
class range_of_bijective_mapping {
public:
  typedef bijective_mapping<T1,T2, Policy>                 mapping_type;
  typedef typename mapping_type::map_table_type            map_table_type;
  typedef typename map_table_type::value_type              base_value_type;
  typedef typename map_table_type::const_iterator          base_iter_type;
  typedef mapped_value_const_iterator<base_iter_type,
                                      get_second<base_value_type> const> const_iterator;

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


  template<class T1, class T2, class Policy>
inline
  inverse_mapping<T1,T2, Policy> bijective_mapping<T1,T2, Policy>::inverse() const 
{ return inverse_type(*this);}

template<class T1, class T2, class Policy>
inline
domain_of_bijective_mapping<T1,T2,Policy> bijective_mapping<T1,T2,Policy >::domain() const 
{ return domain_type(*this);}
 
template<class T1, class T2, class Policy>
inline
range_of_bijective_mapping<T1,T2,Policy> bijective_mapping<T1,T2,Policy>::range() const 
{ return range_type(*this);}

template<class T1, class T2, class Policy>
inline
//domain_of_bijective_mapping<T1,T2>
typename inverse_mapping<T1,T2,Policy>::range_type
inverse_mapping<T1,T2,Policy>::range() const { return bmap->domain();}

template<class T1, class T2, class Policy>
inline
typename inverse_mapping<T1,T2,Policy>::domain_type //range_of_bijective_mapping<T1,T2>
inverse_mapping<T1,T2,Policy>::domain() const { return bmap->range();}


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "bijective-mapping.C"
#endif

#endif
