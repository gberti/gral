#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_H

// $LICENSE

#include "Geometry/point.h"
#include "Utility/copy-traits.h"

namespace GrAL {


class rendering_language;

/*! \defgroup geometricshape Renderable geometric shapes

 */

/*! \brief Abstract base class for renderable geometric item

   \ingroup geometricshape
 */

class geom {
public:
  virtual void write_geom_to(rendering_language&) const = 0;
  virtual ~geom() {}
  virtual geom* clone() const = 0;

  typedef point coord_type;
};

template<>
struct copy_traits<geom> : public copy_traits_base<geom> {
    static geom* clone(geom const& g) { return g.clone();}
};

} // namespace GrAL 

#endif

