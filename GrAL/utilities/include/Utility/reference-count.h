#ifndef REFERENCE_COUNTING_TEMPLATES_H
#define REFERENCE_COUNTING_TEMPLATES_H

// from Scott Meyers "More Effective C++"

#include "Utility/copy-traits.h"

/******************************************************************************
*                       Class RCObject                                        *
******************************************************************************/


class RCObject {                       // base class for reference-
public:                                // counted objects
  void addReference()      { ++refCount;}
  void removeReference()   { if (--refCount == 0) delete this;} 
  // ruft den destruktor der abgeleiteten        ----^
  // Klasse auf (wegen des virtuellen destruktors !)
  void markUnshareable()   {  shareable = false;}
  bool isShareable() const {  return shareable;}
  bool isShared()    const {  return (refCount > 1);}

protected:
  RCObject()                    : refCount(0), shareable(true) {}
  RCObject(const RCObject&) : refCount(0), shareable(true) {}
  RCObject& operator=(const RCObject&) { return *this;}  
  virtual ~RCObject() {} // virtual ist essentiell !

private:
  int  refCount;
  bool shareable;
};



/******************************************************************************
*                 Template Class RCPtr                                        *
******************************************************************************/

template<class T>                      // template class for smart
class RCPtr {                          // pointers-to-T objects; T
                                       // must support the RCObject interface
public:                             
  RCPtr(T* realPtr = 0)   : pointee(realPtr)     { init(); }
  RCPtr(const RCPtr& rhs) : pointee(rhs.pointee) { init(); }

  ~RCPtr() {  if (pointee) pointee->removeReference();}
  RCPtr& operator=(const RCPtr& rhs);                     
  T* operator->() const {  return  pointee;}
  T& operator*()  const {  return *pointee;}

private:
  T *pointee;
  void init();
};

template<class T>
inline void RCPtr<T>::init()
{
  if (pointee == 0) return;
  
  if (pointee->isShareable() == false) {
    //   pointee = new T(*pointee);
    pointee = copy_traits<T>::clone(*pointee);
  }
  pointee->addReference();
}


template<class T>
inline RCPtr<T>& RCPtr<T>::operator=(const RCPtr<T>& rhs)
{
  if (pointee != rhs.pointee) {                   
    if (pointee) 
      pointee->removeReference();                
    pointee = rhs.pointee;
    init(); 
  }
  return *this;
}


// macht eine Kopie bei nicht-const-Zugriff (moegliche Veraenderung)

template<class T>
class RCIPtr {
public:
  RCIPtr(T* realPtr = 0);
  // RCIPtr<T>& operator=(T* realPtr); // G. Berti
  RCIPtr(const RCIPtr<T>& rhs) : counter(rhs.counter) {init();}
  ~RCIPtr()                 { counter->removeReference();}
  RCIPtr<T>& operator=(const RCIPtr<T>& rhs);

        T* operator->()        { makeCopy();  return  (counter->pointee);}
  const T* operator->() const  {              return  (counter->pointee);}
        T& operator*()         { makeCopy();  return *(counter->pointee);}       
  const T& operator*()  const  {              return *(counter->pointee);}

private:
  struct CountHolder: public RCObject {
    ~CountHolder() { delete pointee; }
    T *pointee;
  };
  
  CountHolder *counter;
  void init();
  void makeCopy();                          
};

template<class T>
inline void RCIPtr<T>::init()                      
{                                           
  if (counter->isShareable() == false) { // wie sollte dies eintreten ??
    T *oldValue = counter->pointee;         
    counter = new CountHolder;
    //counter->pointee = new T(*oldValue);    
    counter->pointee = copy_traits<T>::clone(*oldValue);
  }
  counter->addReference();
}

template<class T>
inline RCIPtr<T>::RCIPtr(T* realPtr)
: counter(new CountHolder)
{ 
  counter->pointee = realPtr;
  init();
}


/* THIS CRASHES!
template<class T>
inline RCIPtr<T>&  RCIPtr<T>::operator=(T* realPtr)
{ 
  cerr << "RCIPtr<T>::operator=(T* realPtr)\n";
  counter->removeReference(); // deletes when refcount hits 0, but counter is still used!!
  counter->pointee = realPtr;
  init();
  return *this;
}
*/

template<class T>
inline RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr<T>& rhs)
{
  if (counter != rhs.counter) {                   
    counter->removeReference();            
    counter = rhs.counter;
    init();
  }
  
  return *this;
}

template<class T>                             
inline void RCIPtr<T>::makeCopy()                    
{                                             
  if (counter->isShared()) {                  
    T *oldValue = counter->pointee;           
    counter->removeReference();               
    counter = new CountHolder;
    //    counter->pointee = new T(*oldValue);
    counter->pointee = copy_traits<T>::clone(*oldValue);
    counter->addReference();
  }
}


#endif
