
// $LICENSE

/*
  Hierarchie der Elemente


  Basic Elements
	segments
        triangle 
	quadrilateral 

  Basic Figure
        sphere

  Complex Figures (and its Basics)
        simplex (triangles)
        ball (triangle)
	cube (quadrilaterals)
	cylinder
	cone
	torus (quadrilaterals or quads+bezier)
     --------------------------------	
        letter (segments)
	word (letters)

 */ 
#include <math.h>
#include <iomanip.h>
#include <fstream.h>
#include <string>
//#include "Visualization/colormap.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/oogl.h"
//#include "Geometry/algebra.h"
#include "GraphicsDevice/sphere.h"
#include "GraphicsDevice/ball.h"
#include "GraphicsDevice/triangle.h"
#include "GraphicsDevice/torus.h"
#include "GraphicsDevice/cube.h"
#include "GraphicsDevice/cylinder.h"
#include "GraphicsDevice/cone.h"
#include "GraphicsDevice/circle.h"
#include "GraphicsDevice/rgb-color.h"
#include "GraphicsDevice/simplex.h"
#include "GraphicsDevice/quadrilateral.h"
#include "GraphicsDevice/segment.h"
#include "GraphicsDevice/letter.h"
//#include "GraphicsDevice/word.h"

#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/graphics-device.h"

GraphicsDevice OOGLDevice(std::ostream& out) {
  out << setprecision(9);
  return GraphicsDevice(new oogl(&out));
}
 

GraphicsDevice OOGLDevice(const std::string& name) {
  std::string name1 = name + ".oogl";
  // should be replaced by FileDevice or so.
  std::ofstream* out = new std::ofstream(name1.c_str());
  (*out) << setprecision(9);
  return GraphicsDevice(new oogl(out));
}

oogl::oogl(ostream* out) : rendering_language(out) 
{
  cerr << "output: OOGL\n";
  //the_stream() << "(normalization world keep)\n"
  //       << "(bbox-draw world no)\n";
}

oogl * oogl::clone() const { return new oogl(*this);}


void oogl::pause(double seconds) 
{ the_stream() << "\n(sleep-for " << seconds << ")\n";} 

void oogl::read_from_file(const std::string& file)
{ the_stream() << "< " << '"' << file << '"'  << " ";}


void oogl::begin_object(const RenderableGeom& Obj) 
{
 the_stream() << "{ ";
 Obj.write_geom_to(*this);
}
void oogl::end_object()  { the_stream() << "}\n";}


void oogl::begin_group(const std::string& name)
{
  the_stream() << "(geometry " << name << " {  LIST {\n";
  //the_stream() << "LIST {";
}

void oogl::end_group()
{
  //the_stream() << "}\n";
   the_stream() << "} })\n"; // muesste auf dem stack nachsehen (begin_group(string) muss
                         // anders behandelt werden)
}

void oogl::begin_block()
{
 the_stream() << " LIST { \n";
}

void oogl::end_block()
{
  the_stream() << "}\n";
}

void oogl::begin_attribute(const Attribute& A) 
{
  the_stream() << "{ ";
  write_attribute(A);
}

void oogl::write_attribute(const Attribute& A) 
{
  the_stream()  << "appearance { ";
  wrap_rgb_color c;
  //  cerr << A.Transparent()  << '\n';
  //if (A.Transparent()>=0.5) the_stream() << "+transparent " ;
  if(GetValue(A,c))            // replaced ambient with diffuse: G. Berti,5.9.97
    the_stream() << "material { ka 1.0 diffuse " << c << " edgecolor " << c << "}";
  the_stream() << "}\nLIST{\n";
}

void oogl::end_attribute()  { the_stream() << "\n}\n}\n";}

void oogl::begin_transformation(const Transformation& T) 
{
  if(T.IsAffine() && !IsToDoTrafo()){ // could handle anything until now?
    the_stream() << "{ ";
    write_transformation(T);
  }
  else 
    PushToDoTrafo(T); // has to be processed on lower level, e.g. in specialized
                        // routines for geometries.
}


void oogl::write_transformation(const Transformation& T) 
{
 Transformation::matrix4_type M(T.GetMat4());
 int i,k;
 the_stream() << "INST \n"
	      << "  transform {\n";
  for(i=0;i<4;i++) {
    the_stream() << "    ";
    for(k=0;k<4;k++)
      the_stream() << M(k,i) << " "; // transpose of M !!
    the_stream() << '\n';
  }
  the_stream() << "  }\n"
	       << "  geom { LIST { ";
  
}

void oogl::end_transformation()  
{ 
 if(IsToDoTrafo())
   PopToDoTrafo();
 the_stream() << "\n} } }\n";
}

// function returns the vertices of a torus; 
point pos(double alpha, double beta, double maj_rad, double min_rad)
{
  // alpha, beta = [0,1]
    double a = maj_rad*cos(2*M_PI*alpha);
    double b = maj_rad*sin(2*M_PI*alpha);
    point m(a,b,0.0);
    point e1(m.normalize());
    point e2(0.0,0.0,1.0);
    point Q(m+min_rad*cos(2*M_PI*beta)*e1+min_rad*sin(2*M_PI*beta)*e2);
    
return Q;
}

RFunction TorusSurface(double maj_rad, double min_rad)
{
  // alpha, beta = [0,1]
  RFunction alpha(X(1,2));
  RFunction beta(X(2,2));
  RFunction C(maj_rad*Cos(2*M_PI*alpha));
  RFunction S(maj_rad*Sin(2*M_PI*alpha));
  
  RFunction M(combine(C,S,Constant(point(0.0),1)));
  RFunction e1(M/Sqrt(Norm2(3)(M)));
  point e2(0.0,0.0,1.0);
  RFunction Q(M+min_rad*Cos(2*M_PI*beta)*e1+min_rad*Sin(2*M_PI*beta)*e2);
    
  return Q;
}

point ball_coord(const double& my, const double& ny, 
		 point center, double radius)
{
 point Q(radius*cos(-M_PI/2+(M_PI*my))*cos(2*M_PI*ny)+center[1],
   	 radius*cos(-M_PI/2+(M_PI*my))*sin(2*M_PI*ny)+center[2],
	 radius*sin(-M_PI/2+(M_PI*my))+center[3]);
return Q;
}

//funktioniert nicht, da the_stream() zu oogl geh"ort, in_patch aber nicht
void in_patch(const int& num, const point& left_bottom, const point& right_top, const RFunction& parametrization)
{  
  int m,n;
  for(m=0;m<=num;m++){
    for(n=0;n<=num;n++){
      point q=parametrization(
		point(left_bottom[1]+m*1.0/num*(right_top[1]-left_bottom[1]),
		      left_bottom[2]+n*1.0/num*(right_top[2]-left_bottom[2])));
      //      the_stream() << q[1] << "," << q[2] << "," << q[3] << "  ";
    }
    // the_stream() << "\n";
  }
}


// write a sphere with radius b.radius around b.midpoint
void  oogl::write_sphere(const geom_sphere& rs) 
{ 
  the_stream() << "SPHERE " << rs.radius() << " " << rs.midpoint();
}


//write a triangle via quadrilateral
void oogl::write_triangle(const geom_triangle& tr)
{
  the_stream() << "QUAD\n"; 
  int k;
  // vertices
  for(k=1;k<=3;k++)
      the_stream() << tr.corner(k) << " ";
  the_stream() << tr.corner(1) << " ";
  }


// write a sphere with radius b.radius around b.midpoint via quadrilaterals
void  oogl::write_ball(const geom_ball& b) 
{ 
  int end =24;       // number of circles parallel to aquator
  int mod =6;        // half number of triangles in pole-position
  int steps= (int)floor(end/2);
  //int card=  (int)floor(steps/mod);
  //  int half=steps;
  if (steps >= mod) steps=mod; 
  int count=0;
  int i,j,help;
  the_stream() << "LIST \n";
  for(i=0;i<end;i++){
      count=count+1;
      help=steps;
      if ((cos(i*1.0/steps)*1.5/steps>1.0*b.radius()/end )) {
	steps=steps*2;
	for(j=0;j<help;j++){
	  the_stream() << "{";
	  write_triangle(geom_triangle(
	   ball_coord(i*1.0/end,j*1.0/help,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(2*j+1)*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,(j+1)*1.0/help,b.midpoint(),b.radius())));
	  the_stream() << "}\n{";
	  write_triangle(geom_triangle(
  	   ball_coord(i*1.0/end,j*1.0/help,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(2*j+1)*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,j*1.0/help,b.midpoint(),b.radius())));
	  the_stream() << "}\n{";
	  write_triangle(geom_triangle(
	   ball_coord(i*1.0/end,(j+1)*1.0/help,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(j+1)*1.0/help,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(2*j+1)*1.0/steps,b.midpoint(),b.radius())));
	  the_stream() << "}\n";}
      }
      //(end-count)==trunc(end/(2*3))
      //((end-count)%Mod==3)
      else if ((cos(i*1.0/steps)*1.0/steps<1.5*b.radius()/end )) {
	steps=steps/2;
	for(j=0;j<steps;j++){
	  the_stream() << "{";
	  write_triangle(geom_triangle(
	   ball_coord((i+1)*1.0/end,j*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,(2*j+1)*1.0/help,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(j+1)*1.0/steps,b.midpoint(),b.radius())));
	  the_stream() << "}\n{";
	  write_triangle(geom_triangle(
  	   ball_coord((i+1)*1.0/end,j*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,(2*j+1)*1.0/help,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,j*1.0/steps,b.midpoint(),b.radius())));
	  the_stream() << "}\n{";
	  write_triangle(geom_triangle(
	   ball_coord((i+1)*1.0/end,(j+1)*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,(2*j+2)*1.0/help,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,(2*j+1)*1.0/help,b.midpoint(),b.radius())));
	  the_stream() << "}\n";
	} 
      }
      else {
	for(j=0;j<steps;j++){
	  the_stream() << "{";
	  write_triangle(geom_triangle(
	   ball_coord(i*1.0/end,j*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(j+1)*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord(i*1.0/end,(j+1)*1.0/steps,b.midpoint(),b.radius())));
	  the_stream() << "}\n{";
	  write_triangle(geom_triangle(
  	   ball_coord(i*1.0/end,j*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,(j+1)*1.0/steps,b.midpoint(),b.radius()),
	   ball_coord((i+1)*1.0/end,j*1.0/steps,b.midpoint(),b.radius())));
	  the_stream() << "}\n";
	}
      }
  }
}



/* write a torus approximated with 2 n-gons around (0,0,0)
   major_sides = n-gon around (0,0,0), minor_sides = ngon around major-n-gon,
   major_radius=radius of 1-st n-gon, minor_radius = radius of 2-nd n-gon */
void oogl::write_torus(const geom_torus& t)
{ 
  int major_sides = 40;
  int minor_sides = 40;
   the_stream() << "OFF\n" 
		<< major_sides*minor_sides << " "     //number of vertices
		<< major_sides*minor_sides << " "     //number of facets
		<< 2*major_sides*minor_sides << "\n"; //number of edges
  int i,j;
  // vertices
  for(i=0;i<major_sides;i++){
    for(j=0;j<minor_sides;j++){
      point q=pos(i*1.0/major_sides,j*1.0/minor_sides,
		  t.major_radius(),t.minor_radius());
      the_stream() << q[1] << " " << q[2] << " " << q[3] << " \n";
    }
  }
  // facets
  for(i=0;i<major_sides-1;i++){
    for(j=1;j<minor_sides;j++){
      the_stream() << " 4 " << i*minor_sides+j-1 << " " << i*minor_sides+j 
		   << " " << (i+1)*minor_sides+j 
		   << " " << (i+1)*minor_sides+j-1 << "\n";
    }
      the_stream() << " 4 " << (i+1)*minor_sides-1 << " " << i*minor_sides
		   << " " << (i+1)*minor_sides 
		   << " " << (i+2)*minor_sides-1 << "\n";
  }
  //zykl. schliessen
  for(j=1;j<minor_sides;j++)
    the_stream() << " 4 " << (major_sides-1)*minor_sides+j-1 << " " 
		 << (major_sides-1)*minor_sides+j 
		 << " " << j << " " << j-1 << "\n";
    the_stream() << " 4 " << minor_sides*major_sides-1 
		 << " " << (major_sides-1)*minor_sides
		 << " 0 " << minor_sides-1 << "\n";
}

/* write a torus approximated with bezier around (0,0,0)
   major_sides = n-gon around (0,0,0), minor_sides = ngon around major-n-gon,
   major_radius=radius of 1-st n-gon, minor_radius = radius of 2-nd n-gon */
void oogl::write_torus_bezier(const geom_torus& tor)
{ 
  RFunction torus_param(TorusSurface(tor.major_radius(),tor.minor_radius()));
  int part = 8;           //number of patch-parts
  int patch = 5;
  int sides = 3 * patch;
  strstream s; 
  the_stream() << "BEZ" << part << part << "3\n";
  int i,j;
  // outer vertices of patch
  for(i=0;i<=sides;i=i+3){
    for(j=1;j<=sides;j=j+3){
      point q;
      point left_bottom=point(i*1.0/sides,j*1.0/sides);
      point right_top=point((i+3)*1.0/sides,(j+3)*1.0/sides);
      int m,n;
      for(m=0;m<=part;m++){
	for(n=0;n<=part;n++){
	  q=torus_param(
	     point(left_bottom[1]+m*1.0/part*(right_top[1]-left_bottom[1]),
		   left_bottom[2]+n*1.0/part*(right_top[2]-left_bottom[2])));
	  the_stream() << q[1] << "," << q[2] << "," << q[3] << "  ";
	}
	the_stream() << "\n";
      }



      //in_patch(part,
      //       point(i*1.0/sides,j*1.0/sides),
      //       point((i+3)*1.0/sides,(j+3)*1.0/sides),
      //       torus_param);
      the_stream() << "\n";
    }
  }

}


void oogl::write_cube(const geom_cube& cu)
{
  the_stream() << "QUAD\n"; 
  // facets
  int i,j;
  for (i=1;i<=2;i++)
    {
    for (j=1;j<=5;j=j+4)
      {the_stream() << cu.corner(j) << " " << cu.corner(j+1) << " "
		    << cu.corner(i*4) << " " << cu.corner(i*4-1) << "\n";
      }
    the_stream() << cu.corner(i) << " " << cu.corner(i+2)<< " " 
		 << cu.corner(i+4) << " " << cu.corner(i+6) << "\n";
    }  
}
 


// write a cone approximated with 2 n-gons; n=sides
void oogl::write_cone(const geom_cone& co)
{
  int sides = 25;
   the_stream() << "OFF\n" 
     << 2*sides << " " << sides + 2 << " " << 3*sides << "\n";
  //   #vertices         #facets             #edges
  int i,k;
  double x,y;
  for(i=0;i<sides;i++){
    x = co.lower_radius()*cos(2*M_PI*i/sides);
    y = co.lower_radius()*sin(2*M_PI*i/sides);
    the_stream() << x << " " << y << " 0.0 " << "\n";
    x = co.upper_radius()*cos(2*M_PI*i/sides);
    y = co.upper_radius()*sin(2*M_PI*i/sides);
    the_stream() << x << " " << y << " " << co.height() << " \n";
  } 
  // the regular n-polygons at bottom and top
  for(k = 0; k <= 1; k++){
    the_stream() << sides << " ";
    for(i=0;i<=2*sides-2; i++,i++)
      the_stream() << i + k << " ";
    the_stream() << "\n";
  }
  // vertical facets
  for(i=0;i<=2*sides -4;i++,i++){
    the_stream() << " 4 "<< i << " " << i+1 << " " << i+3 << " " << i+2 << "\n";
  }
  the_stream() << " 4 " << 2*sides-2 << " " << 2*sides-1 << " 1" << " 0" << "\n"; 

}


void oogl::write_simplex(const geom_simplex& sim)
{
   the_stream() << "OFF\n" 
     << "4" << " " << "4" << " " << "6" << "\n";
  // #vertices      #facets        #edges
  int i,j;
  // vertices
  for(i=1;i<=4;i++) {the_stream() << sim.corner(i) << "\n";}
  // facets
  for(i=0;i<4;i++)
    { 
      the_stream() << "3 ";
      for(j=0;j<4;j++)
	if (i!=j) the_stream() << j << " ";
      the_stream() << "\n";
    } 
}

// write a quadrilateral
// 4 corners known
void oogl::write_quadrilateral(const geom_quadrilateral& tr)
{
    the_stream() << "QUAD\n"; 
    // vertices
    for(int i=1;i<=4;i++)
      the_stream() << tr.corner(i) << " ";
}

void oogl::write_color_quad(const geom_color_quad& cq)
{
    the_stream() << "CQUAD\n"; 
    // vertices
    for(int i=1;i<=4;i++)
      the_stream() << cq.corner(i) << " " << cq.color_corner(i) << " 1\n";
}




// write a cylinder as a prisma over a n-gon; 
// n=sides, radius=c.radius,height=c.height
void oogl::write_cylinder(const geom_cylinder& c)
{
  int sides = 50;
   the_stream() << "OFF\n" 
     << 2*sides << " " << sides + 2 << " " << 3*sides << "\n";
  //   #vertices         #facets             #edges
  int i,k;
  double x,y;
  for(i=0;i<sides;i++){
    x = c.radius()*cos(2*M_PI*i/sides);
    y = c.radius()*sin(2*M_PI*i/sides);
    the_stream() << x << " " << y << " 0.0 " << "\n";
    the_stream() << x << " " << y << " " << c.height() << " \n";
  } 
  // the regular n-polygons at bottom and top
  for(k = 0; k <= 1; k++){
    the_stream() << sides << " ";
    for(i=0;i<=2*sides-2; i++,i++)
      the_stream() << i + k << " ";
    the_stream() << "\n";
  }
  // vertical facets
  for(i=0;i<=2*sides -4;i++,i++){
    the_stream() << " 4 "<< i << " " << i+1 << " " << i+3 << " " << i+2 << "\n";
  }
  the_stream() << " 4 " << 2*sides-2 << " " << 2*sides-1 << " 1" << " 0" << "\n"; 
 
}


//write a segment
//2 corners known
void oogl::write_segment(const geom_segment& seg)
{
  the_stream() << "{VECT\n 1 2 0\n\n -2 \n\n 0\n\n";
  the_stream() << seg.corner(1) << "  " << seg.corner(2);
  the_stream() << "}\n";
}


void oogl::write_circle(const geom_circle& cir)
{  
  int sides = 50;
  int i;
  double x,y,z;
   the_stream() << "OFF\n" 
     << sides << " 1 " << sides << "\n\n";
  for(i=0;i<sides;i++){
    x = cir.radius1()*cos(2*M_PI*i/sides)+cir.midpoint()[1];
    y = cir.radius1()*sin(2*M_PI*i/sides)+cir.midpoint()[2];
    z = cir.midpoint()[3];
    the_stream() << x << " " << y << " " << z << "\n";
  }
  the_stream() << sides << " ";
  for(i=0;i<sides; i++)
    the_stream() << i << " ";
  the_stream() << "\n";
 
}
