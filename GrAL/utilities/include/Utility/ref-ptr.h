#ifndef GRAL_GB_UTILITY_REF_PTR_H
#define GRAL_GB_UTILITY_REF_PTR_H

// $LICENSE_NEC

/*! \brief class for disallowing taking the address of temporary.
    \author Guntram Berti

    The intent of this class is to forbid taking the address of temporary objects
    returned from function calls (see docs of ref_ptr for an example).
    This class is best used in conjunction with \c ref_ptr.

    A \c temporary<T> should ideally behave like a \c T.
    Unfortunately, this is not possible, because we cannot overload
    \c operator. (member access).
    Therefore, the corresponding pointer operators were overloaded.
    The address-of \c operator& was made private to forbid obtaining the
    address of the temporary object.
*/

template<class T>
class temporary {
  typedef temporary<T> self;
  T t;
  self* operator&() { return this;}
public:
  explicit temporary(T tt) : t(tt) {}

  // make temporary<T> behave like T const& / T const* in almost all situations ...
  // ...except that we cannot take the address of it.
  // because we cannot overload operator., this conversion does not normally do the trick.
  operator T const& () const { return t;}
  T const& value() const { return t;}
  // must rely on pointer interface
  T const& operator*()  const { return t;}
  T const* operator->() const { return &t;}
};


struct  ref_ptr_base {
  enum ownership { not_owned, excl_owned};
};

/*! \brief class for maintaining a assignable reference to another (often const) object 
    \author Guntram Berti
    
   The purpose of this class is to maintain a reference (pointer) to an object
   which may by allocated statically or dynamically. 
   The motivating observation is the following: When referencing  another object (say \c b of class \c B)
   with a member of an object \c a of class \c A, the allocation policy and life time of \c b
   may fall into different cases:
   -# \c b is statically allocated, but its lifetime is at least as long than that of \c a:
      \code
      Grid g;
      VertexIterator v(g); // a <-> v, and b <-> g
      \endcode
   -# \c b is statically allocated on the fly, i.e. a temporary, for example a light-weight
     object returned by a function:
      \code
       Vertex v(* g.FirstVertex());
       boundary_type bd(star(v));  // a <-> bd, and b <-> result of star(v)
       cout << bd.NumOfVertices(); // Danger, must not take just take address of temporary result star(v)
      \endcode
      (Note: \c star(v) is the set of elements incident to a vertex \c v. This is most likely
       to be calculated on-the-fly.)
   -# \c b is dynamically allocated and manually managed, 
     but the pointer to it has life time at least as long than that of \c a:
      \code
       Grid * g = new Grid(nv,nc);
       // ...
       VertexIterator v(g); // a <-> v, and b <-> g
      \endcode
   -# \c b is dynamically allocated and managed by a smart pointer with sharing semantics:
      \code
      Grid g;
      subrange cells_to_remove(g); 
      // ...
      view2_type v2(cut_cells(g,cells_to_remove)); // a <-> v2, and b <-> result of cut_cells
      \endcode
      Here, \c cut_cells() is likely to return a dynamically allocated object, because it may be large. 
      Of course, in this case, a smart pointer is the method of choice.

   Items 1 and 3 are essentially the same, these can be handled by just storing the address of \c b,
   and there is no need to worry on life time of \c b in \c a. <BR>
   Item 2 is different because the lifetime of \b is to short. In this case we have to create a permanent
   copy. However, when a longer lifetime of b is not needed, we do not want to incur the overhead of
    a dynamic allocation. <BR>
   Item 4 means that we can simply copy the shared smart pointer. 


   The challenge is now to put everything under the hood of a unique pointer-like interface,
   and to use the constructor to differentiate between the cases.

   In case 1 and 3, we just pass a plain reference or pointer to the object to the
   constructor of \c ref_ptr<>. <br>
   For filtering out case 2, we rely on the temporary object being wrappend into
   the \c temporary<> template. <br>
   Case 4 is not yet supported.

  \todo Extend the class to be able to hold instances of true reference-counted smart pointers.
*/

template<class T>
class ref_ptr : public ref_ptr_base {
  typedef ref_ptr<T> self;
  T    * ptr;
  bool owned_;
public:


  ref_ptr() : owned_(false)   { ptr=0;}
  // Cannot overload for T const& and T. So caller must make sure that temporary<T> is passed instead of T.
  // If we disable this, at least the caller must apply the address-of operator&
  // which could inhibit some misuse.
  explicit ref_ptr(T  &     t) : owned_(false) 
  { ptr = &t; } // std::cout << "ref_ptr(T const&     t)" << std::endl;}

  // FIXME: This might result in memory leak if called with newly allocated ptr and own=false
  // Perhaps add shared_ptr<T> to data and allow only ref_ptr(shared_ptr<T>) ?
  explicit ref_ptr(T *   tptr, ownership own = not_owned) : owned_( (own == not_owned ? false : true))
  { ptr = tptr; }
  // explicit 
  ref_ptr(temporary<T> t) : owned_(true)  
  { ptr = new T(t.value()); } // std::cout << "ref_ptr(temporary<T> t)" << std::endl;} 

  template<class U>
  explicit
  ref_ptr(temporary<U> t) : owned_(true)  
  { ptr = new T(t.value()); } // std::cout << "ref_ptr(temporary<T> t)" << std::endl;} 

  // copy constructors and assigment
  ref_ptr(ref_ptr<T> const& rhs) { copy(rhs);}
  ref_ptr<T> & operator=(ref_ptr<T> const& rhs) { 
    if(this != &rhs) {
      clear();
      copy(rhs);
    }
    return *this;
  }

  // conversion constructors and assigment
  template<class U>
  ref_ptr(ref_ptr<U> const& rhs) { copy(rhs);}
  template<class U>
  ref_ptr<T> & operator=(ref_ptr<U> const& rhs) { 
    // no need to check for equality: this would be caught by copy assigment above  
   // if(this != &rhs) {
      clear();
      copy(rhs);
      // }
    return *this;
  }

  template<class U>
  void make_own(U * rhs) 
  {
    clear();
    ptr = rhs; // new T(*rhs);
    owned_ = true;
  }
  void make_copy()
  {
    if(ptr != 0 && !owned_) {
      owned_ = true;
      T * ptr2 = new T(*ptr);
      ptr = ptr2;
    }
  }

  template<class U>
  void make_ref   (U * rhs)
  {
    clear();
    ptr = rhs;
    owned = false;
  }
  // void make_shared(T * rhs);


  template<class U>
  ref_ptr<T> & operator=(U const* tptr) { clear(); owned_ = false; ptr =tptr; return *this;}

  //  template<class U>
  //  ref_ptr<U> get_ref() const { return ref_ptr<U>(this->operator*());}

  //  ref_ptr<T> get_ref() const { return ref_ptr<T>(this->operator*());}

  //  template<class U = T>
  // ref_ptr<U> get_copy() {}

  ~ref_ptr() { clear();}

  // validity check
  bool valid() const { return ptr != 0;}
  bool owned() const { return owned_;}

  friend bool operator==(self const& c, T         * p) { return c.ptr == p;}
  friend bool operator==(T         * p, self const& c) { return c.ptr == p;}
  friend bool operator!=(self const& c, T         * p) { return c.ptr != p;}
  friend bool operator!=(T         * p, self const& c) { return c.ptr != p;}
private:
  template<class U>
  void copy(ref_ptr<U> const& rhs) { 
    /*
    owned_ = false;
    ptr = rhs.get();
    */
    owned_ = rhs.owned();
    if(owned_)
      ptr= new T(*rhs);
    else 
      ptr = rhs.get();
  }
  void clear()  { if(owned_) delete ptr; ptr = 0;}
public:
  T * get()        const { return ptr;}
  T * operator->() const { return ptr;} 
  T & operator* () const { return *ptr;}
};


template<class T>
inline ref_ptr<T> make_ref_ptr(temporary<T> t) { return ref_ptr<T>(t);}

#endif
