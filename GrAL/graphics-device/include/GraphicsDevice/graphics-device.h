#ifndef NMWR_GB_GRAPHICS_DEVICE_1_H
#define NMWR_GB_GRAPHICS_DEVICE_1_H

class GraphicsDevice {
public:
  GraphicsDevice(rendering_language* r) : renderer(r) {}
  void draw(const RenderableGeom& Obj) { TheRenderer().filter(Obj);}
  rendering_language& TheRenderer() { return *renderer;}
private:
  rendering_language* renderer;
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

#endif
