#ifndef GRAL_VTK_GEOMETRY_H
#define GRAL_VTK_GEOMETRY_H

// $LICENSE_CUT_NEC_2007

#include "Geometry/coords.h"
#include "Utility/pre-post-conditions.h"

#include "vtkPoints.h"


namespace GrAL {
namespace vtk {

class coord_type;
class coord_proxy {
private:
  vtkPoints* const points;
  const vtkIdType  index;
  friend class coord_type;
public:
  coord_proxy(vtkPoints* p, vtkIdType id) : points(p), index(id) {}
  inline void operator=(coord_type const& coo);
  inline void operator=(coord_proxy const& coo);
  template<class COORD> void operator=(COORD const& coo);
   
  double  operator[](int i) const { cr(i); return points->GetPoint(index)[i]; }
  void set(double *v) { points->SetPoint(index, v);}
  void inline set(int i, double c); 

  private:
  void cr(int i) const {      
    REQUIRE( 0 <= i && i <  3, "i = " << i << " out of range!\n",1);
  }
};

//! \brief coordinate representation
class coord_type : public array_operators<coord_type, double, 3> {
  double xyz[3];
public:
  coord_type() {}
  coord_type(coord_proxy p)               { init(p.points->GetPoint(p.index));}
  explicit coord_type(double const* xyz_) { init(xyz_);}
  explicit coord_type(double d) { xyz[0] = xyz[1] =xyz[2] = d;}
  coord_type(double x, double y, double z) { 
    xyz[0] = x; 
    xyz[1] = y; 
    xyz[2] = z;}
    coord_type& operator=(coord_proxy p) { 
      init((p.points)->GetPoint(p.index)); 
      return *this;
    }

    double  operator[](int i) const { cr(i); return xyz[i]; }
    double& operator[](int i)       { cr(i); return xyz[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  3, "i = " << i << " out of range!\n",1);
    }
    void init(double const* p)
      { xyz[0] = p[0]; xyz[1] = p[1]; xyz[2] = p[2]; }
};

inline std::ostream&
operator<<(std::ostream& out, coord_type const& p)
{ return (out << p[0] << ' ' << p[1] << ' ' << p[2]);}

inline std::istream&
operator>>(std::istream&  in, coord_type & p)
{ return (in >> p[0] >> p[1] >> p[2]);}

inline std::ostream&
operator<<(std::ostream& out, coord_proxy const& p)
{ return (out << p[0] << ' ' << p[1] << ' ' << p[2]);}

inline std::istream&
operator>>(std::istream&  in, coord_proxy & p) { 
  double c[3];
  in >> c[0] >> c[1] >> c[2];
  p.set(c);
  return in;
}

inline void
coord_proxy::operator=(coord_type const& p) {
  points->SetPoint(index, p[0], p[1], p[2]);
}

inline void
coord_proxy::operator=(coord_proxy const& p) {
  points->SetPoint(index, p[0], p[1], p[2]);
}

inline void
coord_proxy::set(int i, double c) {
  double v[3];
  points->GetPoint(index, v);
  v[i] = c;
  points->SetPoint(index, v);
}; 


template<class COORD>
inline void
coord_proxy::operator= (COORD const& p)
{ 
  typedef point_traits<COORD> pt;
  points->SetPoint(index, pt::x(p), pt::y(p), pt::z(p));
}



} // namespace vtk

template<>
struct point_traits<vtk::coord_type> 
  : public point_traits_fixed_size_array <vtk::coord_type,double,3> {};

template<>
struct point_traits<vtk::coord_proxy> 
  : public point_traits_fixed_size_array <vtk::coord_proxy,double,3> 
{
  typedef Ptype reference_type;
  static void ConstructWithDim(unsigned d, Ptype)
    { REQUIRE( d == 3, "d = " << d, 1); }
};

template<class Q>
inline void assign_point(vtk::coord_proxy p, Q const& q) 
{
 typedef point_traits<vtk::coord_proxy> ptP;
 typedef point_traits<Q> ptQ;

 REQUIRE( (ptP::Dim(p) >= ptQ::Dim(q)), 
	  "Dimension of target point lower than of the source!" 
	  << "(dim(p) = " << ptP::Dim(p)  << ", "
	  << "(dim(q) = " << ptQ::Dim(q), 1);
 
 int iq = ptQ::LowerIndex(q); 
 int ip = ptP::LowerIndex(p);  

 double c[ptP::Dim(p)];
 for(; iq <= ptQ::UpperIndex(q); ++ip, ++iq) {
   c[ip] = q[iq]; 
 }
 // fill missing dimension with zeros;
 for(; ip <= ptP::UpperIndex(p); ++ip) {
   c[ip] = 0.0;
 }
 p.set(c);
}  

} // namespace GrAL

#endif
