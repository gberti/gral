#ifndef NMWR_GB_GEOM_H
#define NMWR_GB_GEOM_H

class rendering_language;
class RenderableGeom;

class geom {
public:
 virtual void write_geom_to(rendering_language&) const = 0;
  virtual ~geom() {}
};

#endif

