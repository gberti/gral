#include "coordinate-mappings/identity2d.h"
#include "Grids/Reg2D/cartesian-grid2d.h"
#include "Grids/Reg2D/mapped-geometry.h"
#include "Grids/Complex2D/stored-geometry.h"

typedef stored_geometry_complex2D geometry_type;
typedef geometry_type::coord_type coord_type;

typedef identity2d<coord_type> source_mapping_type;
typedef general_mapped_geometry_for_reg2d<source_mapping_type> source_geom_type;

