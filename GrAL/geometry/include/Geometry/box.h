#ifndef NMWR_GB_GEOMETRIC_BOX_H
#define NMWR_GB_GEOMETRIC_BOX_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// A rectangular box for arbitrary coordinate types.
//
//----------------------------------------------------------------

template<class coord>
class box {
  typedef box<coord>          self;
  typedef point_traits<coord> pt;
private:
  coord minc, maxc;
public:
  box(const coord& cmin, const coord& cmax) : minc(cmin), maxc(cmax) {}

  const coord& the_min() const { return minc;}
  const coord& the_max() const { return maxc;}

  // (0,..,0) -> minc , (1,...,1) -> max
  coord global_coords(const coord& local) {
    coord res = minc;
    for(int i = pt::LowerIndex(res); i <= pt::UpperIndex(res); ++i) {
      res[i] = (1-local[i]) * minc[i] + local[i] * maxc[i];
    }
    return res;
  }
  // bool element(const coord& p) const;
  // bool empty() const;

  // intersection
  //  self operator & (const self& ls, const self& rs)
  // closure of union
  friend self operator | (const self& ls, const self& rs)
    { 
      coord new_min(ls.minc), new_max(ls.maxc);
      for(int i = pt::LowerIndex(new_min); i <= pt::UpperIndex(new_min); ++i) {
          new_min[i] = min(new_min[i],rs.minc[i]);
          new_max[i] = max(new_max[i],rs.maxc[i]);
      }
      return self(new_min,new_max);
    }
};

#endif
