#ifndef NMWR_GB_GRAL_CARTESIAN2D_INDEXMAP_H
#define NMWR_GB_GRAL_CARTESIAN2D_INDEXMAP_H

// $LICENSE

/*! \file
 */


#include <iostream>
#include "Utility/pre-post-conditions.h"
#include "Container/tuple.h"

namespace GrAL {

/*! \brief Mapping between integer intervals and integer rectangles

    \ingroup cartesian2dmodule

 This class implements a bijective map between
 the subset [n0,n0+size-1] of Z and the subset
 [llx,urx]x[lly,ury] of Z^2 via the x-major-scheme
 (y varies fastest) :

  \code
 (llx,ury)                                 (urx,ury)
          +  n0+ny                        +  n0+(nx*ny) -1
          .
          .

          +  n0+1

          +  n0     + n0+ny+1   ...       +
 (llx,lly)                                  (urx,lly)
  \endcode

   \todo This could probably replace with the general n-dimensional
   index_map_nd.
*/



class xmjr_indexmap2D {
public:
  typedef tuple<int,2>   index_type; 
 
  /*! simple range type
   */
  struct range {
    int x,y;
    range(int xx, int yy) : x(xx), y(yy) {}
    friend ::std::ostream& operator<<(::std::ostream& out, const range& r) {
      return (out << '[' << r.x << ',' << r.y << ']');
    }
  };



 xmjr_indexmap2D(index_type UR = index_type(0,0))
   : ll_(0,0),ur_(UR), n0_(0) { init();}
 xmjr_indexmap2D(int urx, int ury)
   : ll_(0,0),ur_(urx,ury), n0_(0) { init();}

 xmjr_indexmap2D(const index_type& LL,const index_type& UR, int off = 0)
   : ll_(LL), ur_(UR), n0_(off) { init();}

  void init() { 
    nx = ur_.x()-ll_.x()+1;  
    ny = ur_.y()-ll_.y()+1;  
    rect_size = nx*ny;
  }
  
  int x0()   const { return ll_.x();}
  int xmax() const { return ur_.x();}
  int y0()   const { return ll_.y();}
  int ymax() const { return ur_.y();}
  int n0()   const { return n0_;}
  int nmax() const { return n0() + range_size() -1;}

  const index_type& ll() const {return ll_;}
  int llx() const {return ll_.x();}
  int lly() const {return ll_.y();}
  const index_type& ur() const {return ur_;}
  int urx() const {return ur_.x();}
  int ury() const {return ur_.y();}


  bool IsInRange(int x, int y) const { 
    return ((ll_.x() <= x) && ( x <= ur_.x()) &&(ll_.y() <= y) && ( y <= ur_.y()));
  }
  bool IsInRange(const index_type& i) const { 
    return IsInRange(i.x(),i.y());
  }
  bool IsInRange(int i) const {
    return((n0() <= i) && ( i <= n0() -1 + range_size()));
  }

  int number(int x, int y) const { 
    REQUIRE(IsInRange(x,y),
	    "index (x,y) = (" << x << "," << y << ") not in range "
	    << range(ll_.x(),ur_.x()) << "x" << range(ll_.y(),ur_.y()) << "!",1);
    return (n0() + (x-x0())*ny + (y-y0()) );
  }
  int number(const index_type& idx) const { return number(idx.x(),idx.y());}
  int offset(int x, int y) const { return number(x,y) - n0();}
  int offset(const index_type ij) const { return number(ij) - n0();}

  index_type index(int i) const {
    REQUIRE(IsInRange(i),
	    "number not in range " << range(n0(),n0() + range_size() -1) << ": i = " << i,1);
    int i0 = i - n0();
    int x = i0 / ny;
    int y = i0 - x*ny;
    return index_type(x0()+x,y0()+y);
  }
  
  int range_size() const { return rect_size;}

private:
  // a specialized version could set 
  // n0 = 0 and ll_ = (0,0)
  index_type  ll_,ur_;
  int n0_;

  // derived quantities
  int nx,ny;
  int rect_size;
};

} // namespace GrAL 

#endif
