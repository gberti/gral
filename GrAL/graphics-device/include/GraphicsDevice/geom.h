#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_H

// $LICENSE


class rendering_language;

/*! \defgroup geometricshape Renderable geometric shapes

 */

/*! \brief Abstract base class for renderable geometic item

   \ingroup geometricshape
 */

class geom {
public:
  virtual void write_geom_to(rendering_language&) const = 0;
  virtual ~geom() {}
};

#endif

