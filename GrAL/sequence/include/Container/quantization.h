#ifndef GRAL_GB_SEQUENCE_QUANTIZATION_H
#define GRAL_GB_SEQUENCE_QUANTIZATION_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Utility/pre-post-conditions.h"
#include "Container/sequence-algorithms.h"
#include "Container/integer-range.h"

#include <vector>
#include <algorithm>
#include <iostream>

namespace GrAL {


  /*! \brief Map a continuous range of values to a discrete one.

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

    quantization() { this->label.push_back(0); }
    quantization(argument_type x) {
      threshold.push_back(x);
      label.push_back(0);
      label.push_back(1);
      ENSURE_ALWAYS( this->operator()(x) == label[1], "", 1);
    }
    quantization(argument_type t0, argument_type t1) {
      argument_type a[2] = { t0, t1 };
      init(a,a+2);
    }
    quantization(argument_type t0, argument_type t1, argument_type t2) {
      argument_type a[3] = { t0, t1, t2 };
      init(a, a+3);
    }
    void clear() { label.clear(); threshold.clear(); }

    template<class It>
    void init(It first, It beyond) {
      REQUIRE_ALWAYS(sequence::is_increasing(first, beyond), "", 1);
      int i = 0;
      while(first != beyond) {
	threshold.push_back(*first++);
	label    .push_back( i++);
      }
      label.push_back(i);
    }

    template<class TIt, class LIt>
    void init(TIt first_threshold, TIt beyond_threshold,
	      LIt first_label,     LIt beyond_label);

    int label_index(argument_type v) const {
      int i = 0;
      while( i < (int)threshold.size() && v >= threshold[i])
	++i;
      // now i == threshold.size() || v < threshold[i]
      return i;
    }
    label_type  operator()(argument_type v) const { return label[label_index(v)];}
    label_type& operator()(argument_type v)       { return label[label_index(v)];}

    void read (std::istream& in);
    void write(std::ostream& out) const;

  }; // class quantization



  template<class A, class R>
  std::istream& operator>>(std::istream& in,  quantization<A,R>      & q) { q.read(in); return in;}

  template<class A, class R>
  std::ostream& operator<<(std::ostream& out, quantization<A,R> const& q) { q.write(out); return out;}



  template<class A, class R>
  template<class TIt, class LIt>
  void quantization<A,R>::init(TIt first_threshold, TIt beyond_threshold,
			       LIt first_label,     LIt beyond_label) 
  {
    REQUIRE_ALWAYS(sequence::is_increasing(first_threshold, beyond_threshold), "", 1);
    REQUIRE_ALWAYS(1 + std::distance(first_threshold, beyond_threshold) 
		   =   std::distance(first_label,     beyond_label), "", 1);
    while(first_threshold != beyond_threshold) {
      threshold.push_back(*first_threshold++);
      label    .push_back(*first_label    ++);
    }
    label.push_back(*first_label);
  }



  template<class A, class R>
  void quantization<A,R>::read(std::istream & in) 
  { 
    clear();
    
    int n_thresholds;
    in >> n_thresholds;
    threshold.resize(n_thresholds);
    for(int t = 0; t < n_thresholds; ++t) 
      in >> threshold[t];
    label.resize(n_thresholds + 1);
    for(int i = 0; i < (int)label.size(); ++i)
      in >> label[i];
  }

  template<class A, class R>
  void quantization<A,R>::write(std::ostream& out) const 
  {
    out << threshold.size() << " ";
    for(int t = 0; t < (int)threshold.size(); ++t)
      out << threshold[t] << " ";
    for(int i = 0; i < (int)label.size(); ++i)
      out << label[i] << " ";
  }


} // namespace GrAL

#endif
