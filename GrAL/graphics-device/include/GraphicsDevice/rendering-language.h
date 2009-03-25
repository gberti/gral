#ifndef NMWR_GB_GRAPHICSDEVICE_RENDERING_LANGUAGE_H
#define NMWR_GB_GRAPHICSDEVICE_RENDERING_LANGUAGE_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <string>
// #include <strstream.h>

#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/attribute.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

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
class geom_circle;
class geom_polygon;


#include "Utility/copy-traits.h"
/*! \defgroup renderers Interfaces to rendering engines

 */

class rlang_internals;

template<>
struct copy_traits<rlang_internals> 
{
    static rlang_internals* clone  (rlang_internals const& t);
    static void             destroy(rlang_internals      * t);
};

/*! \brief Abstract base class for renderers
  \ingroup renderers
   

  Used as 'letter' by GraphicsDevice.

  These class has to solve a classical 'double dispatch'
  problem: There is an indefinite number of rendering engines,
  and a (possibly unlimited) number of  geometric shapes.
  The solution chosen here is to restrict admissible shapes
  to just a few, including groups of geometric shapes.
  If a request to render an abstract geometric g object is made,
  g is in turn requested to issue the necessary calls to the 
  primitive rendering operations (corresponding to primitive shapes).

  A useful set of primitive shapes is given by the primitives 
  understood by the union of 'standard' rendering engines.
  If a primitive is not understood by a concrete renderer,
  the corresponding subclass of rendering_language has to
  decompose this primitive in yet more primitive parts.


  \todo Should be refined: textual renderers like oogl, Renderman,
    and APIs like OpenGL. 
    Currently contains some functions 
    which are only appropriate to textual case;
    these should be factored out.
   
 */

class rendering_language {
private:
  copy_on_write_ptr<rlang_internals>  state;
  //  virtual void empty_buffer();
protected:
  std::ostream& the_stream();

  //! return accumulated Trafos that couldn't be handled directly
  Transformation ToDoTrafo() const; 
  //! compose ToDoTrafo() with T
  void PushToDoTrafo(const Transformation& T); 
  //! remove last composed trafo
  void PopToDoTrafo(); 
  //! Are there some Trafos accumulated that can't be handled?
  bool IsToDoTrafo() const; 
public:
  rendering_language(): state(0) {}
  rendering_language(std::ostream* out);
  virtual ~rendering_language();
  virtual rendering_language* clone() const = 0;

  virtual void set_stream(std::ostream* out); 
  virtual void begin_transformation(const Transformation&) = 0;
  virtual void end_transformation()  = 0;
  virtual void begin_attribute(const Attribute&)  = 0;
  virtual void end_attribute()  = 0;
  virtual void begin_object(const RenderableGeom& obj)  = 0;
  virtual void end_object()  = 0;
  //  virtual void begin_group() = 0;
  virtual void begin_group(const std::string& name) = 0;
  virtual void end_group()   = 0;
  virtual void begin_block()  = 0;
  virtual void end_block()    = 0;
  virtual void pause(double seconds) = 0;
  virtual void read_from_file(const std::string& file) = 0;
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

template<>
struct copy_traits<rendering_language> : public  copy_traits_base<rendering_language> {
  static rendering_language * clone(rendering_language const& r) 
  { return r.clone();}
};

} // namespace GrAL 

#endif
