#include "Gral/Generation/Triangle/triangle-generator.h"
#include <stdlib.h>

namespace GrAL {

namespace triangle_generator {

extern "C" {

int triunsuitable(double *pa, double *pb, double *pc, double area, 
    void *userData) {
  TriangleGenerator *generator=reinterpret_cast<TriangleGenerator *>(userData);
  return generator->triunsuitable(pa, pb, pc, area); 
}


void incircle_transform(double *apa, double *apb, double *apc, double *apd,
                        double *pa, double *pb, double *pc, double *pd,
                        void *userData, double *inverse) {
  TriangleGenerator *generator=reinterpret_cast<TriangleGenerator *>(userData);
  generator->incircle_transform(apa, apb, apc, apd, 
                                 pa,  pb,  pc,  pd, inverse);
}
}
  
void TriangleGenerator::init_triangulateio(::triangulateio *t) {
  t->numberofpointattributes = 0;
  t->numberofpoints = 0;
  t->numberofregions = 0;
  t->numberofholes = 0;

  t->holelist=NULL;
  t->pointlist=NULL;
  t->pointattributelist=NULL;
  t->pointmarkerlist=NULL;
  t->trianglelist=NULL;
  t->triangleattributelist=NULL;
  t->trianglearealist=NULL;
  t->neighborlist=NULL;
  t->segmentlist=NULL;
  t->segmentmarkerlist=NULL;
  t->edgelist=NULL;
  t->normlist=NULL;
  t->edgemarkerlist=NULL;
  t->regionlist=NULL;
}
  
void TriangleGenerator::setup_vertex_params(TriangleInput & input,
                                            struct ::triangulateio *in) {
  if (use_vertex_params) {
    in->numberofpointattributes = input.NumOfVertexParams();
    int size = in->numberofpoints * in->numberofpointattributes;
    if (size > 0) {
      in->pointattributelist = (REAL *) malloc(size * sizeof(REAL));
      int i = 0;
      for (vertex_iterator v = input.FirstVertex(); 
                           v != input.EndVertex(); ++v) {
        for (int k=0; k<input.NumOfVertexParams(); k++, i++) { 
          in->pointattributelist[i] = v->param(k);
        }  
      }
    }  
  }
}

void TriangleGenerator::setup_vertex_markers(TriangleInput & input,
                                            struct ::triangulateio *in) {
  if (use_vertex_markers) {
    int i=0;
    in->pointmarkerlist = (int *) malloc(in->numberofpoints * sizeof(int));
    for (vertex_iterator v= input.FirstVertex(); v!= input.EndVertex(); 
         ++v, i++) {
      in->pointmarkerlist[i] = v->marker(); 
    }
  }
}

void TriangleGenerator::setup_vertex_coords(TriangleInput & input,
                                            struct ::triangulateio *in) {
  in->numberofpoints = input.NumOfVertices();
  in->pointlist = (REAL *) malloc(in->numberofpoints * 2 * sizeof(REAL));
  int i=0;
  for (vertex_iterator v = input.FirstVertex(); v != input.EndVertex(); 
                 ++v, i+=2) {
    in->pointlist[i] = v->x();
    in->pointlist[i+1] = v->y();
  }
}

void TriangleGenerator::setup_segments(TriangleInput & input,
                                        struct ::triangulateio *in) {
  in->numberofsegments = input.NumOfSegments();
  if (in->numberofsegments > 0) {
    in->segmentlist = (int *) malloc(in->numberofsegments * 2 * sizeof(int));
    int i=0;
    for (segment_iterator s = input.FirstSegment(); s != input.EndSegment(); 
                   ++s, i+=2) {
      in->segmentlist[i] = s->v1();
      in->segmentlist[i+1] = s->v2();
    }
  }
}

void TriangleGenerator::setup_holes(TriangleInput & input,
                                        struct ::triangulateio *in) {
  in->numberofholes = input.NumOfHoles();
  if (in->numberofholes > 0) {
    in->holelist = (REAL *) malloc(in->numberofholes * 2 * sizeof(REAL));
    int i=0;
    for (hole_iterator h = input.FirstHole(); h != input.EndHole(); 
                   ++h, i+=2) {
      in->holelist[i] = h->x();
      in->holelist[i+1] = h->y();
    }
  }
}

void TriangleGenerator::setup_regions(TriangleInput & input,
                                        struct ::triangulateio *in) {
  in->numberofregions = input.NumOfRegions();
  if (in->numberofregions > 0) {
    in->regionlist = (REAL *) malloc(in->numberofregions * 4 * sizeof(REAL));
    int i=0;
    for (region_iterator r = input.FirstRegion(); r != input.EndRegion(); 
                   ++r, i+=4) {
      in->regionlist[i] =   r->x();
      in->regionlist[i+1] = r->y();
      in->regionlist[i+2] = r->param();
      in->regionlist[i+3] = r->maxArea();
    }
  }
}

void TriangleGenerator::setup_input(TriangleInput & input, 
                                    struct ::triangulateio *in) {
  setup_vertex_coords(input, in); 
  setup_vertex_params(input, in);
  setup_vertex_markers(input, in);
  setup_segments(input, in);
  setup_holes(input, in);
  setup_regions(input, in);
}

void TriangleGenerator::triangulate(TriangleInput & input, const char *flags,
                                    TriangleAdapter &output) {
  _options.parse(flags);
  triangulate_impl(input, output);
}

void TriangleGenerator::triangulate(TriangleInput & input, 
                                    TriangleAdapter &output) {
  triangulate_impl(input, output);
}

void TriangleGenerator::triangulate_impl(TriangleInput & input, 
                                         TriangleAdapter &output) {
 
  struct ::triangulateio *in, *out, *vorout;

  in = new struct ::triangulateio;
  out = new struct ::triangulateio;
  vorout = new struct ::triangulateio;
    
  init_triangulateio(in);
  init_triangulateio(out);
  init_triangulateio(vorout);
  
  setup_input(input, in);
  ::triangulate(const_cast<char*>(options()), in, out, vorout, (void*)this);
  output.steal(out);

  /* FIXME remove regionlist and holeslist if switch p was not used 
   * and these pointers were not copied to out*/
  free_input_triangulateio(in);
  free_output_triangulateio(vorout);

  delete in;
  delete vorout;
}

const char* TriangleGenerator::options() {
  char *p = options_string;
  strncpy(options_string, _options.options(), 99);
  p += strlen(options_string);
  *p++ = 'z';
  *p = '\0';
  return options_string;
}

void 
ConstEllipseMapper::setTransformation(const double *lambda_, const double *e_) {
  lambda[0] = lambda_[0];
  lambda[1] = lambda_[1];
  e[0] = e_[0];
  e[1] = e_[1];
  set_transformation(); 
}

void ConstEllipseMapper::setTransformation(double lambda1, double lambda2, double e1, double e2) {
  lambda[0] = lambda1;
  lambda[1] = lambda2;
  e[0] = e1;
  e[1] = e2;
  set_transformation(); 
}

void 
ConstEllipseMapper::set_transformation() {
  double tmp[4];
  tmp[0] = e[0]*e[0];
  tmp[1] = e[1]*e[1];
  tmp[2] = sqrt(lambda[0]);
  tmp[3] = sqrt(lambda[1]);
  m[0] = tmp[2]*tmp[0] + tmp[3]*tmp[1];
  m[1] = (tmp[2]-tmp[3])*e[0]*e[1];
  m[2] = m[1];
  m[3] = tmp[2] *tmp[1] + tmp[3]*tmp[0]; 

  
  double det = m[0]*m[3]-m[1]*m[2];
  if (fabs(det) < 1.0e-12) {
     throw NotInvertibleError(det); 
  } else {
     m_inv[0] = m[3]/det;
     m_inv[1] = -m[1]/det;
     m_inv[2] = -m[2]/det;
     m_inv[3] = m[0]/det;
  }
  printf("In  trans: %f %f %f %f\n", m[0], m[1], m[2], m[3]);
  printf("In  invtrans: %f %f %f %f\n", m_inv[0], m_inv[1], m_inv[2], m_inv[3]);
  
}

} // namespace triangle_generator
} // namespace GrAL
