#ifndef NMWR_GB_INDEX_TYPE_H
#define NMWR_GB_INDEX_TYPE_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "compiler-config.h"
#include "algorithm" // STL


struct int_index_type {
  typedef int_index_type self;

  int_index_type() {}
  int_index_type(int xx, int yy) : x(xx), y(yy) {}
  int x,y;

  // arithmetic operators
  self& operator+=(const self& rhs) { x +=rhs.x; y += rhs.y; return *this;}
  self& operator-=(const self& rhs) { x -=rhs.x; y -= rhs.y; return *this;}
  self& operator*=(int         rhs) { x *=rhs;   y *= rhs;   return *this;}

  friend self operator+(const self& lhs, const self& rhs)
    { return self(lhs.x+rhs.x,lhs.y+rhs.y);}
  friend self operator-(const self& lhs, const self& rhs)
    { return self(lhs.x-rhs.x,lhs.y-rhs.y);}
  friend self operator*(const self& lhs, int rhs)
    { return self(lhs.x*rhs,lhs.y*rhs);}
  friend self operator*(int lhs, const self& rhs)
    { return self(lhs*rhs.x,lhs*rhs.y);}
  friend self operator-(const self& rhs)
    { return self(-rhs.x,-rhs.y);}

  int& operator[](int i)       { return (i == 2 ? y : x);}
  int  operator[](int i) const { return (i == 2 ? y : x);}
  
  // comparision operators
  friend bool operator==(const self& ls, const self& rs)
    { return ((ls.x == rs.x) && (ls.y == rs.y));}
  friend bool operator< (const self& ls, const self& rs)
    { return ((ls.x <  rs.x) || ((ls.x == rs.x) && ( ls.y < rs.y)));}
  
  // output operators
  friend ostream& operator<<(ostream& out, const self& idx)
    { return(out << idx.x << ' ' << idx.y); }

  friend istream& operator>>(istream& in, self& idx)
    { return(in >> idx.x >> idx.y); }
};

inline void make_ordered(int_index_type& ll, int_index_type& ur)
/*  make sure that ll really is the lower left corner 

    ur                                        ur
      +-------------+         +-------------+
      |             |         |             |      
      |             |   -->   |             |
      |             |         |             |
      +-------------+ ll    ll+-------------+ 

*/

    {
      if(ll.x > ur.x)
	__STD::swap(ll.x,ur.x);
      if(ll.y > ur.y)
	__STD::swap(ll.y,ur.y);
    }



#endif
