#ifndef GRAL_MG_GRUMMP_INPUT_H
#define GRAL_MG_GRUMMP_INPUT_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <assert.h>
#include <vector>

class Bdry2D;

namespace GrAL {

namespace grummp_generator {

class Boundary2D {
  public:
  class Point;
  class Polyline;
  class Circle;
  class Arc;
  class Bezier;
  class Spline;
  class BdryPatch2D;

  enum boundary_marker { B='b', BOUNDARY='b', R='r', REGION='r'};
  typedef int point_handle;
  typedef std::vector<Point> points_container_type;
  typedef std::vector<Polyline> polylines_container_type;
  typedef std::vector<Circle> circles_container_type;
  typedef std::vector<Arc> arcs_container_type;
  typedef std::vector<Bezier> beziers_container_type;
  typedef std::vector<Spline> splines_container_type;
  
  typedef points_container_type::const_iterator    PointIterator;
  typedef polylines_container_type::const_iterator PolylineIterator;
  typedef circles_container_type::const_iterator CircleIterator;
  typedef arcs_container_type::const_iterator ArcIterator;
  typedef beziers_container_type::const_iterator BezierIterator;
  typedef splines_container_type::const_iterator SplineIterator;

  public:  
  inline Boundary2D();  
  inline ~Boundary2D();

  inline int NumOfPoints() const;
  inline int NumOfPolylines() const;
  inline int NumOfCircles() const;
  inline int NumOfArcs() const;
  inline int NumOfBeziers() const;
  inline int NumOfSplines() const;
  inline int NumOfPatches() const;

  inline void addPoint(Point const& point);
  inline void addPoint(const double x, const double y);
  inline void addPoint(const double *coords);

  inline void addPolyline(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const int numofpoints,
                   const point_handle* points);
  inline void addPolyline(Polyline const& polyline);

  inline void addCircle(Circle const& circle);
  inline void addCircle(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const double radius,
                   const point_handle center);

  inline void addArc(Arc const& arc);
  inline void addArc(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const double radius,
                   const point_handle start,
                   const point_handle end);

  inline void addBezier(Bezier const& bezier);
  inline void addBezier(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const point_handle start,
                   const point_handle end,
                   const point_handle ctrl1,
                   const point_handle ctrl2); 

  inline void addSpline(Spline const& spline);
  inline void addSpline(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const int numofpoints,
                   const point_handle* points);


  // iteration: half open STL ranges
  inline PointIterator FirstPoint() const;
  inline PointIterator EndPoint() const;
  
  inline PolylineIterator FirstPolyline() const;
  inline PolylineIterator EndPolyline() const;

  inline CircleIterator FirstCircle() const;
  inline CircleIterator EndCircle() const;

  inline ArcIterator FirstArc() const;
  inline ArcIterator EndArc() const;

  inline BezierIterator FirstBezier() const;
  inline BezierIterator EndBezier() const;

  inline SplineIterator FirstSpline() const;
  inline SplineIterator EndSpline() const;

  void SetupBdry2D(::Bdry2D &bdry) const;

  private:
    std::vector<Point> points;
    std::vector<Polyline> polylines;
    std::vector<Circle> circles;
    std::vector<Arc> arcs;
    std::vector<Bezier> beziers;
    std::vector<Spline> splines;
    
    void SetupPolyline(::Bdry2D &bdry, Polyline const & polyline, 
                          int &iBdryPatch) const;
    void SetupCircle(::Bdry2D &bdry, Circle const & circle, 
                          int &iBdryPatch) const;
    void SetupArc(::Bdry2D &bdry, Arc const & arc, 
                          int &iBdryPatch) const;
    void SetupBezier(::Bdry2D &bdry, Bezier const & bezier, 
                          int &iBdryPatch) const;
    void SetupSpline(::Bdry2D &bdry, Spline const & spline, 
                          int &iBdryPatch) const;
  public:

  //-------------------------------------------------------------------
  // Definition of the local classes
  //-------------------------------------------------------------------
  class Point {
    private:
      double _coords[2];
      point_handle _handle;
    public:
      inline Point();
      inline Point(const double x, const double y);
      inline Point(const double *coords);
      inline ~Point(); 

      inline double x() const;
      inline void x(const double x);
      inline double y() const; 
      inline void y(const double y);
      inline void coords(const double *coords);
      inline const double* coords() const;
      inline point_handle handle() const;
  }; 

  class BdryPatch2D {
    public: 
      typedef Boundary2D boundary_type;
      typedef boundary_type  anchor_type;
    public:
      inline BdryPatch2D(Boundary2D const & boundary, 
                  const boundary_marker leftsidetype, 
                  const int leftnumber, 
                  const boundary_marker rightsidetype,
                  const int rightnumber);
      virtual ~BdryPatch2D(); 
      void SetupPatchMarkers(int &iBCL, int &iBCR, 
                             int& iRegL, int &iRegR) const;
      inline boundary_type const&  TheBoundary() const;
      inline anchor_type const& TheAnchor() const; 
    protected:
      Boundary2D const* _boundary;
      boundary_marker _leftsidetype; 
      int _leftnumber; 
      boundary_marker _rightsidetype;
      int _rightnumber;
    private:
       BdryPatch2D(); // not implemented
  };

  //-------------------------------------------------------------------
  //  Polyline
  //-------------------------------------------------------------------
  class Polyline : public BdryPatch2D {
    public:
      typedef std::vector<point_handle> handles_container_type;
      typedef handles_container_type::const_iterator PointHandleIterator;
      
    public:
      inline Polyline(Boundary2D const& boundary,
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber);
      inline Polyline(Boundary2D const& boundary,
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber, 
                      const int numofpoints, 
                      const point_handle *handles);
      virtual ~Polyline();  

      inline void addPoint(const point_handle handle);
      inline void setPoints(const int numofpoints, const point_handle* handles);

      inline int NumOfPoints() const;
      inline PointHandleIterator FirstPointHandle() const;
      inline PointHandleIterator EndPointHandle() const;
      
    private:
      Polyline(); // not implemented
      handles_container_type _handles;
  };

  //-------------------------------------------------------------------
  //  Circle 
  //-------------------------------------------------------------------
  class Circle : public BdryPatch2D {
    public:
      inline Circle(Boundary2D const& boundary, 
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber, 
                      const double radius=0.0,
                      const point_handle center=0); 

      inline void setCenter(const point_handle handle);
      inline void setRadius(const double radius);
      inline point_handle getCenter() const;
      inline double getRadius() const;
      
    private:
      Circle(); // not implemented
      point_handle _center;
      double _radius;
  };

  //-------------------------------------------------------------------
  //  Arc 
  //-------------------------------------------------------------------
  class Arc : public BdryPatch2D {
    public:
      inline Arc(Boundary2D const& boundary, 
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber, 
                      const double radius = 0.0,
                      const point_handle start = 0, 
                      const point_handle end = 0) :
             Boundary2D::BdryPatch2D(boundary, leftsidetype, leftnumber, 
             rightsidetype, rightnumber), _start(start), _end(end), _radius(radius) {}

      inline void setStartPoint(const point_handle handle) { _start = handle;} 
      inline void setEndPoint(const point_handle handle) { _end = handle; }
      inline void setRadius(const double radius) { _radius = radius; }
      inline point_handle getStartPoint() const {return _start;}
      inline point_handle getEndPoint() const {return _end;}
      inline double getRadius() const {return _radius;}
      
    private:
      Arc(); // not implemented
      point_handle _start;
      point_handle _end;
      double _radius;
  };

  class Bezier : public BdryPatch2D {
    public:
      inline Bezier(Boundary2D const& boundary, 
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber, 
                      const point_handle start = 0, 
                      const point_handle end = 0,
                      const point_handle ctrl1 = 0,
                      const point_handle ctrl2 = 0) :
             Boundary2D::BdryPatch2D(boundary, leftsidetype, leftnumber, 
             rightsidetype, rightnumber), _start(start), _end(end), _ctrl1(ctrl1),
                                          _ctrl2(ctrl2) {}

      inline void setStartPoint(const point_handle handle) { _start = handle;} 
      inline void setEndPoint(const point_handle handle) { _end = handle; }
      inline void setCtlr1(const point_handle handle) { _ctrl1 = handle; }
      inline void setCtrl2(const point_handle handle) { _ctrl2 = handle; }
      inline point_handle getStartPoint() const {return _start;}
      inline point_handle getEndPoint() const {return _end;}
      inline point_handle getCtrl1() const {return _ctrl1;}
      inline point_handle getCtrl2() const {return _ctrl2;}
      
    private:
      Bezier(); // not implemented
      point_handle _start;
      point_handle _end;
      point_handle _ctrl1;
      point_handle _ctrl2;  
  };

  //-------------------------------------------------------------------
  //  Spline
  //-------------------------------------------------------------------
  class Spline : public Polyline  {
    public:
      inline Spline(Boundary2D const& boundary,
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber) : Polyline(boundary,leftsidetype, leftnumber,
                                                        rightsidetype, rightnumber) {};
      inline Spline(Boundary2D const& boundary,
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber, 
                      const int numofpoints, 
                      const point_handle *handles) : Polyline(boundary, leftsidetype, 
                                                        leftnumber,
                                                        rightsidetype, rightnumber,
                                                        numofpoints, handles) {};
  };

}; 

inline 
Boundary2D::Boundary2D() {
  // empty 
}  

inline Boundary2D::~Boundary2D() {
  // empty  
}

inline int 
Boundary2D::NumOfPoints() const {
  return points.size(); 
}

inline int 
Boundary2D::NumOfPolylines() const {
  return polylines.size();  
}

inline int 
Boundary2D::NumOfCircles() const {
  return circles.size();  
}

inline int 
Boundary2D::NumOfArcs() const {
  return arcs.size();  
}

inline int 
Boundary2D::NumOfBeziers() const {
  return beziers.size();  
}

inline int 
Boundary2D::NumOfSplines() const {
  return splines.size();  
}
inline int 
Boundary2D::NumOfPatches() const {
  return NumOfPolylines() + NumOfCircles() + NumOfArcs() + NumOfBeziers() + 
         NumOfSplines(); 
}

inline void 
Boundary2D::addPoint(Point const& point) {
  points.push_back(point); 
}

inline void 
Boundary2D::addPoint(const double x, const double y) {
  points.push_back(Point(x,y)); 
}

inline void 
Boundary2D::addPoint(const double *coords) {
  points.push_back(Point(coords)); 
}

inline void 
Boundary2D::addCircle(Circle const& circle) {
  circles.push_back(circle);  
}

inline void 
Boundary2D::addCircle(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const double radius,
                   const point_handle center) {
  circles.push_back(Circle(*this, leftsidetype, leftnumber, rightsidetype,
        rightnumber,radius, center));  
}



inline void 
Boundary2D::addPolyline(Polyline const& polyline) {
  polylines.push_back(polyline);  
}

inline void 
Boundary2D::addPolyline(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const int numofpoints,
                   const point_handle* handles) {
  polylines.push_back(Polyline(*this, leftsidetype, leftnumber, rightsidetype,
        rightnumber, numofpoints, handles));
}

inline void 
Boundary2D::addArc(Arc const& arc) {
  arcs.push_back(arc);  
}

inline void 
Boundary2D::addArc(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const double radius,
                   const point_handle start,
                   const point_handle end) {
  arcs.push_back(Arc(*this, leftsidetype, leftnumber, rightsidetype,
        rightnumber,radius, start, end));  
}

inline void 
Boundary2D::addBezier(Bezier const& bezier) {
  beziers.push_back(bezier);  
}

inline void 
Boundary2D::addBezier(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const point_handle start,
                   const point_handle end,
                   const point_handle ctrl1,
                   const point_handle ctrl2) {
  beziers.push_back(Bezier(*this, leftsidetype, leftnumber, rightsidetype,
        rightnumber, start, end, ctrl1, ctrl2));  
}

inline void 
Boundary2D::addSpline(Spline const& spline) {
  splines.push_back(spline);  
}

inline void 
Boundary2D::addSpline(const boundary_marker leftsidetype, 
                   const int leftnumber, 
                   const boundary_marker rightsidetype,
                   const int rightnumber,
                   const int numofpoints,
                   const point_handle* handles) {
  splines.push_back(Spline(*this, leftsidetype, leftnumber, rightsidetype,
        rightnumber, numofpoints, handles));
}

inline Boundary2D::PointIterator 
Boundary2D::FirstPoint() const {
  return points.begin(); 
}

inline Boundary2D::PointIterator 
Boundary2D::EndPoint() const {
  return points.end(); 
}
  
inline Boundary2D::PolylineIterator 
Boundary2D::FirstPolyline() const {
  return polylines.begin(); 
}

inline Boundary2D::PolylineIterator 
Boundary2D::EndPolyline() const {
  return polylines.end(); 
}

inline Boundary2D::CircleIterator 
Boundary2D::FirstCircle() const {
  return circles.begin(); 
}

inline Boundary2D::CircleIterator 
Boundary2D::EndCircle() const {
  return circles.end(); 
}

inline Boundary2D::ArcIterator 
Boundary2D::FirstArc() const {
  return arcs.begin(); 
}

inline Boundary2D::ArcIterator 
Boundary2D::EndArc() const {
  return arcs.end(); 
}

inline Boundary2D::BezierIterator 
Boundary2D::FirstBezier() const {
  return beziers.begin(); 
}

inline Boundary2D::BezierIterator 
Boundary2D::EndBezier() const {
  return beziers.end(); 
}

inline Boundary2D::SplineIterator 
Boundary2D::FirstSpline() const {
  return splines.begin(); 
}

inline Boundary2D::SplineIterator 
Boundary2D::EndSpline() const {
  return splines.end(); 
}

// -------------------- Point

inline 
Boundary2D::Point::Point() {
  _coords[0]=0.0;
  _coords[1]=0.0;
}

inline 
Boundary2D::Point::Point(const double x, const double y) {
  _coords[0]=x;
  _coords[1]=y;
}

inline 
Boundary2D::Point::Point(const double *coords) {
  _coords[0] = coords[0];
  _coords[1] = coords[1];
}

inline 
Boundary2D::Point::~Point() {
  // empty
}

inline double 
Boundary2D::Point::x() const {
  return _coords[0];
}

inline void 
Boundary2D::Point::x(const double x) {
  _coords[0] = x;
}

inline double 
Boundary2D::Point::y() const {
  return _coords[1]; 
}

inline void 
Boundary2D::Point::y(const double y) {
  _coords[1] = y;
}

inline void 
Boundary2D::Point::coords(const double *coords) {
  _coords[0] = coords[0];
  _coords[1] = coords[1]; 
}

inline const double* 
Boundary2D::Point::coords() const {
  return _coords; 
}

inline Boundary2D::point_handle
Boundary2D::Point::handle() const {
  return _handle;
}

// ---------------------- BdryPatch2D
 
inline 
Boundary2D::BdryPatch2D::BdryPatch2D(Boundary2D const & boundary,
                                     const boundary_marker leftsidetype, 
                                     const int leftnumber, 
                                     const boundary_marker rightsidetype,
                                     const int rightnumber) : 
                                     _boundary(&boundary), 
                                     _leftsidetype(leftsidetype), 
                                     _leftnumber(leftnumber), 
                                     _rightsidetype(rightsidetype),
                                     _rightnumber(rightnumber) {
 // empty
}

inline Boundary2D::BdryPatch2D::boundary_type const&  
Boundary2D::BdryPatch2D::TheBoundary() const {
  return *_boundary;  
}

inline Boundary2D::BdryPatch2D::anchor_type const& 
Boundary2D::BdryPatch2D::TheAnchor() const {
  return TheBoundary(); 
} 
 
// ----------------------- Polyline

inline 
Boundary2D::Polyline::Polyline(
                      Boundary2D const& boundary, 
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber) : 
                   Boundary2D::BdryPatch2D(boundary, leftsidetype, leftnumber, 
                       rightsidetype, rightnumber) {
  // empty
} 

inline 
Boundary2D::Polyline::Polyline(
                      Boundary2D const& boundary, 
                      const boundary_marker leftsidetype, 
                      const int leftnumber, 
                      const boundary_marker rightsidetype,
                      const int rightnumber,
                      const int numofpoints,
                      const point_handle* handles) : 
                   Boundary2D::BdryPatch2D(boundary, leftsidetype, leftnumber, 
                       rightsidetype, rightnumber) {
  for (int i=0; i<numofpoints; ++i) {
     _handles.push_back(handles[i]);
  }
} 

inline void 
Boundary2D::Polyline::addPoint(const point_handle handle) {
  _handles.push_back(handle); 
}

inline void 
Boundary2D::Polyline::setPoints(const int numofpoints, 
                                const point_handle* handles) {
  _handles.clear();
  for (int i=0; i<numofpoints; ++i) {
     _handles.push_back(handles[i]);
  }
}

inline int 
Boundary2D::Polyline:: NumOfPoints() const {
  return _handles.size();  
} 

inline Boundary2D::Polyline::PointHandleIterator 
Boundary2D::Polyline::FirstPointHandle() const {
  return _handles.begin(); 
}

inline Boundary2D::Polyline::PointHandleIterator 
Boundary2D::Polyline::EndPointHandle() const {
  return _handles.end(); 
}

//----------------------------------- Circle
inline 
Boundary2D::Circle::Circle(Boundary2D const& boundary, 
                    const boundary_marker leftsidetype, 
                    const int leftnumber, 
                    const boundary_marker rightsidetype,
                    const int rightnumber, 
                    const double radius,
                    const point_handle center) : 
             Boundary2D::BdryPatch2D(boundary, leftsidetype, leftnumber, 
             rightsidetype, rightnumber), _center(center), _radius(radius) {
// empyt
}

inline void 
Boundary2D::Circle::setCenter(const point_handle handle) {
  _center = handle; 
}

inline void 
Boundary2D::Circle::setRadius(const double radius) {
  _radius = radius; 
}

inline Boundary2D::point_handle 
Boundary2D::Circle::getCenter() const {
  return _center; 
}

inline double 
Boundary2D::Circle::getRadius() const {
  return _radius; 
}

} // grummp_generator

} // namespace GrAL
#endif
