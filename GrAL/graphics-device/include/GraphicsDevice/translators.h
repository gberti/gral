#ifndef NMWR_GB_TRANSLATORS_H
#define NMWR_GB_TRANSLATORS_H

#include <string.h>

#include <strstream.h>
#include "Graphics/transformation.h"
#include "Graphics/attribute.h"
#include "Graphics/renderable-object.h"


// geometric primitives
class geom_sphere;
class geom_triangle;
class geom_ball;
class geom_torus;
class geom_cube;
class geom_cone;
class geom_cylinder;
class geom_simplex;
class geom_quadrilateral;
class geom_color_quad;
class geom_color_bar;
class geom_segment;
class geom_letter;
class geom_word;
class geom_movie;
class geom_circle;
class geom_polygon;


// dies sollte weiter verfeinert werden:
// text-umwandlung -> language-text (oogl, RenderMan,...)
// prozedur-aufrufe -> language-proc (GRAPE,...)

class rlang_internals;

class rendering_language {
private:
  rlang_internals* state;
  //  virtual void empty_buffer();
protected:
  ostream& the_stream();
  Transformation ToDoTrafo() const; // return accumulated Trafos that couldn't be handled directly
  void PushToDoTrafo(const Transformation& T); // compose ToDoTrafo() with T
  void PopToDoTrafo(); // remove last composed trafo
  bool IsToDoTrafo() const; // are there some Trafos accumulated that can't be handled?
public:
  rendering_language(): state(0) {}
  rendering_language(ostream* out);
  virtual ~rendering_language();
  virtual void set_stream(ostream* out); 
  virtual void begin_transformation(const Transformation&) = 0;
  virtual void end_transformation()  = 0;
  virtual void begin_attribute(const Attribute&)  = 0;
  virtual void end_attribute()  = 0;
  virtual void begin_object(const RenderableGeom& obj)  = 0;
  virtual void end_object()  = 0;
//  virtual void begin_group() = 0;
  virtual void begin_group(const string& name) = 0;
  virtual void end_group()   = 0;
  virtual void begin_block()  = 0;
  virtual void end_block()    = 0;
  virtual void pause(double seconds) = 0;
  virtual void read_from_file(const string& file) = 0;
  virtual void filter(const RenderableGeom& Obj);

  virtual void  write_sphere(const geom_sphere& )  = 0;
  virtual void  write_ball(const geom_ball& )  = 0;
  virtual void  write_triangle(const geom_triangle& );
  virtual void  write_torus(const geom_torus& )  = 0;
  virtual void  write_torus_bezier(const geom_torus& )  = 0;
  virtual void  write_cone(const geom_cone& )  = 0;
  virtual void  write_cube(const geom_cube& )  = 0;
  virtual void  write_cylinder(const geom_cylinder& )  = 0;
  virtual void  write_simplex(const geom_simplex& ) = 0;
  virtual void  write_quadrilateral(const geom_quadrilateral& )  = 0;
  virtual void  write_color_quad(const geom_color_quad& )  = 0;
  virtual void  write_segment(const geom_segment& )  = 0;
  virtual void  write_circle(const geom_circle& )  = 0;
  virtual void  write_polygon(const geom_polygon& ) {}

};


#include "Graphics/manips.h"
#include "Graphics/graphics-device.h"
/*
class GraphicsDevice {
private:
 ostream& out;
 rendering_language& L;
public:
 GraphicsDevice(ostream& oo, rendering_language& LL) : out(oo), L(LL) {L.set_stream(&out);}
 rendering_language& Lang() { return L;}
 friend GraphicsDevice& operator<<(GraphicsDevice& G, const RenderableGeom& Obj)
   { G.L.filter(Obj); return(G);}
 friend GraphicsDevice& operator<<(GraphicsDevice& G, const DevManip0& M)
   { M(G.L); return G;}
};



template<class arg>
GraphicsDevice& operator<<(GraphicsDevice& G, const DevManip1<arg>& M)

{
  M.apply(G.Lang());
  return G;
}
*/
#endif
