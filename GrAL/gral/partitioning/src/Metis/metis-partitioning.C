
// $LICENSE

/*! \file
 */

#include "Gral/Partitioning/metis-partitioning.h"
#include "Gral/Partitioning/internal/metis-options.h"

namespace GrAL {

MetisPartitioning::MetisPartitioning()
{ options = new metis_options; }

MetisPartitioning::~MetisPartitioning()
{ delete options;}

} // namespace GrAL 
