#ifndef NMWR_GB_GEOMETRY_BOX_H
#define NMWR_GB_GEOMETRY_BOX_H

// $LICENSE


#include <algorithm>
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include <boost/limits.hpp>

/*! \defgroup boundingbox Bounding boxes 
    
*/


/*! \brief An axisparallel box for arbitrary coordinate types.

    \ingroup boundingbox 
    This class is especially useful for representing bounding boxes.
    Union (<tt> operator | </tt>) and intersection (<tt> operator & </tt>) of boxes give
    again boxes (namely bounding boxes of the set-theoretic union
    and intersection, resp.)

    A box can be used as a function, mapping \f$ [0,1]^d \f$ to the convex
    hull of its min and max corner <tt> [the_min(), the_max()]</tt>, 
    in the obvious way.

    \see Tested in \ref test-box.C
    \see Module  boundingbox
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

  void ce() const { REQUIRE(!empty(), "", 1);}

  const coord& the_min() const { return minc;}
  const coord& the_max() const { return maxc;}
  coord        center()  const { return 0.5*(minc+maxc);}

  // function interface
  typedef coord argument_type;
  typedef coord result_type;
  /*! \brief Map local coordinates to global coordinates
     
     \c local \f$ = (0, \ldots, 0) \mapsto \f$ \c the_min(), <br>
     \c local \f$ = (1, \ldots, 1) \mapsto \f$ \c the_max().
   */
  coord operator()(const coord& local) const
    { return global_coords(local); }

  // (0,..,0) -> minc , (1,...,1) -> max
  coord global_coords(const coord& local) const {
    ce();
    coord res = minc;
    for(int i = pt::LowerIndex(res); i <= pt::UpperIndex(res); ++i) {
      res[i] = (1-local[i]) * minc[i] + local[i] * maxc[i];
    }
    return res;
  }


  /*! \brief Volume of the box
   */
  scalar_type volume() const {
    if(empty()) return 0.0;
    scalar_type res = 1.0;
    for(int i = pt::LowerIndex(maxc); i <= pt::UpperIndex(maxc); ++i) {
      res *= (maxc[i] - minc[i]);
    }
    return res;
  }


  /*! \brief Scale by a factor \f$ s \geq 0 \f$
     
     The scaling is symmetric with respect to the center.
     A scaling factor of \f$ s = 1 \f$ will leave the box unchanged.
  */
  void scale(scalar_type s) { 
    REQUIRE(s >= 0.0, " s=" << s, 1);
    if(! empty()) {
      coord c(center());
      minc = c + s*(minc - c);
      maxc = c + s*(maxc - c);
    }
  }
  /*! \brief Translate the box by \c t
   */
  void translate(coord t) {
    if(!empty()) {
      minc += t;
      maxc += t;
    }
  }


  // dangerous - cannot check consistency
  // coord& the_min()  { return minc;}
  // coord& the_max()  { return maxc;}



  /*! \brief True iff \f$  p \in \f$ <tt> [the_min(), the_max()] </tt>
   */
  bool contains(coord const& p) const {
    bool res = true;
    for(int i = pt::LowerIndex(p); i <= pt::UpperIndex(p); ++i) {
      res = res && (p[i] >= minc[i]) && (p[i] <= maxc[i]);
    }
    return res;
  }
  
  /*! \brief True iff the box is empty.

     The box is empty iff <tt> the_min()[i] > the_max()[i] </tt> for some \c i.
  */
  bool empty() const {
    bool res = false;
    for(int i = pt::LowerIndex(minc); i <= pt::UpperIndex(minc); ++i) 
      res = res || (minc[i] > maxc[i]);
    return res;
  }


  //! intersection
  self& operator &=(self const& rhs) {
    for(int i = pt::LowerIndex(minc); i <= pt::UpperIndex(minc); ++i) {
      minc[i] = std::max(minc[i], rhs.minc[i]);
      maxc[i] = std::min(maxc[i], rhs.maxc[i]);
    }
    if(empty())
      *this = self();
    return *this;
  }


  //! closure of union
  self & operator |= (self const& rs)
    {
      for(int i = pt::LowerIndex(minc); i <= pt::UpperIndex(minc); ++i) {
          minc[i] = std::min(minc[i],rs.minc[i]);
          maxc[i] = std::max(maxc[i],rs.maxc[i]);
      }
      return *this;
    }

};


/*! \brief Box intersection.

    \see \c box<COORD>
    \ingroup boundingbox
*/
template<class COORD>
box<COORD>  operator & (box<COORD> const& b1, box<COORD> const& b2) 
{
  box<COORD> res(b1);
  return res &= b2;
}

/*! \brief Box union

    Returns the bounding box of the union of two boxes
    \see \c box<COORD>
    \ingroup boundingbox
*/
template<class COORD>
box<COORD>  operator | (box<COORD> const& b1, box<COORD> const& b2) 
{ 
  box<COORD> res(b1);
  return res |= b2;
}



/*! \brief Approximate Hausdorff distance of two bounding boxes
   
    \note This is not the exact Hausdorff distance.
    \see \c box<COORD>
    \ingroup boundingbox
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

    \see \c box<COORD>
    \ingroup boundingbox
 */
template<class COORD>
inline
typename box<COORD>::scalar_type diameter(box<COORD> const& b)
{
  typedef algebraic_primitives<COORD> ap;
  return ap::distance(b.the_min(), b.the_max());
}

#endif


