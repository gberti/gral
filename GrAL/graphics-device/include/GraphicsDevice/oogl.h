#ifndef NMWR_GB_GRAPHICSDEVICE_OOGL_H
#define NMWR_GB_GRAPHICSDEVICE_OOGL_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/rendering-language.h"

namespace GrAL {

class oogl : public rendering_language {
private:
  oogl(const oogl& ) {}
public:
  oogl(std::ostream* out);
  oogl() : rendering_language() {}
  virtual oogl* clone() const;

  virtual void begin_transformation(const Transformation&) ;
  virtual void end_transformation() ;
  virtual void begin_attribute(const Attribute&) ;
  virtual void end_attribute();
  virtual void begin_object(const RenderableGeom& Obj) ;
  virtual void end_object() ;
  virtual void begin_group(const std::string& name);
  virtual void end_group();
  virtual void begin_block();
  virtual void end_block();
  
  virtual void pause(double seconds);
  virtual void read_from_file(const std::string& file);
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

} // namespace GrAL 

#endif
