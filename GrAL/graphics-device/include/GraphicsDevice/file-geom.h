#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_FILE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_FILE_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"
//#include "GraphicsDevice/colormap.h"
#include "GraphicsDevice/rgb-color.h"

#include "Utility/pre-post-conditions.h"

#include <string>

namespace GrAL {


/*! Geom read from external file

 */
class geom_file : public geom {
private:
  typedef geom_file self;
  std::string       nm_;
protected:
  void init(std::string const& nm) { nm_ = nm;}
public:
  geom_file(std::string const& nm) { init(nm);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.read_from_file(file());}

  std::string file() const { return nm_;}
};

extern RenderableGeom RFileGeom(std::string const& nm);

} // namespace GrAL 

#endif
