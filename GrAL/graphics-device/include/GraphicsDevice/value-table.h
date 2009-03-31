#ifndef NMWR_GB_GRAPHICSDEVICE_VALUE_TABLE_H
#define NMWR_GB_GRAPHICSDEVICE_VALUE_TABLE_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <map>
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>

namespace GrAL {

template<class KEY>
class generic_value {
public:
  virtual ~generic_value() {}
  virtual boost::shared_ptr<generic_value<KEY> > getval()       const         = 0;
  virtual void           setval(boost::shared_ptr<generic_value<KEY> const>)  = 0;
  virtual KEY key() const = 0;
  virtual void print(std::ostream& out) const = 0;
};


template<class T, class KEY>
class typed_value : public generic_value<KEY> {
private:
 T t;
public:
  typed_value(const T& tt) : t(tt) {}
  boost::shared_ptr<generic_value<KEY> > getval() const
  {
    return boost::shared_ptr<generic_value<KEY> >(new typed_value<T,KEY>(t));
  }

  void setval(boost::shared_ptr<generic_value<KEY> const> v) { 
    t= (dynamic_cast<typed_value<T,KEY> const&>(*v)).t;
  }
  void print(std::ostream& out) const {t.print(out);}
  T    Value() const {return(t);}
  KEY  key  () const {return(t.key());}
};



// this is basically a dictionary that maps strings
// to placeholders for a variable reference.
// pointers to the abstract base class are used in order to profit
// from polymorphism (derived classes are automatically
// generated via templates in class TypedMutator).

template<class KEY>
class ValueTable {
private:
  //typedef leda_d_array<KEY, const generic_value<KEY> *>  table_type;
  typedef std::map<KEY, boost::shared_ptr<const generic_value<KEY> >, std::less<KEY> >  table_type;
  table_type table;
  int entries;
public:
  //ValueTable()  : table((generic_value<KEY> *)nil), entries(0) {}
  ValueTable()  : table(), entries(0) {}
  bool IsEmpty() const { return (entries == 0);}
  void AddVal(boost::shared_ptr<const generic_value<KEY> > m)
  {
    //if( ! table.defined(m->key()))
    if( table.find(m->key())==table.end() )
	entries++;
    table[m->key()]=m->getval(); 
  } 
  bool GetVal(boost::shared_ptr<generic_value<KEY> > m)  const
  {
    //if (! table.defined(m->key()))
    if( table.find(m->key())==table.end() )
      return false;
    else {
      //  m->setval(table[m->key()]->getval());
      boost::shared_ptr<const generic_value<KEY> > entry = (*(table.find(m->key()))).second;
      m->setval(entry->getval());
      return true;
    }
  };
};


// this function is necessary, because member fct's ValueTable::AddVal /SetVal
// cannot be  template functions (in most compilers)

template<class T, class KEY> 
inline void AddVal(ValueTable<KEY> & VT, const T& v)
{  VT.AddVal(boost::shared_ptr<generic_value<KEY> >(new typed_value<T,KEY>(v)));}

template<class T, class KEY> 
inline bool GetVal(const ValueTable<KEY>& VT,  T& v)
{
  typed_value<T,KEY> * pv = new typed_value<T,KEY>(v);
  boost::shared_ptr<generic_value<KEY> > res(pv);
  bool done =  VT.GetVal(res);
  v = pv->Value();
  return done;
}

} // namespace GrAL 

#endif
