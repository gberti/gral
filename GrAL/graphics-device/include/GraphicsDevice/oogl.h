#ifndef NMWR_GB_GRAPHICSDEVICE_OOGL_H
#define NMWR_GB_GRAPHICSDEVICE_OOGL_H

// $LICENSE


/*----------------------------------------------------------------------------
    oogl.h		

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/rendering-language.h"
/* #include "shapes.h" */


class oogl : public rendering_language {
private:
  oogl(const oogl& ) {}
public:
  oogl(ostream* out);
  oogl() : rendering_language() {}
  virtual void begin_transformation(const Transformation&) ;
  virtual void end_transformation() ;
  virtual void begin_attribute(const Attribute&) ;
  virtual void end_attribute();
  virtual void begin_object(const RenderableGeom& Obj) ;
  virtual void end_object() ;
  virtual void begin_group(const string& name);
  virtual void end_group();
  virtual void begin_block();
  virtual void end_block();
  
  virtual void pause(double seconds);
  virtual void read_from_file(const string& file);
  void  write_attribute(const Attribute& A) ;
  void  write_transformation(const Transformation& T);
  
  void write_sphere(const geom_sphere& s);
  void write_ball(const geom_ball& s);
  void write_triangle(const geom_triangle& s);
  void write_torus(const geom_torus& t);
  void write_torus_bezier(const geom_torus& t);
  void write_cone(const geom_cone& t);
  void write_cube(const geom_cube& t);
  void write_cylinder(const geom_cylinder& t);
  void write_simplex(const geom_simplex& t);
  void write_quadrilateral(const geom_quadrilateral& t);
  void write_color_quad(const geom_color_quad& c);
  void write_segment(const geom_segment& seg);

  void write_circle(const geom_circle& cir);
};

#endif
