#ifndef NMWR_GB_GRAPHICSDEVICE_GRAPHICS_DEVICE_H
#define NMWR_GB_GRAPHICSDEVICE_GRAPHICS_DEVICE_H

// $LICENSE

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/manips.h"

namespace GrAL {

/*! \brief Envelope class for rendering_language

  \ingroup renderers
 */

class GraphicsDevice {
private:
  copy_on_write_ptr<rendering_language> renderer;
public:
  GraphicsDevice(rendering_language * r) : renderer(r) {}
  void draw(const RenderableGeom& Obj) { TheRenderer().filter(Obj);}

  //! \todo Should be private, ops << should be friends
  rendering_language& TheRenderer() { return *renderer;}
};

inline GraphicsDevice& operator<<(GraphicsDevice& G, const RenderableGeom& Obj)
{ G.draw(Obj); return(G);}

inline GraphicsDevice& operator<<(GraphicsDevice& G, DevManip0 M)
{ (*M)(G.TheRenderer()); return G;} 

template<class arg>
inline GraphicsDevice& operator<<(GraphicsDevice& G, const DevManip1<arg>& M)
{ M.apply(G.TheRenderer());  return G; }

template<class Container>
inline void draw_geom(GraphicsDevice& Dev, const Container& C)
{
  typedef typename Container::const_iterator cit;
  for(cit g = C.begin(); g != C.end(); ++g)
    Dev << (*g);
}

} // namespace GrAL 

#endif
