#ifndef NMWR_GB_OVERLAP_RANGE_INPUT_C
#define NMWR_GB_OVERLAP_RANGE_INPUT_C

// $LICENSE

template<class E>
template<class OVLPRANGE, class FILTER>
void overlap_range_input<E>::copy_to(OVLPRANGE      & dest,
                                     FILTER    const& f) const
{ 
  // init();
  REQUIRE_ALWAYS( ( (in != 0) && (in)), "Invalid stream!\n",1);
  dest.TheContainer().reserve(size());
  unsigned cnt = 0; 
  for(; cnt < npriv; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
  dest.set_exposed_begin();
  for(; cnt < npriv+nexposed; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
  dest.set_shared_begin();
  for(; cnt < npriv+nexposed+nshared; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
  dest.set_copied_begin();
  for(; cnt < npriv+nexposed+nshared+ncopied; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
}

#endif
