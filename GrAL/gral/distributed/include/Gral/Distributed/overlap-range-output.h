#ifndef NMWR_GB_OVERLAP_RANGE_OUTPUT_H
#define NMWR_GB_OVERLAP_RANGE_OUTPUT_H

// $LICENSE


#include "Utility/pre-post-conditions.h"

/*! \brief Output adapter for an overlap range
   \ingroup overlapds

   This class plays the same role as overlap_range.
   It does not allow access to individual ranges (exposed, shared etc),
   only to copy them as a whole.
 */
template<class E>
class overlap_range_output {
  typedef overlap_range_output<E> self;

  std::ostream* out; 
  bool out_is_owned; // is out owned or referenced?

  void init(self const& rhs) 
  {
    REQUIRE( !rhs.out_is_owned, "cannot copy ostream!\n",1);
    out = rhs.out;
    out_is_owned = false;
  }
public:
  typedef element_traits<E>        et;
  typedef typename et::handle_type element_handle;
  typedef typename et::grid_type   grid_type;
  typedef unsigned                 size_type;
  typedef element_handle           value_type;

  overlap_range_output() 
    : out(0), out_is_owned(false)
  {}

  overlap_range_output(grid_type const&)
    : out(0), out_is_owned(false)
  {}

  overlap_range_output(std::ostream& out_) 
    : out(&out_), out_is_owned(false)
  {}
  overlap_range_output(self const& rhs)
  { init(rhs);  }

  self& operator=    (self const& rhs)
  {
    init(rhs);
    return *this;
  }
  
  ~overlap_range_output() { if (out_is_owned) delete out;}

  void set_grid(grid_type const&) {}

  void init(std::ostream& out_) {
    out(&out_); 
    out_is_owned = false;
  }

  void init(std::string const& out_nm)
  {
    std::ofstream * fout  = new std::ofstream(out_nm.c_str());
    REQUIRE( (fout->is_open()), "Could not open file " << out_nm << "!\n", 1);
    out = fout;
    out_is_owned = true;
  }


public:
  template<class OVLPRANGE, class FILTER>
  void copy_from(OVLPRANGE const& ovlprange, 
                 FILTER    const& f);
};


/*! \brief Copy an overlap range to output adapter
    \ingroup overlapds
     This is a specialization of CopyOverlapRanges 
     for copying to a overlap_range_output adapter.
*/
template
< class SrcRange, 
  class E, 
  class Filter
>
void CopyOverlapRanges(overlap_range_output<E>      & dest, 
                       SrcRange                const& src,
                       Filter                  const& f)
{ dest.copy_from(src,f);}

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/overlap-range-output.C"
#endif


#endif
