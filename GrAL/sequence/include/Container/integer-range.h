#ifndef GRAL_GB_SEQUENCE_INTEGER_RANGE_H
#define GRAL_GB_SEQUENCE_INTEGER_RANGE_H

// $LICENSE_NEC_2004

#include "Geometry/interval.h"
#include "Utility/checked-istream.h"
#include "Utility/pre-post-conditions.h"

#include <ctype.h>
#include <iostream>
#include <vector>

namespace GrAL {  namespace sequence {

  /*! \brief Union of integer intervals

      This class represents a set of integers, represented
      as a sequence of closed intervals. It main feature 
      it the ability to read range descriptions like <tt>6,8,10-33,40-100</tt>
      via <tt>operator>></tt>.
   */
  template<class INT> class integer_range {
    typedef interval<INT> interval_type;
    typedef INT           element_type;
    std::vector<interval_type> intervals;
  public:
    //! Construct empty range
    integer_range() {}
    //! Construct range of single point
    integer_range(INT i) { intervals.push_back(interval_type(i,i));}
    //! Construct range of single interval
    integer_range(interval_type iv) { if(!iv.empty()) intervals.push_back(iv);}


    //! True if no integer is contained
    bool empty() const { return intervals.empty();}
    //! Set to empty range
    void clear()       { intervals.clear();}

    //! STL-compliant iterator
    typedef typename std::vector<interval_type>::const_iterator const_iterator;
    //! First interval
    const_iterator begin() const { return intervals.begin();}
    //! Past-the-end interval
    const_iterator end()   const { return intervals.end();}

    //! smallest element. The range is contained in [low(), high()]
    element_type low()  const {return intervals.front().low();}
    //! largest element. The range is contained in [low(), high()]
    element_type high() const {return intervals.back() .high();}


    //! True iff \c i is contained in an interval
    bool contains(INT i) const { 
      bool res = false;
      for(const_iterator iv = begin(); iv != end(); ++iv)
	res = res || iv->contains(i);
      return res;
    }
    void read(std::istream& in);
  private:
    // returns false if read not successful
    bool read_interval(std::istream& in, interval_type& iv, bool& end);
    // returns false if read not successful
    bool read_number  (std::istream& in, INT & i);
    //  void compactify() { /* sort and join overlapping intervals */ }
  };

  /*! \brief Input operator. Can read range descriptions like <tt>6,8,10-33,40-100</tt>.
      
   */
  template<class INT>
  std::istream& operator>>(std::istream& in, integer_range<INT> & r)
  { r.read(in); return in;}

  /*! \brief Output operator

       Writes the range <tt>6,8,10-33,40-100</tt> 
       in the normalized form <tt>6-6,8-8,10-33,40-100</tt>.
   */ 
  template<class INT>
  std::ostream& operator<<(std::ostream& out, integer_range<INT> const& r)
  { 
    for(typename integer_range<INT>::const_iterator iv = r.begin(); iv != r.end(); ++iv) {
      if(iv != r.begin())
	out << ',';
      out << iv->l() << '-' << iv->r();
    }
    return out;
  }





  template<class INT> 
  void integer_range<INT>::read(std::istream& in)
  {
    clear();
    interval_type iv;
    bool end = false;
    while(!end && read_interval(in,iv,end) ) { 
      if(! iv.empty())
	intervals.push_back(iv);
    }
  }

  template<class INT> 
  bool integer_range<INT>::read_number  (std::istream& in, INT & i)
  {
    checked_istream checked_in(in);
    return (checked_in >> i);
  }
  
  template<class INT> 
  bool integer_range<INT>::read_interval(std::istream& in, typename integer_range<INT>::interval_type & iv, bool& end) 
  {
    INT low, high;
    bool res = true;
    if(! (in >> low)) {
      end = true;
      return false;
    }
    high = low;
    char next = '\0';

    if(in.eof() || ! in.get(next)) {
      end = true;
      in.clear();
    }
    else {
      if(next == '-') {
	res = read_number(in, high);
	if(res) {
	  if(in.get(next) &&  next != ',') {
	    if(! isspace(next)) {
	      res = false;
	    }
	    else {
	      end = true;
	    }
	    in.putback(next);
	  }
	}
      }
      else if( next == ',') {
	res = true;
	if(in.get(next)) {
	  end = ! (isdigit(next) || next == '-' || next == '+'); // sign prefixes!
	  in.putback(next);
	}
      }
      else if(isspace(next)) {
	res = true;
	end = true;
	in.putback(next);
      }
      else {
        res = false;
	end = true;
	// REQUIRE(false,"Failure to read interval!",1);
      }
    } // could get next char
    iv = interval_type(low,high);
    return res;
  }


} } // namespace GrAL {  namespace sequence {

#endif
