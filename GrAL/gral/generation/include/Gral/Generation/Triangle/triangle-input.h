#ifndef GRAL_MG_TRIANGLE_INPUT_H
#define GRAL_MG_TRIANGLE_INPUT_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#define REAL double
extern "C" {
#include "triangle.h"
}
#include <assert.h>
#include <vector>

namespace GrAL {

namespace triangle_generator {


struct TriangleInput_types {
     typedef std::vector<double> params_array;
     typedef params_array::iterator param_iterator;
     typedef params_array::size_type param_index;
     typedef unsigned int vertex_handle;
     typedef unsigned int segment_handle;
     typedef unsigned int hole_handle;
     typedef unsigned int region_handle;

//     typedef typename std::vector<Vertex>::_iterator vertex_iterator;
//     typedef typename std::vector<Segment>::iterator segment_iterator;
}; 

class TriangleInput : public TriangleInput_types {
  public:
  inline TriangleInput() {}  


    int WritePolyFile(const char* name); 
      
     //@{ Default input element parameters
     double default_vertex_param(param_index i) const {
        return vertex_params.at(i);
     }

     void default_vertex_param(param_index i, double val) {
       vertex_params.resize(i, 0.0);
       vertex_params[i] = val;
     }

     double default_segment_param() const {
        return _segment_param;
     }

     void default_segment_param(double val) {
        _segment_param = val;
     }

     double default_region_param() const {
        return _region_param;
     }

     void default_region_param(double val) {
       _region_param = val;
     }
     //@}

    inline int NumOfVertexParams() { return vertex_params.size(); }

  
  //-------------------------------------------------------------------
  // Definition of the local classes
  //-------------------------------------------------------------------
  class Vertex : public TriangleInput_types {
    private:
      TriangleInput *_g;
      vertex_handle _handle;
      int _marker;
      double coords[2];
      params_array _params;
     
    public:
      Vertex() : _g(0), _handle(0), _marker(0) {};
      Vertex(TriangleInput *g, vertex_handle handle, double x, double y);
      Vertex(TriangleInput *g, vertex_handle handle, double x, double y,
                          int marker, int nparams, const double *params);   
     ~Vertex() {} 

     double x() const { return coords[0]; }
     void x(double x_) { coords[0] = x_; }
     double y() const { return coords[1]; } 
     void y(double y_) { coords[1] = y_; }

     vertex_handle handle() const {return _handle;}

     int marker() const { return _marker; }
     void marker(int m) { _marker = m; }

     void param(param_index i, double value);
     double param(param_index i) const;

     inline double default_param(param_index i) const {
        return _g->default_vertex_param(i); 
     };
  }; 

  //-------------------------------------------------------------------
  //
  //-------------------------------------------------------------------
  class Segment : public TriangleInput_types {
    private:
      TriangleInput *_g;
      segment_handle _handle;
      vertex_handle _v1;
      vertex_handle _v2;
      int _marker;
      double _param;

    public:
      Segment(TriangleInput *g, segment_handle handle, vertex_handle v1, 
                                                   vertex_handle v2);
      Segment(TriangleInput *g, segment_handle handle, vertex_handle vv1, 
                            vertex_handle vv2, int marker, double param);

      segment_handle handle() {return _handle;}

      vertex_handle v1() const {return _v1;}
      vertex_handle v2() const {return _v2;}

      int marker() const { return _marker; }
      void marker(int m) { _marker = m; }

      void param(double value) { _param = value; }
      double param() const  { return _param; }

      inline double default_param() const {
         return _g->default_segment_param();  
      };

  };

  //-------------------------------------------------------------------
  //
  //-------------------------------------------------------------------
  class Hole : public TriangleInput_types {
    private:
      TriangleInput *_g;
      hole_handle _handle;
      double coords[2];
     
    public:
      Hole() : _g(0), _handle(0) {}
      Hole(TriangleInput *g, hole_handle handle, const double x, 
                                                 const double y);
      ~Hole() {} 

      double x() const { return coords[0]; }
      void x(double x_) { coords[0] = x_; }
      double y() const { return coords[1]; } 
      void y(double y_) { coords[1] = y_; }

      hole_handle handle() const {return _handle;}

  };

  class Region : public TriangleInput_types {
    private:
      TriangleInput *_g;
      region_handle _handle;
      double coords[2];
      double _param;
      double _maxarea;
     
    public:
      Region() : _g(0), _handle(0) {}
      Region(TriangleInput *g, region_handle handle, double x, double y, 
             double param, double maxarea=-1.0);
      ~Region() {} 

      double x() const { return coords[0]; }
      void x(double x_) { coords[0] = x_; }
      double y() const { return coords[1]; } 
      void y(double y_) { coords[1] = y_; }

      region_handle handle() const {return _handle;}

      void param(double value) { _param = value; }
      double param() const { return _param; } 

      void maxArea(double maxarea) { _maxarea = maxarea; }
      double maxArea() const {return _maxarea; } 

      inline double default_param() const {
         return _g->default_region_param(); 
      };

  };

//@{ Mesh constraints creation 
    vertex_handle addVertex(Vertex const& v); 
    vertex_handle addVertex(double x, double y);
    vertex_handle addVertex(double x, double y, int marker, int nparams, 
                                        const double *params);   
     segment_handle addSegment(vertex_handle v1, vertex_handle v2);
     segment_handle addSegment(vertex_handle v1, vertex_handle v2, int marker,
           double param);
     segment_handle addSegment(Vertex const& v1, Vertex const& v2);
     segment_handle addSegment(Vertex const& v1, Vertex const& v2, 
                               int marker, double param);

     hole_handle addHole(const double x, const double y);
     hole_handle addHole(Hole const& hole);
     
     
     region_handle addRegion(const double x, const double y, 
                             const double param, 
                             const double maxarea=-1.0);
     region_handle addRegion(Region const& region);
     //@}
     
     //@{ Access to elements
     Vertex  & vertex(const vertex_handle h) { return vertices[h];}
     Segment  & segment(const segment_handle h) { return segments[h];}
     Hole & hole(const hole_handle h) { return holes[h];}
     Region  & region(const region_handle h) { return regions[h];}
     //@}
     //
     typedef  std::vector<Vertex>::const_iterator vertex_iterator;
     typedef  std::vector<Segment>::const_iterator segment_iterator;
     typedef  std::vector<Hole>::const_iterator hole_iterator;
     typedef  std::vector<Region>::const_iterator region_iterator;

     vertex_iterator FirstVertex() const {return vertices.begin();}
     vertex_iterator EndVertex() const {return vertices.end();}

     segment_iterator FirstSegment() const {return segments.begin();}
     segment_iterator EndSegment() const {return segments.end();}

     hole_iterator FirstHole() const {return holes.begin();}
     hole_iterator EndHole() const {return holes.end();}

     region_iterator FirstRegion() const {return regions.begin();}
     region_iterator EndRegion() const {return regions.end();}

     //@{ @name Size information
     int NumOfVertices() const { return vertices.size(); }
     int NumOfSegments() const { return segments.size(); }
     int NumOfHoles() const { return holes.size(); }
     int NumOfRegions() const { return regions.size(); }
     //@}

  private:
     std::vector<Vertex> vertices;
     std::vector<Segment> segments;
     std::vector<Hole> holes;
     std::vector<Region> regions;

     double _region_param;
     double _segment_param;
     params_array vertex_params; 
}; 

} // triangle_generator

} // namespace GrAL
#endif
