#ifndef NMWR_GB_COPY_ON_WRITE_PTR_H
#define NMWR_GB_COPY_ON_WRITE_PTR_H

// $LICENSE




/*! \class copy_on_write_ptr 
    \brief A smart copy-on-write pointer.

   This class has been adapted from Scott Meyers "More Effective C++".
   The class can be used also for pointers to abstract classes,
   due to replacement of `new T' by copy_traits<T>::clone(T*).
*/

#include "Utility/copy-traits.h"


/*! \brief base class for reference counted objects

     \see copy_on_write_ptr
*/

class  counted_obj {                      
public:                               
  void operator++() { ++count;}
  void operator--() {
    if (--count == 0) 
      delete this; // calls d'tor of derived class (virtual d'tor!)
  } 
  bool is_shared() const { return (count > 1);}

protected:
  counted_obj() : count(0) {}
  virtual ~counted_obj() {} // virtual is essential, see op-- above!

private:
  unsigned  count;

  // Forbidden
  counted_obj(counted_obj const&);
  counted_obj& operator=(counted_obj const&);
};



/*!
 \todo: Should the non-const operator* / operator-> get different names,
 as to inhibit unneeded copying?

 */

template<class T>
class copy_on_write_ptr {
public:
  copy_on_write_ptr(T* pt = 0);
  copy_on_write_ptr(const copy_on_write_ptr<T>& rhs) 
    : counter(rhs.counter) {init();}
  copy_on_write_ptr<T>& operator=(const copy_on_write_ptr<T>& rhs);
  ~copy_on_write_ptr() { --(*counter); }

  const T* operator->() const  {          return  (counter->T_ptr);}
  const T& operator*()  const  {          return *(counter->T_ptr);}

        T& operator*()         { copy();  return *(counter->T_ptr);}       
        T* operator->()        { copy();  return  (counter->T_ptr);}

private:
  struct holder: public  counted_obj {
    ~holder() { copy_traits<T>::destroy(T_ptr); }
    T *T_ptr;
  };
  
  holder *counter;
  void init() { ++(*counter);}
  void copy();                          
};


template<class T>
inline copy_on_write_ptr<T>::copy_on_write_ptr(T* pt)
: counter(new holder)
{ 
  counter->T_ptr = pt;
  init();
}

template<class T>
inline copy_on_write_ptr<T>& 
copy_on_write_ptr<T>::operator=(const copy_on_write_ptr<T>& rhs)
{
  if (counter != rhs.counter) {                   
    --(*counter);
    counter = rhs.counter;
    init();
  }
  return *this;
}

template<class T>                             
inline void copy_on_write_ptr<T>::copy()                    
{ 
  if(counter->is_shared()) {                                            
    T * old_T_ptr = counter->T_ptr;           
    --(*counter);
    counter = new holder;
    counter->T_ptr = copy_traits<T>::clone(*old_T_ptr);
    ++(*counter);
  }
}


#endif
