#ifndef NMWR_GB_DISTRIBUTED_BASIC_TYPES_H
#define NMWR_GB_DISTRIBUTED_BASIC_TYPES_H


// the semantic of distributed sets:
// copy means: all corr. objects have the same (global) value
// sum  means: the global value is the sum of the local values
struct coherence_type {
  enum  { copy, sum, invalid }
};

struct overlap_layer_type {
  enum  { private, exposed, shared, copied, 
	  owned, imported, exported, invalid};
};

struct data_transfer_type { 
  enum { send, recv, invalid };
};

#endif
