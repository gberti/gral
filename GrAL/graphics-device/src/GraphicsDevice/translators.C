#include "Graphics/translators.h"
#include "Graphics/triangle.h"
#include "Graphics/segment.h"

void rendering_language::write_triangle(const geom_triangle& tr)
{
  write_segment(geom_segment(tr.corner(1),tr.corner(2)));
  write_segment(geom_segment(tr.corner(2),tr.corner(3)));
  write_segment(geom_segment(tr.corner(3),tr.corner(1)));
}
