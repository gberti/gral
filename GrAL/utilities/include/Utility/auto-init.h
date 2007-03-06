#ifndef GRAL_GB_UTILITY_AUTOINIT_H
#define GRAL_GB_UTILITY_AUTOINIT_H


// $LICENSE_NEC_2007

namespace GrAL {


  /*! \brief class with default initialisation

    \ingroup memory
 
    \templateparams
     - T must be an integral type (admissible as template value parameter)
     - value is the initial value

    \example
    \code
    class Foo {
    private:
      auto_init<bool, false> has_bar;
      auto_init<int,  0>     num_of_bars;
      //  ...

      // zillions of constructors
      Foo(args_1) { ... } // don't bother with contructors of data members
      Foo(args_2) { ... } // don't bother with contructors of data members
      // ...
      Foo(args_N) { ... } // don't bother with contructors of data members
    };
    \endcode
  */
  template<class T, T value>
  class auto_init {
    typedef auto_init self;
  private:
    T val;
  public:
    auto_init() : val(value) {}
    operator T() const { return val;}
    self& operator=(T const& rhs) { val = rhs; return *this;}
  };


} // namespace GrAL


#endif
