#ifndef GRAL_GB_HETEROGENEOUS_LIST_H
#define GRAL_GB_HETEROGENEOUS_LIST_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {


  /*! \brief A heterogeneous list 

      The class template implements a compile-time
      definable list of different types, which can be defined
      conveniently using \c operator,:

      \code
      T1 t1; T2 t2, T3 t3;
      template<class LIST> void F(LIST l); 
      F(Begin, t1, t2, t3);
      \endcode

      This is still experimental!

      It is used for creating GMV output with an arbitrary number of
      grid functions of diverse types via the 
      OstreamGMV2DFmt and OstreamGMV3DFmt I/O adapter.
      An example is found in test-gmv-output2d.C and test-gmv-output.C
   */
namespace heterogeneous_list {

  struct END {};
  /*! \brief Begin of list marker. See  \ref heterogeneous_list.
   */
  struct BEGIN {};

  /*! \brief Compile-time list of heterogeneous types. See \ref heterogeneous_list 
   
  */

  template<class T, class TAIL>
    class List {
    public:
      typedef T value_type;
      typedef TAIL tail_type;
    private:
      T const* t_;
      TAIL     tail_;
    public:
      List(T const& t, TAIL const& tail1) : t_(&t), tail_(tail1) {}
      T const&    head()    const { return *t_;}
      TAIL const& tail() const { return tail_;}
    };


  // empty List
  template<class END>
    class List<END,END> {
    public:
      typedef END head_type;
      typedef END tail_type;
      END e;
      List() {}
      List(END,END) {}
      END head() { return e;}
      END tail() { return e;}
    };

  template<class T, class T2, class TAIL>
    List<T,List<T2,TAIL> > operator,(List<T2,TAIL> tail, T const& t)
    { return List<T,List<T2,TAIL> >(t,tail);}

  template<class T>
    List<T,List<END,END> > operator,(BEGIN b, T const& t)
    { return List<T,List<END,END> >(t,List<END,END>());}

} // namespace heterogeneous_list

} // namespace GrAL 

#endif
