#ifndef NMWR_GB_GRAPHICSDEVICE_RENDERABLE_GEOM_H
#define NMWR_GB_GRAPHICSDEVICE_RENDERABLE_GEOM_H

// $LICENSE


class rendering_language;

#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/attribute.h"

#include "Utility/reference-count.h"


/*! \brief Envelope class for geom
     \ingroup geometricshape
   
    This class is intended to be used in application programs.
    An object  of type RenderableGeom can be output to 
    a GraphicsDevice:

    \code    
    GraphicsDevice Dev; // ...
    RenderableGeom S = RSphere(1.0, point(0,0,0));
    Dev << S;
    \endcode
*/

class RenderableGeom {
private:
  copy_on_write_ptr<geom> g; 
  Transformation          T; 
  Attribute               A;

  geom const*  geometry() const {return g.operator->();}
protected:
  virtual void init();
public:
  RenderableGeom();
  RenderableGeom(geom* gg);
  virtual ~RenderableGeom();
  virtual bool HasTrafo() const;
  virtual bool HasAttr()  const;
  virtual void SetAttr(const Attribute& AA) { A = AA;}
  virtual void Map(const Transformation& T);
  virtual Transformation  trafo() const;
  virtual Attribute       attr()  const;

  typedef geom::coord_type  coord_type;
  virtual void write_geom_to(rendering_language&) const;
};


#endif
