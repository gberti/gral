#ifndef NMWR_GB_RGEOM_H
#define NMWR_GB_RGEOM_H

#include "Geometry/point-traits.h"
#include "forward/string.h"

#include "LinAlg/point.h"
#include "Graphics/renderable-object.h"

class rgb_color;
class GraphicsDevice;
// class RenderableGeom;
// class point;

extern GraphicsDevice OOGLDevice(ostream&);
extern GraphicsDevice OOGLDevice(const string& name);

// g++ does not correctly take the special version RSegment(point,point)
// instead of the template version, therefore the hack with "_1" - suffix.
extern RenderableGeom RSegment_1(const point& c1, const point& c2);
extern RenderableGeom RSegment(const point& c1, const point& c2);

inline RenderableGeom RSegment(float x1, float y1, float z1,
			       float x2, float y2, float z2)
{return RSegment_1(point(x1,y1,z1),point(x2,y2,z2));}

template<class P>
inline RenderableGeom RSegment(const P& c1, const P& c2)
{ 
  typedef  point_traits<P> pt;
  return RSegment_1(point(pt::x(c1), pt::y(c1), pt::z(c1)),
		    point(pt::x(c2), pt::y(c2), pt::z(c2)));
}

// hack: sometimes g++ doesn't take the above template definition.
template<class P>
inline RenderableGeom RSegmentT(const P& c1, const P& c2)
{ 
  typedef  point_traits<P> pt;
  return RSegment_1(point(pt::x(c1), pt::y(c1), pt::z(c1)),
		    point(pt::x(c2), pt::y(c2), pt::z(c2)));
}



extern RenderableGeom RTriangle(const point& c1, const point& c2, 
				const point& c3);

extern RenderableGeom RTriangle_1(const point& c1, const point& c2, 
				   const point& c3);

template<class P>
inline RenderableGeom RTriangle(const P& c1, const P& c2, const P& c3) {
 typedef point_traits<P> pt;
 return RTriangle_1(point(pt::x(c1),pt::y(c1),pt::z(c1)),
		  point(pt::x(c2),pt::y(c2),pt::z(c2)),
		  point(pt::x(c3),pt::y(c3),pt::z(c3)));
}


extern RenderableGeom RQuadrilateral(const point& c1, const point& c2,
				     const point& c3, const point& c4);

extern RenderableGeom RQuadrilateral_1(const point& c1, const point& c2,
				       const point& c3, const point& c4);

template<class P>
inline RenderableGeom RQuadrilateral(const P& c1, const P& c2, const P& c3, const P& c4) {
 typedef point_traits<P> pt;

 return RQuadrilateral_1(point(pt::x(c1),pt::y(c1),pt::z(c1)),
		      point(pt::x(c2),pt::y(c2),pt::z(c2)),
		      point(pt::x(c3),pt::y(c3),pt::z(c3)),
		      point(pt::x(c4),pt::y(c4),pt::z(c4)));
}

extern RenderableGeom RQuadrilateral(const point& c1, const point& c2,
				     const point& c3, const point& c4,
				     const rgb_color& c);
extern RenderableGeom RQuadrilateral_1(const point& c1, const point& c2,
				       const point& c3, const point& c4,
				       const rgb_color& c);
template<class P>
inline RenderableGeom RQuadrilateral(const P& c1, const P& c2, 
				     const P& c3, const P& c4,
				     const rgb_color& c) {
  typedef point_traits<P> pt;
  
  return RQuadrilateral_1(point(pt::x(c1),pt::y(c1),pt::z(c1)),
			  point(pt::x(c2),pt::y(c2),pt::z(c2)),
			  point(pt::x(c3),pt::y(c3),pt::z(c3)),
			  point(pt::x(c4),pt::y(c4),pt::z(c4)), c);
}

extern RenderableGeom RQuadrilateral(const point& c1, const rgb_color& color1, 
				     const point& c2, const rgb_color& color2, 
				     const point& c3, const rgb_color& color3, 
				     const point& c4, const rgb_color& color4);

extern RenderableGeom RQuadrilateral_1(const point& c1, const rgb_color& color1, 
				       const point& c2, const rgb_color& color2, 
				       const point& c3, const rgb_color& color3, 
				       const point& c4, const rgb_color& color4);
template<class P>
inline RenderableGeom RQuadrilateral(const P& c1, const rgb_color& color1, 
				     const P& c2, const rgb_color& color2, 
				     const P& c3, const rgb_color& color3, 
				     const P& c4, const rgb_color& color4) {
 typedef point_traits<P> pt;
  return RQuadrilateral_1(point(pt::x(c1),pt::y(c1),pt::z(c1)),color1,
			  point(pt::x(c2),pt::y(c2),pt::z(c2)),color2,
			  point(pt::x(c3),pt::y(c3),pt::z(c3)),color3,
			  point(pt::x(c4),pt::y(c4),pt::z(c4)),color4);
}


extern RenderableGeom RCircle(const point& c1, double r);
extern RenderableGeom RCircle_1(const point& c1, double r);
template<class P>
inline RenderableGeom RCircle(const P& c1, double r)
{
 typedef point_traits<P> pt;
 point pp=point(pt::x(c1),pt::y(c1),pt::z(c1));
 return RCircle_1(pp, r);
}

extern RenderableGeom REllipse(const point& c1, double r1, double r2);


extern RenderableGeom RCube(const point& c1, const point& c2, const point& c3, 
		     const point& c4, const point& c5, const point& c6, 
		     const point& c7, const point& c8);
extern RenderableGeom RCube(double a);

extern RenderableGeom RSimplex(const point& c1, const point& c2, 
			       const point& c3, const point& c4);
extern RenderableGeom RSimplex(double a);


extern RenderableGeom RCylinder(double r, double h);
extern RenderableGeom RCone(double r1, double r2, double h);

extern RenderableGeom RBall(double r, const point& m);
extern RenderableGeom RSphere(double r, const point& m);

extern RenderableGeom RTorus(double r1, double r2, char style='n');

extern RenderableGeom RLetter(char ch,const point& offset);

extern RenderableGeom RWord(const string& str, 
			    const point& c1, 
			    const point& c2, 
			    const point& c3);
extern RenderableGeom RWord_1(const string& str, 
			    const point& c1, 
			    const point& c2, 
			    const point& c3);
template<class P>
inline RenderableGeom RWord(const string& s, const P& c1, const P& c2, const P& c3)
{
  typedef point_traits<P> pt;
  point pp1=point(pt::x(c1),pt::y(c1),pt::z(c1));
  point pp2=point(pt::x(c2),pt::y(c2),pt::z(c2));
  point pp3=point(pt::x(c3),pt::y(c3),pt::z(c3));
  return RWord_1(s,pp1,pp2,pp3);
}

#endif
