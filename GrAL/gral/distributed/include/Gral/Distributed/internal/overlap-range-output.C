#ifndef NMWR_GB_OVERLAP_RANGE_OUTPUT_C
#define NMWR_GB_OVERLAP_RANGE_OUTPUT_C

// $LICENSE

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

#endif
