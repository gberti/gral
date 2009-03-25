
/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Generation/Triangle/triangle-input.h"
#include <stdio.h>

namespace GrAL {

namespace triangle_generator {

int TriangleInput::WritePolyFile(const char* name) {
  FILE *fptr;
  fptr = fopen(name, "w");
  if (fptr == NULL) {
    return -1;
  }
  fprintf(fptr, "%d %d %d %d\n",  vertices.size(), 0, 
                   NumOfVertexParams(), 1); 
  int i=1;
  for(vertex_iterator v=FirstVertex(); v!=EndVertex(); ++v, ++i) {
    fprintf(fptr, "%d %f %f ", i, v->x(), v->y());
     for (int j=0; j<NumOfVertexParams(); ++j) {
       fprintf(fptr, " %f", v->param(j));
     }
    fprintf(fptr, "%d\n", v->marker());  
  }
  fprintf(fptr, "%d %d\n", 0,0);
  fprintf(fptr, "%d\n", 0);
  fclose(fptr);  
  return 1;
}
  
double TriangleInput::Vertex::param(param_index i) const {
        if (i < _params.size()) {
          return _params[i]; 
        } else {   
          return default_param(i);
        } 
}

void TriangleInput::Vertex::param(param_index i, double val) {
       _params.resize(i, default_param(i));
       _params[i] = val;
}

TriangleInput::Vertex::Vertex(TriangleInput *g, vertex_handle handle, double x, double y) : 
      _g(g), _handle(handle), _marker(-1) {
    coords[0] = x;
    coords[1] = y;
}

TriangleInput::Vertex::Vertex(TriangleInput *g, vertex_handle handle, double x, double y,
               int marker, int nparams, const double *params) : _g(g), 
               _handle(handle), _marker(marker) {
    coords[0] = x;
    coords[1] = y;
    for (int i=0; i<nparams; i++) {
       _params.push_back(params[i]);
    }
}  

TriangleInput::Segment::Segment(TriangleInput *g, segment_handle handle, 
                 vertex_handle v1, vertex_handle v2) :
                 _g(g), _handle(handle),_v1(v1), _v2(v2), _marker(-1), 
                 _param(0.0) {}
         
                 TriangleInput::Segment::Segment(TriangleInput *g, segment_handle handle, vertex_handle vv1, 
                 vertex_handle vv2, int marker, double param) : 
                 _g(g), _handle(handle), _v1(vv1), _v2(vv2), 
                 _marker(marker), _param(param) {} 

TriangleInput::Hole::Hole(TriangleInput *g, hole_handle handle, 
    const double x, const double y) : 
      _g(g), _handle(handle) {
    coords[0] = x;
    coords[1] = y;
}

TriangleInput::Region::Region(TriangleInput *g, region_handle handle, 
                 double x, double y, double param, double maxarea) :
                 _g(g), _handle(handle), _param(param), _maxarea(maxarea) {
                       coords[0] = x;
                       coords[1] = y;
                 }


TriangleInput::vertex_handle
TriangleInput::addVertex(Vertex const& v) {
  vertex_handle handle = vertices.size();
  vertices.push_back(Vertex(this, handle, v.x(), v.y()));
  return handle;
}

TriangleInput::vertex_handle
TriangleInput::addVertex(double x, double y) {
  vertex_handle handle = vertices.size();
  vertices.push_back(Vertex(this, handle, x, y));
  return handle;
}

TriangleInput::vertex_handle
TriangleInput:: addVertex(double x, double y, int marker, int nparams, 
                                        const double *params) {   
  vertex_handle handle = vertices.size();
  vertices.push_back(Vertex(this, handle, x, y, marker, nparams, params));
  return handle;
}

TriangleInput::segment_handle
TriangleInput::addSegment(vertex_handle v1, vertex_handle v2) {
  segment_handle handle = segments.size();
  segments.push_back(Segment(this, handle, v1, v2));
  return handle;
}

TriangleInput::segment_handle
TriangleInput::addSegment(vertex_handle v1, vertex_handle v2,
    int marker, double param) {
  segment_handle handle = segments.size();
  segments.push_back(Segment(this, handle, v1, v2, marker, param));
  return handle;
}

TriangleInput::segment_handle
TriangleInput::addSegment(Vertex const& v1, Vertex const& v2) {
  segment_handle handle = segments.size();
  segments.push_back(Segment(this, handle, v1.handle(), v2.handle()));
  return handle;
}

TriangleInput::segment_handle
TriangleInput::addSegment(Vertex const& v1, Vertex const& v2,
    int marker, double param) {
  segment_handle handle = segments.size();
  segments.push_back(Segment(this, handle, v1.handle(), v2.handle(), 
                             marker, param));
  return handle;
}

TriangleInput::hole_handle
TriangleInput::addHole(const double x, const double y) {
  hole_handle handle = holes.size();
  holes.push_back(Hole(this, handle, x, y));
  return handle;
}

TriangleInput::hole_handle
TriangleInput::addHole(Hole const& h) {
  hole_handle handle = holes.size();
  holes.push_back(Hole(this, handle, h.x(), h.y()));
  return handle;
}

TriangleInput::region_handle
TriangleInput::addRegion(const double x, const double y, const double param,
    const double maxarea) {
  region_handle handle = regions.size();
  regions.push_back(Region(this, handle, x, y, param, maxarea));
  return handle;
}

TriangleInput::region_handle
TriangleInput::addRegion(Region const& r) {
  region_handle handle = regions.size();
  regions.push_back(Region(this, handle, r.x(), r.y(), r.param(), r.maxArea()));
  return handle;
}

} // namespace triangle_generator

} // namespace GrAL
