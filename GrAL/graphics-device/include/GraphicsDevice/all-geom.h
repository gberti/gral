#ifndef NMWR_GB_GRAPHICSDEVICE_RGEOM_H
#define NMWR_GB_GRAPHICSDEVICE_RGEOM_H

// $LICENSE

#include <string>

#include "Geometry/point-traits.h"
#include "GraphicsDevice/renderable-geom.h"


class rgb_color;
class GraphicsDevice;

extern GraphicsDevice OOGLDevice(ostream&);
extern GraphicsDevice OOGLDevice(const std::string& name);


extern RenderableGeom RFileGeom(std::string const&);

// g++ does not correctly take the special version RSegment(point,point)
// instead of the template version, therefore the hack with "_1" - suffix.



extern RenderableGeom RSegment_1(RenderableGeom::coord_type const& c1,
                                 RenderableGeom::coord_type const& c2);

extern RenderableGeom RSegment(RenderableGeom::coord_type const& c1, 
                               RenderableGeom::coord_type const& c2);

inline RenderableGeom RSegment(float x1, float y1, float z1,
			       float x2, float y2, float z2)
{
  return RSegment_1(RenderableGeom::coord_type(x1,y1,z1),
                  RenderableGeom::coord_type(x2,y2,z2));
}

template<class P>
inline RenderableGeom RSegment(const P& c1, const P& c2)
{ 
  typedef point_traits<P> pt;
  return RSegment_1(RenderableGeom::coord_type(pt::x(c1), pt::y(c1), pt::z(c1)),
		    RenderableGeom::coord_type(pt::x(c2), pt::y(c2), pt::z(c2)));
}

// hack: sometimes g++ doesn't take the above template definition.
/*
template<class P>
inline RenderableGeom RSegmentT(const P& c1, const P& c2)
{ 
  typedef  RenderableGeom::coord_type_traits<P> pt;
  return RSegment_1(RenderableGeom::coord_type(pt::x(c1), pt::y(c1), pt::z(c1)),
		    RenderableGeom::coord_type(pt::x(c2), pt::y(c2), pt::z(c2)));
}
*/


extern RenderableGeom RTriangle(RenderableGeom::coord_type const& c1, 
                                RenderableGeom::coord_type const& c2, 
				RenderableGeom::coord_type const& c3);

extern RenderableGeom RTriangle_1(RenderableGeom::coord_type const& c1, 
                                  RenderableGeom::coord_type const& c2, 
                                  RenderableGeom::coord_type const& c3);

template<class P>
inline RenderableGeom RTriangle(const P& c1, const P& c2, const P& c3) {
 typedef point_traits<P> pt;
 return RTriangle_1(RenderableGeom::coord_type(pt::x(c1),pt::y(c1),pt::z(c1)),
                    RenderableGeom::coord_type(pt::x(c2),pt::y(c2),pt::z(c2)),
                    RenderableGeom::coord_type(pt::x(c3),pt::y(c3),pt::z(c3)));
}


extern RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, 
                                     RenderableGeom::coord_type const& c2,
				     RenderableGeom::coord_type const& c3, 
                                     RenderableGeom::coord_type const& c4);

extern RenderableGeom RQuadrilateral_1(RenderableGeom::coord_type const& c1, 
                                       RenderableGeom::coord_type const& c2,
				       RenderableGeom::coord_type const& c3, 
                                       RenderableGeom::coord_type const& c4);

template<class P>
inline RenderableGeom RQuadrilateral(const P& c1, const P& c2, const P& c3, const P& c4) {
 typedef point_traits<P> pt;

 return RQuadrilateral_1(RenderableGeom::coord_type(pt::x(c1),pt::y(c1),pt::z(c1)),
                         RenderableGeom::coord_type(pt::x(c2),pt::y(c2),pt::z(c2)),
                         RenderableGeom::coord_type(pt::x(c3),pt::y(c3),pt::z(c3)),
                         RenderableGeom::coord_type(pt::x(c4),pt::y(c4),pt::z(c4)));
}

extern RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, 
                                     RenderableGeom::coord_type const& c2,
				     RenderableGeom::coord_type const& c3, 
                                     RenderableGeom::coord_type const& c4,
				     const rgb_color& c);

extern RenderableGeom RQuadrilateral_1(RenderableGeom::coord_type const& c1, 
                                       RenderableGeom::coord_type const& c2,
				       RenderableGeom::coord_type const& c3, 
                                       RenderableGeom::coord_type const& c4,
				       const rgb_color& c);
template<class P>
inline RenderableGeom RQuadrilateral(const P& c1, const P& c2, 
				     const P& c3, const P& c4,
				     const rgb_color& c) {
  typedef point_traits<P> pt;
  
  return RQuadrilateral_1(RenderableGeom::coord_type(pt::x(c1),pt::y(c1),pt::z(c1)),
			  RenderableGeom::coord_type(pt::x(c2),pt::y(c2),pt::z(c2)),
			  RenderableGeom::coord_type(pt::x(c3),pt::y(c3),pt::z(c3)),
			  RenderableGeom::coord_type(pt::x(c4),pt::y(c4),pt::z(c4)), c);
}

extern RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, const rgb_color& color1, 
				     RenderableGeom::coord_type const& c2, const rgb_color& color2, 
				     RenderableGeom::coord_type const& c3, const rgb_color& color3, 
				     RenderableGeom::coord_type const& c4, const rgb_color& color4);

extern RenderableGeom RQuadrilateral_1(RenderableGeom::coord_type const& c1, const rgb_color& color1, 
				       RenderableGeom::coord_type const& c2, const rgb_color& color2, 
				       RenderableGeom::coord_type const& c3, const rgb_color& color3, 
				       RenderableGeom::coord_type const& c4, const rgb_color& color4);
template<class P>
inline RenderableGeom RQuadrilateral(const P& c1, const rgb_color& color1, 
				     const P& c2, const rgb_color& color2, 
				     const P& c3, const rgb_color& color3, 
				     const P& c4, const rgb_color& color4) {
 typedef point_traits<P> pt;
 return RQuadrilateral_1(RenderableGeom::coord_type(pt::x(c1),pt::y(c1),pt::z(c1)),color1,
                         RenderableGeom::coord_type(pt::x(c2),pt::y(c2),pt::z(c2)),color2,
                         RenderableGeom::coord_type(pt::x(c3),pt::y(c3),pt::z(c3)),color3,
                         RenderableGeom::coord_type(pt::x(c4),pt::y(c4),pt::z(c4)),color4);
}


extern RenderableGeom RCircle  (RenderableGeom::coord_type const& c1, double r);
extern RenderableGeom RCircle_1(RenderableGeom::coord_type const& c1, double r);

template<class P>
inline RenderableGeom RCircle(const P& c1, double r)
{
 typedef point_traits<P> pt;
 RenderableGeom::coord_type pp =
   RenderableGeom::coord_type(pt::x(c1),pt::y(c1),pt::z(c1));
 return RCircle_1(pp, r);
}

extern RenderableGeom REllipse(RenderableGeom::coord_type const& c1, double r1, double r2);


extern RenderableGeom RCube(RenderableGeom::coord_type const& c1, 
                            RenderableGeom::coord_type const& c2, 
                            RenderableGeom::coord_type const& c3, 
                            RenderableGeom::coord_type const& c4, 
                            RenderableGeom::coord_type const& c5, 
                            RenderableGeom::coord_type const& c6, 
                            RenderableGeom::coord_type const& c7,
                            RenderableGeom::coord_type const& c8);

extern RenderableGeom RCube(double a);


extern RenderableGeom RSimplex(RenderableGeom::coord_type const& c1, 
                               RenderableGeom::coord_type const& c2, 
			       RenderableGeom::coord_type const& c3, 
                               RenderableGeom::coord_type const& c4);
extern RenderableGeom RSimplex(double a);


extern RenderableGeom RCylinder(double r, double h);
extern RenderableGeom RCone(double r1, double r2, double h);

extern RenderableGeom RBall(double r, RenderableGeom::coord_type const& m);
extern RenderableGeom RSphere(double r, RenderableGeom::coord_type const& m);

extern RenderableGeom RTorus(double r1, double r2, char style='n');

extern RenderableGeom RLetter(char ch,RenderableGeom::coord_type const& offset);

extern RenderableGeom RWord(const std::string& str, 
			    RenderableGeom::coord_type const& c1, 
			    RenderableGeom::coord_type const& c2, 
			    RenderableGeom::coord_type const& c3);

extern RenderableGeom RWord_1(const std::string& str, 
                              RenderableGeom::coord_type const& c1, 
                              RenderableGeom::coord_type const& c2, 
                              RenderableGeom::coord_type const& c3);
template<class P>
inline RenderableGeom RWord(const std::string& s, const P& c1, const P& c2, const P& c3)
{
  typedef point_traits<P> pt;
  typedef RenderableGeom::coord_type coord_type;
  coord_type pp1=coord_type(pt::x(c1),pt::y(c1),pt::z(c1));
  coord_type pp2=coord_type(pt::x(c2),pt::y(c2),pt::z(c2));
  coord_type pp3=coord_type(pt::x(c3),pt::y(c3),pt::z(c3));
  return RWord_1(s,pp1,pp2,pp3);
}

#endif
