
// $LICENSE

#include "GraphicsDevice/file-geom.h"

RenderableGeom RFileGeom(std::string const& nm)
{ return RenderableGeom(new geom_file(nm)); }

