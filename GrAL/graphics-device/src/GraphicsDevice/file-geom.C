
// $LICENSE

#include "GraphicsDevice/file-geom.h"

namespace GrAL {

RenderableGeom RFileGeom(::std::string const& nm)
{ return RenderableGeom(new geom_file(nm)); }

} // namespace GrAL 
