#ifndef NMWR_GB_GRAPHICSDEVICE_VALUE_TABLE_H
#define NMWR_GB_GRAPHICSDEVICE_VALUE_TABLE_H

// $LICENSE


#include <map>
#include <iostream>
#include <string>

template<class KEY>
class generic_value {
public:
  virtual generic_value<KEY>* getval()       const         = 0;
  virtual void           setval(const generic_value<KEY>*) = 0;
  virtual KEY key() const = 0;
  virtual void print(std::ostream& out) const = 0;
};


template<class T, class KEY>
class typed_value : public generic_value<KEY> {
private:
 T t;
public:
 typed_value(const T& tt) : t(tt) {}
 generic_value<KEY>* getval() const
{
  generic_value<KEY>* r = new typed_value<T,KEY>(t);
  return (r);
}

 void                setval(const generic_value<KEY>* v) { t=((typed_value<T,KEY>*)v)->t;} 
 void print(std::ostream& out) const {t.print(out);}
 T   Value() const {return(t);}
 KEY   key() const {return(t.key());}
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
  typedef std::map<KEY, const generic_value<KEY> *,std::less<KEY> >  table_type;
  table_type table;
  int entries;
public:
  //ValueTable()  : table((generic_value<KEY> *)nil), entries(0) {}
  ValueTable()  : table(), entries(0) {}
  bool IsEmpty() const { return (entries == 0);}
  void AddVal(const generic_value<KEY> * m)
  {
    //if( ! table.defined(m->key()))
    if( table.find(m->key())==table.end() )
	entries++;
    table[m->key()]=m->getval(); 
  } 
  bool GetVal(generic_value<KEY> * m)  const
  {
    //if (! table.defined(m->key()))
    if( table.find(m->key())==table.end() )
      return false;
    else{
    //  m->setval(table[m->key()]->getval());
    const generic_value<KEY> * entry = (*(table.find(m->key()))).second;
     m->setval(entry->getval());
      return true;
    }
  };
};


// this function is necessary, because member fct's ValueTable::AddVal /SetVal
// cannot be  template functions (in most compilers)

template<class T, class KEY> 
inline void AddVal(ValueTable<KEY> & VT, const T& v)
{  VT.AddVal(new typed_value<T,KEY>(v));}

template<class T, class KEY> 
inline bool GetVal(const ValueTable<KEY>& VT,  T& v)
{
  typed_value<T,KEY>* res = new typed_value<T,KEY>(v);
  bool done =  VT.GetVal(res);
  v = res->Value();
  return done;
}

#endif
