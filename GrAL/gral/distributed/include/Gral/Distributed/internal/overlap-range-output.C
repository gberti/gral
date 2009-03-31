#ifndef NMWR_GB_OVERLAP_RANGE_OUTPUT_C
#define NMWR_GB_OVERLAP_RANGE_OUTPUT_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

template<class E>
template<class OVLPRANGE, class FILTER>
void overlap_range_output<E>::copy_from(OVLPRANGE const& src,
                                        FILTER    const& f)
{ 
  (*out) << src.privee() .size() << ' ' 
         << src.exposed().size() << ' '
         << src.shared() .size() << ' '
         << src.copied() .size() << '\n';
  
  typedef typename OVLPRANGE::const_iterator iter;
  for(iter i = src.privee() .begin(); i != src.privee().end(); ++i)
    (*out) << *i << ' ';
  (*out) << '\n';
  for(iter i = src.exposed().begin(); i != src.exposed().end(); ++i)
    (*out) << *i << ' ';
  (*out) << '\n';
  for(iter i = src.shared() .begin(); i != src.shared().end(); ++i)
    (*out) << *i << ' ';
  (*out) << '\n';
  for(iter i = src.copied() .begin(); i != src.copied().end(); ++i)
    (*out) << *i << ' ';
  (*out) << '\n';
  
}

} // namespace GrAL 

#endif
