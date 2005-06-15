#include "Gral/Generation/Triangle/triangle-adapter.h"

namespace GrAL {

namespace triangle_adapter {
  
void free_input_triangulateio(struct ::triangulateio *t) {
  free(t->pointlist);
  free(t->pointattributelist);
  free(t->pointmarkerlist);
  free(t->trianglelist);
  free(t->triangleattributelist);
  free(t->trianglearealist);
  free(t->neighborlist);
  free(t->segmentlist);
  free(t->segmentmarkerlist);
  free(t->edgelist);
  free(t->edgemarkerlist);
  free(t->normlist);
}

void free_output_triangulateio(struct ::triangulateio *t) {
  free(t->pointlist);
  free(t->pointattributelist);
  free(t->pointmarkerlist);
  free(t->trianglelist);
  free(t->triangleattributelist);
  free(t->trianglearealist);
  free(t->neighborlist);
  free(t->segmentlist);
  free(t->segmentmarkerlist);
  free(t->edgelist);
  free(t->edgemarkerlist);
  free(t->normlist);
  free(t->holelist);
  free(t->regionlist);
}

  TriangleAdapter::SD::SD() {
  the_archetype[0] = archetype_type(3);
  } 
  TriangleAdapter::SD TriangleAdapter::sd;

} // namespace triangle_adapter
} // namespace GrAL
