#ifndef NMWR_DATA_EXCHANGE_PATTERN_H
#define NMWR_DATA_EXCHANGE_PATTERN_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
// This class stores a temporal sequence of data communication
// operations. This is important because the order of data exchange
// may matter, e.g. in the case of a structured grid, were clever
// scheduling makes exchange of "corner data" superflous.
//
// The pattern is formally an alternating sequence of sets of 
// (independent) actions and synchronizations.
//
//----------------------------------------------------------------

template<class PGrid>
class data_exchange_pattern {

};


#endif
