#ifndef NMWR_GB_GEOMETRY_BOX_H
#define NMWR_GB_GEOMETRY_BOX_H

// $LICENSE


#include <algorithm>
#include "Geometry/point-traits.h"
#include <boost/limits.hpp>

/*! \brief An axisparallel box for arbitrary coordinate types.

    This class is especially useful for representing bounding boxes.
    Union (operator |) and intersection (operator &) of boxes give
    again boxes (namely bounding boxes of the set-theoretic union
    and intersection, resp.)

    A box can be used as a function, mapping [0,1]^d to the convex
    hull of its min and max corner, in the obvious way.

    \see Tested in test-box.C 
 */



template<class coord>
class box {
  typedef box<coord>          self;
  typedef point_traits<coord> pt;
  typedef typename pt::component_type scalar_type;
private:
  coord minc, maxc;
public:
  box() : minc( std::numeric_limits<scalar_type>::infinity()),
	  maxc(-std::numeric_limits<scalar_type>::infinity())
  {}    // => |= works correctly
  box(const coord& cmin) : minc(cmin), maxc(cmin) {}
  box(const coord& cmin, const coord& cmax) : minc(cmin), maxc(cmax) 
  {}

  const coord& the_min() const { return minc;}
  const coord& the_max() const { return maxc;}
  coord        center()  const { return 0.5*(minc+maxc);}

  // dangerous - cannot check consistency
  coord& the_min()  { return minc;}
  coord& the_max()  { return maxc;}

  // (0,..,0) -> minc , (1,...,1) -> max
  coord global_coords(const coord& local) const {
    coord res = minc;
    for(int i = pt::LowerIndex(res); i <= pt::UpperIndex(res); ++i) {
      res[i] = (1-local[i]) * minc[i] + local[i] * maxc[i];
    }
    return res;
  }
  // function interface
  typedef coord argument_type;
  typedef coord result_type;
  coord operator()(const coord& local) const
    { return global_coords(local); }

  bool contains(coord const& p) const {
    bool res = true;
    for(int i = pt::LowerIndex(p); i <= pt::UpperIndex(p); ++i) {
      res = res && (p[i] >= minc[i]) && (p[i] <= maxc[i]);
    }
    return res;
  }
  
  // bool element(const coord& p) const;
  bool empty() const {
    bool res = false;
    for(int i = pt::LowerIndex(minc); i <= pt::UpperIndex(minc); ++i) 
      res = res || (minc[i] > maxc[i]);
    return res;
  }


  // intersection
  // self& operator &=(self const&);
  //  self operator & (const self& ls, const self& rs)
  // closure of union
  self & operator |= (self const& rs)
    {
      for(int i = pt::LowerIndex(minc); i <= pt::UpperIndex(minc); ++i) {
          minc[i] = std::min(minc[i],rs.minc[i]);
          maxc[i] = std::max(maxc[i],rs.maxc[i]);
      }
      return *this;
    }
  friend self operator | (const self& ls, const self& rs)
    { 
      self res(ls);
      return res |= rs;
    }
  /*
      coord new_min(ls.minc), new_max(ls.maxc);
      for(int i = pt::LowerIndex(new_min); i <= pt::UpperIndex(new_min); ++i) {
          new_min[i] = min(new_min[i],rs.minc[i]);
          new_max[i] = max(new_max[i],rs.maxc[i]);
      }
      return self(new_min,new_max);
    }
  */
};

#endif


