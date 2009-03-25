#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_RANGES_H
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_RANGES_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

/*! \brief Copy an overlap range
    \ingroup overlapds
*/
template<class DestRange, class SrcRange,  class Filter>
void CopyOverlapRanges(DestRange     & dest,
                       SrcRange const& src, 
                       Filter   const& f);


/*! \brief Create a layered range from separate ranges.
    \ingroup overlapds

    \templateparams
    - \c SrcRange: STL ForwardContainer (value type element handle)
    - \c DestRange: STL BackInsertionContainer
    - \c Filter: <tt> SrcRange::value_type -> DestRange::value_type </tt>
 */

template<class DestRange,class SrcRange,  class Filter>
void CopyOverlapRanges(DestRange& dest,
                       SrcRange const& priv, SrcRange const& exp,
                       SrcRange const& shd,  SrcRange const& cop,
                       Filter   const& f);

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/copy-overlap-ranges.C"
#endif

#endif
