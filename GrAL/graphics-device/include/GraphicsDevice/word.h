#ifndef NMWR_GB_GEOM_word_H
#define NMWR_GB_GEOM_word_H

/*----------------------------------------------------------------------------
    word.h		class to create a word

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Utility/pre-post-conditions.h"
#include "Graphics/geom.h"

#include <math.h>

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"
#include "Graphics/transformation.h"


class geom_word : public geom {
private:
  string str;
  point c[3];
protected:
  void init(const string& ss,point cc1) {str=ss;c[0]=cc1;c[1]=cc1+point(str.length(),0,0);c[2]=cc1+point(0,1,0);}
  void init(const string& ss,point cc1,point cc2,point cc3) {str=ss;c[0]=cc1;c[1]=cc2;c[2]=cc3;}
public:
  geom_word(const string& ss,point cc1) {str=ss;c[0]=cc1;}
  geom_word(const string& ss,point cc1,point cc2,point cc3) {init(ss,cc1,cc2,cc3);}
  string text() const { return str;}
  point  corner(int i) const 
    {
      REQUIRE( (i <= 3 && i >=1),
	       "geom_triangle::corner called with i= "<<i,1);
      return c[i-1];
    }

    void   write_geom_to(rendering_language& L) const; 
};


extern RenderableGeom RWord(const string& str, const point& c1, const point& c2, const point& c3);


#endif
