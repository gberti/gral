#ifndef NMWR_GB_GENERAL_INDEXMAP_H
#define NMWR_GB_GENERAL_INDEXMAP_H

// $LICENSE

#include <iostream>
#include "Utility/pre-post-conditions.h"
#include "Gral/Grids/Cartesian2D/index-type.h"

/*! \brief Bijection between subsets of \f$ \Z \f$ and \f$ \Z \times \Z \f$

 This class implements a bijective map between
 the subset [n0,n0+size-1] of Z and the subset
 [v1,v2] := [min(v1.x,v2.x), max(v1.x,v2.x)]x[min(v1.y,v2.y), max(v1.y,v2.y)] of ZxZ 
 with given minor direction (varies fastest) :

 \code
 (llx,ury)  = v2 (e.g)                     (urx,ury)

          +  n0+(nx*ny)-1                  +  n0+n_minor
          .
          .                                ^
                                           | minor direction
          +  
                                 maj. dir.
          +  n0+(nx*(ny-1)) -1      <-     + n0

 (llx,lly)                                  (urx,lly) = v1 (e.g)
 \endcode

   \todo  Empty ranges cannot be represented 
   (in contrast to xmjr_indexmap2D, where ur < ll indicates
     an empty range)
*/



class general_indexmap2D {
private:
  // helper functions
  template<class T>
  int sign(const T& t) const { return (t < 0 ? -1 : (t == 0 ? 0 : 1)); }

  template<class T>
  T abs(const T& t) const { return ( t < 0 ? -t : t);}

public:
  typedef int_index_type index_type;
  typedef general_indexmap2D self;

  struct range {
    int x,y;
    range(int xx, int yy) : x(xx), y(yy) {}
    friend ostream& operator<<(ostream& out, const range& r) {
      return (out << '[' << r.x << ',' << r.y << ']');
    }
  };


  //@{ @name Constructors
  general_indexmap2D(const index_type& V2 = index_type(0,0))
   : v1(0,0),v2(V2), dir(0,1), n0_(0) { init();}
  general_indexmap2D(int urx, int ury)
   : v1(0,0), v2(urx,ury), dir(0,1), n0_(0) { init();}
  
  general_indexmap2D(const index_type& V1,
		 const index_type& V2, 
		 const index_type& Dir)
   : v1(V1), v2(V2), dir(Dir), n0_(0) { init();}
  //@}

private:
  void init() { 
    ll_ = v1; ur_ = v2;
    make_ordered(ll_,ur_);

    minor_idx = ( dir[1] == 0 ? 2 : 1);
    major_idx = 3 - minor_idx;

    minor_incr = sign(v2[minor_idx] -v1[minor_idx]);
    major_incr = sign(v2[major_idx] -v1[major_idx]);

    np[minor_idx] = abs(v2[minor_idx]-v1[minor_idx]) +1;
    np[major_idx] = abs(v2[major_idx]-v1[major_idx]) +1;
    rect_size_p = np.x*np.y;
    nmax_ = n0_ + rect_size_p -1;
  }
  
public:
  int x0()   const { return ll_.x;}
  int xmax() const { return ur_.x;}
  int y0()   const { return ll_.y;}
  int ymax() const { return ur_.y;}
  int n0()   const { return n0_;}
  int nmax() const { return nmax_; }

  const index_type& ll() const {return ll_;}
  int llx() const {return ll_.x;}
  int lly() const {return ll_.y;}
  const index_type& ur() const {return ur_;}
  int urx() const {return ur_.x;}
  int ury() const {return ur_.y;}
 


  bool IsInRange(int x, int y) const { 
    return ((ll_.x <= x) && ( x <= ur_.x) &&(ll_.y <= y) && ( y <= ur_.y));
  }
  bool IsInRange(const index_type& i) const { 
    return IsInRange(i.x,i.y);
  }
  bool IsInRange(int i) const {
    return((n0() <= i) && ( i <= n0() -1 + range_size()));
  }

  int number(int x, int y) const { 
    REQUIRE(IsInRange(x,y),
	    "index (x,y) = (" << x << "," << y << ") not in range "
	    << range(ll_.x,ur_.x) << "x" << range(ll_.y,ur_.y) << "!",1);
    index_type v(x,y);
    return (n0() + abs(v[major_idx] - v1[major_idx])*np[minor_idx] + abs(v[minor_idx]-v1[minor_idx]));
  }

  int number(const index_type& idx) const { return number(idx.x,idx.y);}
  int offset(int x, int y) const { return number(x,y) - n0();}
  int offset(const index_type ij) const { return number(ij) - n0();}

  index_type index(int i) const {
    REQUIRE(IsInRange(i),
	    "number not in range " << range(n0(),n0() + range_size() -1) << ": i = " << i,1);

    int i0 = i - n0();
    int res_major = i0 / np[minor_idx];
    int res_minor = i0 - res_major*np[minor_idx];
    index_type res;
    res[minor_idx] = v1[minor_idx] + minor_incr*res_minor;
    res[major_idx] = v1[major_idx] + major_incr*res_major;
    return res;
  }
  
  void next(index_type& v) {
     if( v[minor_idx] != v2[minor_idx])
       v[minor_idx] += minor_incr;
     else
       v[major_idx] += major_incr;
  }

  int range_size() const { return rect_size_p;}
  void pretty_print(std::ostream& out);


  // return an enlarged or shrinked range
  // (e.g. vertex-range -> corr. cell-range: diff = 1

  void shrink_x_v(int diff, index_type& v1_s, index_type& v2_s) const {
    if(v1_s.x < v2_s.x)
      v2_s.x -= diff;
    else if (v1_s.x > v2_s.x)
      v1_s.x -= diff;
  }

  void shrink_y_v(int diff, index_type& v1_s, index_type& v2_s) const {
    if(v1_s.y < v2_s.y)
      v2_s.y -= diff;
    else if (v1_s.y > v2_s.y)
      v1_s.y -= diff;
  }

  self shrinked_map(int diff) const {
     index_type v1_shrink(v1), v2_shrink(v2);
     shrink_x_v(diff,v1_shrink,v2_shrink);
     shrink_y_v(diff,v1_shrink,v2_shrink);
     return self(v1_shrink,v2_shrink,dir);
  }

  self shrinked_x_map(int diff) const {
     index_type v1_shrink(v1), v2_shrink(v2);
     shrink_x_v(diff,v1_shrink,v2_shrink);
     return self(v1_shrink,v2_shrink,dir);     
  }
  self shrinked_y_map(int diff) const {
     index_type v1_shrink(v1), v2_shrink(v2);
     shrink_y_v(diff,v1_shrink,v2_shrink);
     return self(v1_shrink,v2_shrink,dir);     
  }

private:

  // primary quantities
  index_type  v1,v2;
  index_type dir;
  int n0_;

  // derived quantities
  int nmax_;
  index_type  ll_,ur_;
  int minor_idx,major_idx;
  int minor_incr,major_incr;

  index_type np;
  int rect_size_p;
};

#endif
