
// $LICENSE


#include <math.h>

#include "GraphicsDevice/word.h"
#include "GraphicsDevice/letter.h"
#include "GraphicsDevice/calc-transformations.h"
#include "Geometry/algebraic-primitives.h"

namespace GrAL {

void geom_word::init(std::string           const& ss, 
                     geom_word::coord_type const& cc1) 
{
  str=ss;
  c[0]=cc1;
  c[1]=cc1+coord_type(str.length(),0,0);
  c[2]=cc1+coord_type(0,1,0);
}

void geom_word::init(std::string           const& ss,
                     geom_word::coord_type const& cc1,
                     geom_word::coord_type const& cc2,
                     geom_word::coord_type const& cc3) 
{
  str=ss;
  c[0]=cc1;
  c[1]=cc2;
  c[2]=cc3;
}

void geom_word::write_geom_to(rendering_language& L) const
{
  int xcount=0,ycount=1,max=0;

    for(int i=0;i<(int)str.length();i++){
      char ch=str[i];
      if (ch=='\n') {ycount=ycount+1;xcount=0;}
      else xcount=xcount+1;
      if (xcount>max) max=xcount;
    }

  double x=0,y=0;
  Transformation TF0;
  point b_l=c[0];  // point(0,0,0); // left-buttomn-corner of text-field
  point b_r=c[1];  // point(2,0,1); // right-buttomn-corner of text-field
  point t_l=c[2];  // point(0,2,1); // left-top-corner of text-field
  typedef algebraic_primitives<point> ap;
  double x_length = ap::distance(b_r,b_l);
  double y_length = ap::distance(t_l,b_l);
  double x_shrink=x_length*1.0/(1.05*max);      // skale in x-direction
  double y_shrink=y_length*1.0/(1.15*ycount);   // skale in y-direction
  if (x_shrink==0) {
    std::cerr << "No X-LENGTH <using standards> \n";
  }
  if (y_shrink==0) {
    std::cerr << "No Y-LENGTH <using standards> \n";
  }
  // correture of shrink-factors
  // if ((x_shrink*1.0/y_shrink < 1.2) || (x_shrink*1.0/y_shrink > 2.8)){
  //  cerr << "text_scale " << x_shrink*1.0/y_shrink << " try to correct \n";
  //}
  while (x_shrink*1.0/y_shrink < 0.25)
    {
      y_shrink=y_shrink*1.0/1.5;
    }
  while (x_shrink*1.0/y_shrink > 0.75)
    {
      x_shrink=x_shrink*1.0/1.5;
    }
  

 L.begin_block();
   RenderableGeom Letter;
   {
     TF0=Scale(point(x_shrink,y_shrink,1));
   } 
   for(int j=0;j<(int)str.length();j++){
     char ch=str[j];
     Letter= TransformRectangle(b_l,b_r,t_l)(TF0(RLetter(ch,point(x,y,0))));
     L.filter(Letter);
     x=x+1;
     if (ch=='\n') {y=y-1.2;x=0;}
   }
 L.end_block();
}

extern RenderableGeom RWord(std::string                const& str, 
                            RenderableGeom::coord_type const& c1,  //< lower left
                            RenderableGeom::coord_type const& c2,  //< lower right
                            RenderableGeom::coord_type const& c3) //< upper left
{return RenderableGeom(new geom_word(str,c1,c2,c3));}

extern RenderableGeom RWord_1(std::string                const& str, 
                              RenderableGeom::coord_type const& c1,  //< lower left
                              RenderableGeom::coord_type const& c2,  //< lower right
                              RenderableGeom::coord_type const& c3) //< upper left
{ return RenderableGeom(new geom_word(str,c1,c2,c3));}

} // namespace GrAL 
