#ifndef GRAL_GB_SEQUENCE_QUANTIZATION_H
#define GRAL_GB_SEQUENCE_QUANTIZATION_H

// $LICENSE_NEC_2005

#include <vector>

#include "Utility/pre-post-conditions.h"

namespace GrAL {


  /*! \brief Map a continous range of values to a discrete one.

  */
  template<class A, class R>
  class quantization {
  public:
    typedef A argument_type;
    typedef R result_type;

    typedef R label_type;
  private:
    // invariant: label.size() = 1 + threshold.size();
    std::vector<argument_type> threshold;
    // label[i] for args in \f$ T_i \f$, where T_i = [t_{i-1}, t_i), 1 \leq i < n,
    // \f$ T_0 = (-\infty, t_0) \f$ and \f$ T_n = [t_{n-1}, \infty) \f$
    std::vector<result_type>   label;  

  public:

    quantization() { labels.push_back(0); }
    quantization(argument_type x) {
      threshold.push_back(x);
      label.push_back(0);
      label.push_back(1);
      ENSURE_ALWAYS( this->operator()(x) == label[1], "", 1);
    }
    quantization(argument_type t0, argument_type t1) {
      REQUIRE_ALWAYS(t0 < t1, "", 1);
      threshold.push_back(t0);
      threshold.push_back(t1);
      label.push_back(0);
      label.push_back(1);
      label.push_back(2);
      ENSURE_ALWAYS( this->operator()(t0) == label[1], "", 1);
      ENSURE_ALWAYS( this->operator()(t1) == label[2], "", 1);
    }
    quantization(argument_type t0, argument_type t1, argument_type t2) {
      REQUIRE_ALWAYS(t0 <= t1 && t1 <= t2, "", 1);
      threshold.push_back(t0);
      threshold.push_back(t1);
      threshold.push_back(t2);
      label.push_back(0);
      label.push_back(1);
      label.push_back(2);
      label.push_back(3);
      ENSURE_ALWAYS( this->operator()(t0) == label[1], "", 1);
      ENSURE_ALWAYS( this->operator()(t1) == label[2], "", 1);
      ENSURE_ALWAYS( this->operator()(t2) == label[3], "", 1);
    }

    template<class It>
    void init(It first, It beyond) {
      REQUIRE_ALWAYS(std::is_sorted(first, beyond), "", 1);

      while(first != beyond) {
	threshold.push_back(*first++);
	label    .push_back(*i++);
      }
      label.push_back(i);
    }


    label_type operator()(argument_type v) const { 
      int i = 0;
      //while(v >= threshold[i] && i < (int)threshold.size() -1)
      while( i < (int)threshold.size() && v >= threshold[i])
	++i;
      //if(v >= threshold[i])
	// in this case, we stopped because i == threshold.size() - 1 
	//++i;
      return label[i];
    }

    label_type low () const { return label.front();}
    label_type high() const { return label.back ();}

  };

} // namespace GrAL

#endif
