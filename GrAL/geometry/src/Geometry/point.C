
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <math.h>
#include "Geometry/point.h"

namespace GrAL {

// CONSTRUCTORS

point::point(const double x1): n(1) {x=new double[n]; x[0]=x1;}

point::point(const double x1, const double y1) : n(2) 
{x=new double[n]; x[0]=x1;x[1]=y1;}

point::point(const double x1, const double y1, const double z1) : n(3)
{x=new double[n]; x[0]=x1;x[1]=y1;x[2]=z1;}

point::point(const point& v) : n(v.n)
{
  if(n > 0) {
    x=new double[n]; 
    for(int i=0;i<=n-1;i++) 
      x[i]=v.x[i]; 
  }
  else
    x = 0;
}


point::point(const double  a[], int nn) : n(nn)
{
  if (n > 0) {
  x=new double[n]; 
  for(int i=0;i<=n-1;i++) 
    x[i]=a[i];
  } 
}

// ASSIGNMENT OPERATORS

point& point::operator = (const point& v)
{
  n = v.n;
  if(x != 0) {
    delete[] x; 
  }
  if(n > 0) {
    x=new double[n]; 
    for(int i=0;i<=n-1;i++) 
      x[i]=v.x[i];
  }
  else
    x = 0;

  return *this; 
}
 

point& point::operator += ( const point& v )
{ 
  for (int i= 0;i<=n-1;i++)
    x[i] += v.x[i];
  return *this;
}

point& point::operator -= ( const point& v )
{ 
  for (int i= 0;i<=n-1;i++)
    x[i] -= v.x[i];
  return *this;
}


point& point::operator *= ( const double d )
{ 
  for (int i= 0;i<=n-1;i++)
    x[i] *= d;
  return *this;
}


point& point::operator /= ( const double d )
{ 
  for (int i= 0;i<=n-1;i++)
    x[i] /= d;
  return *this;
}


// SPECIAL FUNCTIONS

double point::length() const
{ return sqrt(length2()); }

double point::length2() const
{ 
  double sum = 0;
  for(int i=0;i<=n-1;i++)
    sum += x[i]*x[i];
  return sum;
}

point  point::normalize() const
{ 
 double l = length();
 REQUIRE((l!=0.0),"point::normalize: point is zero!\n",1);

 point p(*this);
 p /= l; 
 return p; 
}

// FRIENDS

point operator - (const point& a)
{ 
 point p(a.n,point::no_init);
 for(int i=1;i<=a.n;i++)
   p[i] = -a[i];
 return p;
}

point operator + (const point& a, const point& b)
{ 
  REQUIRE((a.n == b.n),
	  "point a+b: dim a == " << a.n << " != dim b == " << b.n << "!",1);
 point p(a.n,point::no_init);
 for(int i=1;i<=a.n;i++)
   p[i] = a[i]+b[i];
 return p;
}

point operator - (const point& a, const point& b)
{ 
  REQUIRE((a.n == b.n),
	  "point a-b: dim a == " << a.n << " != dim b == " << b.n << "!",1);
 point p(a.n,point::no_init);
 for(int i=1;i<=a.n;i++)
   p[i]=a[i]-b[i];
 return p;
}

//point operator , (const point& a, const point& b)
point combine(const point& a, const point& b)
{ 
  point p(a.n+b.n,point::no_init);
 for(int i=1;i<=a.n;i++)
   p[i]=a[i];
 for(int k=1;k<=b.n;k++)
   p[k+a.n]=b[k];
 return p;
}

point operator * (const point& a, const double d)
{ 
 point p(a.n,point::no_init);
 for(int i=1;i<=a.n;i++)
   p[i]=d*a[i];
 return p;
}

point operator * (const double d, const point& a)
{ return a*d; }

point operator / (const point& a, const double d)
{ return(a*(1.0/d));}


double operator * (const point& a, const point& b)
{
  REQUIRE((a.n == b.n),
	  "point a*b: dim a == " << a.n << " != dim b == " << b.n << "!",1);
  double sum = 0;
  for(int i=1;i<=a.n;i++)
    sum += a[i]*b[i];
  return sum;
}

std::ostream& operator << (::std::ostream& s, const point& v)
{ 
  for(int i=1;i<=v.n;i++)
    s << v[i] << " ";
  return s;
}

std::istream& operator >> (::std::istream& s, point& v) {
  for(int i =1;i<=v.n;i++)
    s >> v[i];
  return s;
}

void swap(point& a, point& b)
{ point tmp(a); a = b; b = tmp; }

} // namespace GrAL

