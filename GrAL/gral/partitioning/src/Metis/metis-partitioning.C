
// $LICENSE

#include "Gral/Partitioning/metis-partitioning.h"
#include "Gral/Partitioning/internal/metis-options.h"


MetisPartitioning::MetisPartitioning()
{ options = new metis_options; }

MetisPartitioning::~MetisPartitioning()
{ delete options;}