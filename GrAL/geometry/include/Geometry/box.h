#ifndef NMWR_GB_GEOMETRY_BOX_H
#define NMWR_GB_GEOMETRY_BOX_H

// $LICENSE


#include <algorithm>
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include <boost/limits.hpp>

/*! \brief An axisparallel box for arbitrary coordinate types.

    This class is especially useful for representing bounding boxes.
    Union (<tt> operator | </tt>) and intersection (<tt> operator & </tt>) of boxes give
    again boxes (namely bounding boxes of the set-theoretic union
    and intersection, resp.)

    A box can be used as a function, mapping \f$ [0,1]^d \f$ to the convex
    hull of its min and max corner <tt> [the_min(), the_max()]</tt>, 
    in the obvious way.

    \see Tested in test-box.C 
 */



template<class coord>
class box {
  typedef box<coord>          self;
public:
  typedef point_traits<coord>         pt;
  typedef typename pt::component_type scalar_type;
private:
  coord minc, maxc;
public:
  /*! \brief Create empty box
   */
  box() : minc( std::numeric_limits<scalar_type>::max()),
	  maxc(-std::numeric_limits<scalar_type>::max())
  {
    REQUIRE(empty(), "minc=" << minc << " maxc=" << maxc,1);
  }  
  /*! \brief Create bounding box of the single point \c p
   */
  box(const coord& p) : minc(p), maxc(p) {}
  /*! \brief Create bounding box of the set <tt> {cmin, cmax} </tt>
   */
  box(const coord& cmin, const coord& cmax) : minc(cmin), maxc(cmin)
  { *this |= cmax; }
  
  /*! \brief Create bounding box of the sequence <tt> [begin, end[ </tt>
   */
  template<class INPUTITERATOR >
  void init(INPUTITERATOR begin, INPUTITERATOR end)
  {
    minc = coord( std::numeric_limits<scalar_type>::max());
    maxc = coord(-std::numeric_limits<scalar_type>::max());
    while(begin != end) {
      for(int i = pt::LowerIndex(minc); i <= pt::UpperIndex(minc); ++i) {
	minc[i] = std::min(minc[i], (*begin)[i]);
	maxc[i] = std::max(maxc[i], (*begin)[i]);
      }
      ++begin;
    }
  }

  const coord& the_min() const { return minc;}
  const coord& the_max() const { return maxc;}
  coord        center()  const { return 0.5*(minc+maxc);}

  /*! \brief Scale by a factor \f$ s \geq 0 \f$
     
     The scaling is symmetric with respect to the center.
     A scaling factor of \f$ s = 1 \f$ will leave the box unchanged.
  */
  void scale(scalar_type s) { 
    REQUIRE(s >= 0.0, " s=" << s, 1);
    coord c(center());
    minc = c + s*(minc - c);
    maxc = c + s*(maxc - c);
  }
  /*! \brief Translate the box by \c t
   */
  void translate(coord t) {
    minc += t;
    maxc += t;
  }

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
  /*! \brief Map local coordinates to global coordinates
     
     \c local \f$ = (0, \ldots, 0) \mapsto \f$ \c the_min(), <br>
     \c local \f$ = (1, \ldots, 1) \mapsto \f$ \c the_max().
   */
  coord operator()(const coord& local) const
    { return global_coords(local); }

  /*! \brief True iff \f$  p \in \f$ <tt> [the_min(), the_max()] </tt>
   */
  bool contains(coord const& p) const {
    bool res = true;
    for(int i = pt::LowerIndex(p); i <= pt::UpperIndex(p); ++i) {
      res = res && (p[i] >= minc[i]) && (p[i] <= maxc[i]);
    }
    return res;
  }
  
  // bool element(const coord& p) const;
  /*! \brief True iff the box is empty.

     The box is empty iff <tt> the_min()[i] > the_max()[i] </tt> for some \c i.
  */
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

/*! \brief Approximate Hausdorff distance of two bounding boxes
   
    This is not the exact Hausdorff distance.
*/
template<class COORD>
inline
typename box<COORD>::scalar_type distance(box<COORD> const& b1, box<COORD> const& b2)
{
  typedef algebraic_primitives<COORD> ap;
  return std::max(ap::distance(b1.the_min(), b2.the_min()),
		  ap::distance(b1.the_max(), b2.the_max()));
}

/*! \brief Diameter of a box.

 */
template<class COORD>
inline
typename box<COORD>::scalar_type diameter(box<COORD> const& b)
{
  typedef algebraic_primitives<COORD> ap;
  return ap::distance(b.the_min(), b.the_max());
}

#endif


