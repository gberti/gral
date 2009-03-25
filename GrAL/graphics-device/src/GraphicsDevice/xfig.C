

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


// INCOMPLETE FILE

#include <math.h>
#include "Geometry/algebra.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/xfig.h"
#include "GraphicsDevice/circle.h"
#include "GraphicsDevice/segment.h"
#include "GraphicsDevice/quadrilateral.h"
#include "GraphicsDevice/triangle.h"

namespace GrAL { 

// this matrix is nessessary to transform points
mat4 M(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(0,0,0,1));
//mat4 M(Identity3D);
point Cam=point(0,0,1);


xfig::xfig(ostream* out) : rendering_language(out) 
{
  cerr << "output: XFIG\n"; 
  the_stream() << "#FIG 3.1 via multi-graph \nLandscape "
	       << "\nCenter \nMetric \n1200 2\n";  
//the_stream() << "(normalization world keep)\n"
  //       << "(bbox-draw world no)\n";
}

void xfig::pause(double seconds) 
{cerr << "not implemented.\n";}
//{ the_stream() << "\n(sleep-for " << seconds << ")\n";} 

void xfig::read_from_file(const string& file)
{cerr << "not implemented.\n";}
//{ the_stream() << "< " << '"' << file << '"'  << " ";}

void xfig::Camera(const point Camera)
{
  static point Cam=Camera;
}


//const point Cam, 
point xfig::projektion(const point Pkt)
{
  point export;
  export[0]=(((Pkt[0]-Cam[0])*Cam[2]*1.0)/(Cam[2]-Pkt[2]))-Pkt[0];
  export[1]=(((Pkt[1]-Cam[1])*Cam[2]*1.0)/(Cam[2]-Pkt[2]))-Pkt[1];
  return 450*export;
}

double xfig::projektion(const point Pkt1, const point Pkt2)
{
  return 0.0;
}

void xfig::begin_transformation(const Transformation& T)
{cerr << "Can't Handle this routine?.\n";
  if(T.IsAffine() && !IsToDoTrafo()){ // could handle anything until now?
    set_transformation(T);
  }
  else 
    PushToDoTrafo(T); // has to be processed on lower level, e.g. in specialized
                        // routines for geometries.

}

void xfig::end_transformation()
{cerr << "not implemented.\n";}

void xfig::begin_attribute(const Attribute&)
{cerr << "not implemented.\n";}

void xfig::end_attribute()
{cerr << "not implemented.\n";}

void xfig::begin_group(const string& name)
{cerr << "not implemented.\n";}

void xfig::end_group()
{cerr << "not implemented.\n";}
 
void xfig::begin_block()
{cerr << "not implemented.\n";}

void xfig::end_block()
{cerr << "not implemented.\n";}

void xfig::set_transformation(const Transformation& T)
{
  M=(GetMat4(T)*M);
}
 
void xfig::write_attribute(const Attribute& A)
{cerr << "not implemented.\n";}
 
void xfig::begin_object(const RenderableGeom& Obj) 
{
 Obj.write_geom_to(*this);
}
void xfig::end_object()  { /*the_stream() << "}\n";*/}

void xfig::write_circle(const geom_circle& cr)
{
  if (cr.radius1()==cr.radius2())
    the_stream() << "1 3 ";
  else
    the_stream() << "1 1 ";
  the_stream() << "0 1 -1 7 0 0 -1 0.000 1 0.000 " 
	       << projektion(cr.midpoint()) << " "
	       << 450*cr.radius1() << " " << 450*cr.radius2() << " " 
	       << projektion(cr.midpoint()+point(cr.radius1(),0)) << " "
	       << projektion(cr.midpoint()+point(cr.radius2(),0)) << " "
	       << "\n";
}


void xfig::write_sphere(const geom_sphere& cr)
{cerr << "not implemented.\n";}

void xfig::write_ball(const geom_ball& cr)
{cerr << "not implemented.\n";}

void xfig::write_triangle(const geom_triangle& tr)
{
  the_stream() << "2 1 0 1 -1 7 0 0 -1 0.000 0 0 -1 0 0 4 \n         ";
  for(int i=1;i<=3;i++)
    the_stream() << M*450*tr.corner(i) << " ";
  the_stream() << M*450*tr.corner(1) << "\n";

}

void xfig::write_torus(const geom_torus& cr)
{cerr << "not implemented.\n";}

void xfig::write_torus_bezier(const geom_torus& cr)
{cerr << "not implemented.\n";}

void xfig::write_cone(const geom_cone& cr)
{cerr << "not implemented.\n";}

void xfig::write_cube(const geom_cube& cr)
{cerr << "not implemented.\n";}

void xfig::write_cylinder(const geom_cylinder& cr)
{cerr << "not implemented.\n";}

void xfig::write_simplex(const geom_simplex& cr)
{cerr << "not implemented.\n";}

void xfig::write_quadrilateral(const geom_quadrilateral& q)
{    
  the_stream() << "2 1 0 1 -1 7 0 0 -1 0.000 0 0 -1 0 0 5 \n         ";
  for(int i=1;i<=4;i++)
    the_stream() << 450*q.corner(i) << " ";
  the_stream() << 450*q.corner(1) << "\n";

}

void xfig::write_color_quad(const geom_color_quad& cr)
{cerr << "not implemented.\n";}

void xfig::write_segment(const geom_segment& sg)
{
  the_stream() << "2 1 0 1 -1 7 0 0 -1 0.000 0 0 -1 0 0 2 \n         "
	       << 450*sg.corner(1) << " " << 450*sg.corner(2) << "\n";
}

/*void xfig::write_word(const geom_word& w)
{
  the_stream() << "4 0 -1 0 0 0 ";
}
*/

} // namespace GrAL 
