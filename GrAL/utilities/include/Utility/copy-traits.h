#ifndef NMWR_GB_COPY_TRAITS_H
#define NMWR_GB_COPY_TRAITS_H


// $LICENSE

namespace GrAL {

/*! \brief  Default implementation for \c copy_traits<>
 */
template<class T>
struct copy_traits_base {
    static T* clone(T const& t) { return (new T(t));}
    static void destroy(T * t)  { delete t;}
};

/*!
   \brief Compile-time branch on how to copy pointees.

    If we have a pointer to an abstract base of a polymorphic
    type, we cannot copy the content by simply calling <TT>operator new</TT>.
    Instead, we must use some virtual member function like <TT>clone()</TT>.
    This is a problem when using polymorphic types together with 
    smart pointers (how do they copy the pointer?). 
    Also, if we want to use the 'pimpl' idiom together with a smart pointer,
    we cannot call delete on the opaque pointer-to-impl inside the smart pointer.

    These differences are hidden by 
    <TT> T* copy_traits<T>::clone(const T&)</TT>
    and
    <TT> copy_traits<T>::destroy(T*)</TT>
    Default implementation in copy_traits_base 
    for non-polymorphic types
    is simply operator new / delete.

    For T being an abstract class, this implmentation
    has to be overridden with
    a method like <TT>T::clone()</TT>
    by specializing <TT> copy_traits<> </TT> for T.

    For an opaque type, we can \e declare a specialization
    along with the forward declaration of the type:
    
    \code
    class impl;
    template<>
    struct copy_traits<impl> {
      static impl* clone  (impl const&);
      static void  destroy(impl*);
   };
   \endcode

   and \e define the specialization with the definition of the type:

   \code
   class impl { ... };
   
   impl* copy_traits<impl>::clone  (impl const& i) { return new impl(i);}
   void  copy_traits<impl>::destroy(impl      * i) { delete i;}
   \endcode
*/
template<class T>
struct copy_traits : public copy_traits_base<T> {};

} // namespace GrAL 

#endif
