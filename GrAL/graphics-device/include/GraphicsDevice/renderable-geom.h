#ifndef NMWR_GB_GRAPHICSDEVICE_RENDERABLE_OBJECT_H
#define NMWR_GB_GRAPHICSDEVICE_RENDERABLE_OBJECT_H

// $LICENSE


class rendering_language;
class geom;
#include "GraphicsDevice/transformation.h"
#include "GraphicsDevice/attribute.h"


class RenderableGeom {
private:
  geom *g; 
  Transformation T; 
  Attribute A;

  geom* geometry() const {return g;}
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

  virtual void write_geom_to(rendering_language&) const;
};

#include "Work-around/default-operators.h"

MAKE_DEFAULT_OPS(RenderableGeom,"RenderableGeom");

#endif
