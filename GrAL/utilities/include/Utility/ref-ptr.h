#ifndef GRAL_GB_UTILITY_REF_PTR_H
#define GRAL_GB_UTILITY_REF_PTR_H

// $LICENSE_NEC

#include <boost/shared_ptr.hpp>
#include "Utility/pre-post-conditions.h"


namespace GrAL {


  template<class T> class ref_ptr;

  /*! \brief Simplistic method for determining if a pointer resides on the stack
    
  \ingroup memory

     The implementation assumes a downward growing stack.
  */
  template<class T>
  inline bool is_stack_address(T * const& ptr)
  {
    // assuming downwards growing stack
    char i;
    unsigned long ptr_long = (unsigned long)ptr;
    unsigned long irf_long = (unsigned long)(&i);
    return ( ptr_long > irf_long ? true : false);
  }

  /*
  // Enable direct construction ref_ptr<const T>(ref_ptr<T>)
  // but this does not suffice to enable implicit conversion
  // where a ref_ptr<const T> is expected. 
  template<class T>
  struct constref2ref { 
  private:
    struct dummy_type {};
  public:
    typedef dummy_type type; 
  };

  template<class T>
  struct constref2ref<const T> { 
    typedef T type; 
  };

  template<class T>
  struct ref2constref { 
    typedef const T type; 
  };

  template<class T>
  struct ref2constref<const T> { 
  private:
    struct dummy_type {};
  public:
    typedef dummy_type type; 
  };
  */



/*! \brief class for disallowing taking the address of temporary.
    \author Guntram Berti

    \ingroup memory
    \see \c ref_ptr

    The intent of this class is to forbid taking the address of temporary objects
    returned from function calls (see docs of ref_ptr for an example).
    This class is best used in conjunction with \c ref_ptr.

    A \c temporary<T> should ideally behave like a \c T.
    Unfortunately, this is not possible, because we cannot overload
    \c operator. (member access).
    Therefore, the corresponding pointer operators were overloaded.
    The address-of \c operator& was made private to forbid obtaining the
    address of the temporary object.
 
    \note What we did try with this class was an implicit conversion to \c ref_ptr<T>
     if assigned to such an object. However, this does not seem to work.
     Neither the conversion operator  to \c ref_ptr<T> nor the constructors of \c ref_ptr taking
     \c temporary work as desired. This needs further attention. Currently, no \c temporary<T>
     objects are returned, so this class is not used. Instead, we return \c  ref_ptr, which always
     uses dynamic allocation.

*/

template<class T>
class temporary {
  typedef temporary<T> self;
  T t;

  // Forbidden
  self* operator&() { return this;}
public:
  explicit temporary(T tt) : t(tt) {}

  // make temporary<T> behave like T const& / T const* in almost all situations ...
  // ...except that we cannot take the address of it.
  // because we cannot overload operator., this conversion does not normally do the trick.
  // operator T const& () const { return t;}
  operator ref_ptr<T> () const; 
  T const& value() const { return t;}
  // must rely on pointer interface
  T const& operator*()  const { return t;}
  T const* operator->() const { return &t;}
};

struct null_deleter
{
  void operator()(void const *) const {}
};

struct  ref_ptr_base {
  enum ownership { referenced, shared};
};

/*! \brief class for maintaining a assignable reference to another (often const) object 
    \author Guntram Berti

   \ingroup memory
    
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
   and to use the constructor to differentiate between the cases (which does not work automatically).

   In case 1 and 3, we just pass a plain reference or pointer to the object to the
   constructor of \c ref_ptr<>. <br>
   For filtering out case 2, we rely on the temporary object being wrappend into
   the \c temporary<> template. <br>
   For handling case 4, we also pass in a pointer, but must explicitely enable shared ownership.

*/

template<class T>
class ref_ptr : public ref_ptr_base {
  typedef ref_ptr<T> self;

  boost::shared_ptr<T> ptr;
public:
  ref_ptr()  {} 

  // Cannot overload for T const& and T. So caller must make sure that temporary<T> is passed instead of T.
  // If we disable this, at least the caller must apply the address-of operator&
  // which could inhibit some misuse.

  /*! \brief Store a reference to \c t
       
      \post \c is_reference()
   */
  explicit ref_ptr(T  &     t) : ptr(&t , null_deleter()) 
  { ENSURE(is_reference(),"",1); }

  // FIXME: This might result in memory leak if called with newly allocated ptr and own=false
  // Perhaps add shared_ptr<T> to data and allow only ref_ptr(shared_ptr<T>) ?

  /*! \brief Store a reference to \c *tptr

      If \c tptr points to dynamically allocated memory, 
      deallocation  remains in the responsibility of the caller.
      
      \post \c is_reference()
   */
  explicit ref_ptr(T *   tptr) 
    : ptr(tptr , null_deleter())
  { 
    ENSURE(is_reference(),"",1);
  } 

  template<class U>
  explicit ref_ptr(U *   uptr) 
    : ptr(uptr , null_deleter())
  { 
    ENSURE(is_reference(),"",1);
  } 

  /*! \brief Store either a reference or a shared pointer to \c *tptr
      \pre if <tt> own == shared </tt> then \c tptr must point to dynamically allocated memory,
      i.e. calling <tt> delete tptr </tt> must be legal.
      \post if <tt> own == referenced </tt> then <tt> is_referenced() == true</tt>
            <br>
            if <tt> own == shared </tt> then <tt> is_shared() == true</tt>
  */
  ref_ptr(T *   tptr, ownership own) 
  { 
    if(own == referenced) {
      ptr.reset(tptr, null_deleter());
      ENSURE(is_reference(),"",1);
    }
    else { // own == shared
      // this cannot check if tptr is a pointer to a heap-allocated array element,
      // which is equally forbidden.
      REQUIRE(!is_stack_address(tptr), "Attempt to create shared pointer from automatic (stack) variable!",1);
      ptr.reset(tptr);
      ENSURE(is_shared(),"",1);
    } 
  }

  template<class U>
  ref_ptr(U *   uptr, ownership own) 
  { 
    if(own == referenced) {
      ptr.reset(uptr, null_deleter());
      ENSURE(is_reference(),"",1);
    }
    else { // own == shared
      // this cannot check if uptr is a pointer to a heap-allocated array element,
      // which is equally forbidden.
      REQUIRE(!is_stack_address(uptr), "Attempt to create shared pointer from automatic (stack) variable!",1);
      ptr.reset(uptr);
      ENSURE(is_shared(),"",1);
    } 
  }

  /*
  // FIXME
  // if not explicit, gcc will try to use an intermediate temporary<> in some cases.
  // else, it will not construct from a temporary<T> ... strange! 
  // explicit 
  ref_ptr(temporary<T> t) : ptr(new T(t.value())) 
  {     ENSURE(is_shared(),"",1); }  
  
  template<class U>
  explicit
  ref_ptr(temporary<U> t) : ptr(new T(t.value())) 
  {     ENSURE(is_shared(),"",1); } 
  */
 

  // copy constructors and assigment
  ref_ptr(ref_ptr<T> const& rhs) : ptr(rhs.ptr) {} 

  ref_ptr<T> & operator=(ref_ptr<T> const& rhs) { 
    if(this != &rhs) {
      ptr = rhs.ptr;
    }
    return *this;
  }
  /* this seems to lead to ambigous conversions, while not allowing implicit conversions
  // if T is 'const U', allow construction from ref_ptr<U>
  ref_ptr(ref_ptr<typename constref2ref<T>::type> const& rhs) : ptr(rhs.get_ptr()) {}

  typedef ref_ptr<typename ref2constref<T>::type> const_ref_ptr; 
  operator const_ref_ptr() const { return const_ref_ptr(*this);}
  */

  // disabled - too dangerous.
  /*  \brief Set to reference to tptr
    
      \post \c is_reference()
   */
  /*
  ref_ptr<T> & operator=(T * tptr) { 
    //REQUIRE(!is_stack_address(tptr), "Stack objects must be passed as reference!", 1);
    // ptr.reset(tptr); 
    make_ref(tptr);
    return *this;
    ENSURE(is_reference(),"",1);
  }
  */
 

  //! conversion constructors 
  template<class U>
  ref_ptr(ref_ptr<U>   const&   rhs) : ptr(rhs.get_ptr()) {}  

  //! conversion assignment
  template<class U>
  ref_ptr<T> & operator=(ref_ptr<U>   const&   rhs) {
    ptr = rhs.get_ptr();
    return *this;
  }


  /*! \brief Set to a shared pointer to \c uptr
      \pre \c tuptr must point to dynamically allocated memory,
      i.e. calling <tt> delete uptr </tt> must be legal. 
      \post \c is_shared()
   */
  template<class U>
  void make_shared(U * uptr) 
  {
    // uptr must point to a dynamically allocated object, i.e. delete uptr must be OK.
    REQUIRE(!is_stack_address(uptr), "Stack objects must be passed as reference!", 1);
    ptr.reset(uptr);
    ENSURE(is_shared(),"",1);
  }

  //! Set to a reference to uptr
  template<class U>
  void make_ref(U * uptr)
  {
    ptr.reset(uptr, null_deleter());
    ENSURE(is_reference(),"",1);
  }

  /*! \brief make a copy of the pointee
      \pre  <tt> valid() </tt>
      \post <tt> valid() && shared() </tt>
   */
  void make_copy() {
    if(ptr) {
      ptr.reset(new T(*ptr));
      ENSURE(is_shared(),"",1);
    }
  }


  // validity check
  bool valid() const { return ptr != 0;}
  void clear() { ptr.reset();}

  bool is_shared()    const { return (boost::get_deleter<null_deleter>(ptr) != 0 ? false : true);}
  bool is_reference() const { return !is_shared();}


  friend bool operator==(self const& c, T         * p) { return c.get() == p;}
  friend bool operator==(T         * p, self const& c) { return c.get() == p;}
  friend bool operator!=(self const& c, T         * p) { return c.get() != p;}
  friend bool operator!=(T         * p, self const& c) { return c.get() != p;}
private:
public:
  // comparison must be possible also with 0 pointers
  T * get()        const {                           return ptr.get();}
  T * operator->() const { REQUIRE(ptr != 0, "", 1); return ptr.get();} 
  T & operator* () const { REQUIRE(ptr != 0, "", 1); return *ptr;}
  boost::shared_ptr<T> const& get_ptr() const { return ptr;}
};



template<class T>
temporary<T>::operator ref_ptr<T> () const { return ref_ptr<T>(new T(t), ref_ptr_base::shared);}


  /*! \defgroup refptrgenerator Generator functions for ref_ptr
      
      These generator functions make it easier/more convenient to generate a ref_ptr

      \ingroup memory
      \relates ref_ptr
 */

  /*! \brief Copy \c t to a ref_ptr
     \relates ref_ptr
     \ingroup refptrgenerator
  */
  template<class T>
  inline ref_ptr<T> copy_to_ref_ptr(T const& t) { return ref_ptr<T>(new T(t), ref_ptr_base::shared);}


  /*! \brief Convert newly allocated pointer to ref_ptr

      \ingroup refptrgenerator
   
      It is assumed that \c t is newly allocated, and no \c delete is called.
      Example:
      \code
      {
      // ...
      ref_ptr<MyClass> r = new_ref_ptr(new MyClass(myargs));
      return r;
      // note: Ownership is taken by r.
      }
      \endcode
   */
  template<class T>
  inline ref_ptr<T> new_ref_ptr(T * t) { return ref_ptr<T>(t, ref_ptr_base::shared);}




  /*! \brief Copy \c t to a const ref_ptr
    \relates ref_ptr
  */
  template<class T>
  inline ref_ptr<const T> copy_to_const_ref_ptr(T const& t)
  { return ref_ptr<const T>(new T(t), ref_ptr_base::shared);}
  


  template<class T>
  inline ref_ptr<T>  ref_to_ref_ptr(T & t) { return ref_ptr<T>(t);}

  template<class T>
  inline ref_ptr<const T> const_ref_to_ref_ptr(T const& t) { return ref_ptr<const T>(t); }

  template<class T>
  inline T& ref(T & t) { return t;}

  template<class T>
  inline T& ref(ref_ptr<T> t) { return *t;}



} // namespace GrAL 

#endif
