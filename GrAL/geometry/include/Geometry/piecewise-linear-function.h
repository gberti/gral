#ifndef GRAL_GB_GEOMETRY_PIECEWISE_LINEAR_FUNCTION_H
#define GRAL_GB_GEOMETRY_PIECEWISE_LINEAR_FUNCTION_H

// $LICENSE_NEC_2005

#include "Utility/checked-istream.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>
#include <vector>
#include <algorithm>


namespace GrAL {

  /*! \brief Piecewise linear function 

     \see Tested in \ref test-piecewise-linear-function.C
   */
  template<class ARG, class VAL = ARG>
  class piecewise_linear_function {
  public:
    typedef ARG   argument_type;
    typedef VAL   value_type;
    typedef value_type result_type;
  private:
    std::vector<argument_type> t;
    std::vector<value_type>    v;
    value_type v_left;
    value_type v_right;
  public:
    explicit piecewise_linear_function(value_type vv = value_type(0)) 
      : t(1, argument_type(0)), v_left(vv), v_right(vv)  {}

    template<class ARG_IT, class VAL_IT>
    piecewise_linear_function(ARG_IT arg_begin, ARG_IT arg_end,
			      VAL_IT val_begin, VAL_IT val_end) 
    { init(arg_begin, arg_end, val_begin, val_end);}

    template<class ARG_IT, class VAL_IT>
    piecewise_linear_function(ARG_IT arg_begin, ARG_IT arg_end,
			      VAL_IT val_begin, VAL_IT val_end,
			      value_type val_left, value_type val_right)
    { init(arg_begin, arg_end, val_begin, val_end, val_left, val_right);}


    template<class ARG_IT, class VAL_IT>
    void init(ARG_IT arg_begin, ARG_IT arg_end,
	      VAL_IT val_begin, VAL_IT val_end,
	      value_type v_left_, value_type v_right_);

    template<class ARG_IT, class VAL_IT>
    void init(ARG_IT arg_begin, ARG_IT arg_end,
	      VAL_IT val_begin, VAL_IT val_end);

    void read (std::istream& in);
    void write(std::ostream& out);
    void clear() { t.clear(); v.clear();}

    result_type operator()(argument_type x) const {
      return (x <= t[0] ? v_left : (x >= t[t.size()-1] ? v_right : interpolate(x)));
    }
    
    result_type interpolate(argument_type x) const {
      int i = interval(x); 
      argument_type tt = (x-t[i])/(t[i+1]-t[i]);
      return (1-tt)*v[i] + tt*v[i+1];
    }

    //! \brief Return i with <tt> t[i] <= x < t[i+1] </tt>
    int interval(argument_type x) const {
      REQUIRE( x >= t[0] && x <= t[t.size()-1], "x=" << x, 1);
      int res = std::lower_bound(t.begin(), t.end(), x) - t.begin() -1;
      ENSURE(t[res] <= x && x <= t[res+1], "res=" << res << " x=" << x, 1);
      return res;
    }
  };



  
  template<class ARG, class VAL>
  template<class ARG_IT, class VAL_IT>
  void piecewise_linear_function<ARG, VAL>::init(ARG_IT arg_begin, ARG_IT arg_end,
						 VAL_IT val_begin, VAL_IT val_end,
						 value_type v_left_, value_type v_right_)
  {
    clear();
    std::copy(arg_begin, arg_end, std::back_inserter(t));
    std::copy(val_begin, val_end, std::back_inserter(v));
    REQUIRE_ALWAYS(t.size() == v.size(), "",1);
    v_left  = v_left_;
    v_right = v_right_;
  }

  template<class ARG, class VAL>
  template<class ARG_IT, class VAL_IT>
  void piecewise_linear_function<ARG, VAL>::init(ARG_IT arg_begin, ARG_IT arg_end,
						 VAL_IT val_begin, VAL_IT val_end)
  {
    clear();
    init(arg_begin, arg_end, val_begin, val_end, *val_begin, *val_begin);
    v_left  = v[0];
    v_right = v[v.size()-1];
  }




  template<class ARG, class VAL>
  void piecewise_linear_function<ARG, VAL>::read(std::istream& in) 
  {
    clear();
    checked_istream chin(in);
    int n = 0;
    chin >> n;
    t.resize(n);
    v.resize(n);
    for(int i = 0; i < n; ++i)
      chin >> t[i];
    for(int i = 0; i < n; ++i)
      chin >> v[i];
    // chin >> v_left;
    // chin >> v_right;
  }

  template<class ARG, class VAL>
  void piecewise_linear_function<ARG, VAL>::write(std::ostream& out) 
  {
    out << t.size() << " ";
    for(unsigned i = 0; i < t.size(); ++i)
      out << t[i] << " ";
    out << "\n";
    for(unsigned i = 0; i < v.size(); ++i)
      out << v[i] << " ";
    //  out << v_left << " ";
    //  out << v_right;
   }

  template<class ARG, class VAL>
  std::istream& operator>>(std::istream& in, piecewise_linear_function<ARG, VAL>& f)
  {
    f.read(in);
    return in;
  }

  template<class ARG, class VAL>
  std::ostream& operator<<(std::ostream& out, piecewise_linear_function<ARG, VAL> const& f)
  {
    f.write(out);
    return out;
  }


} // namespace GrAL


#endif
