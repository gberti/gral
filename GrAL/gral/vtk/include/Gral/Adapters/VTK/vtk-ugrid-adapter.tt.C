#ifndef GRAL_VTK_UGRID_ADAPTER_C
#define GRAL_VTK_UGRID_ADAPTER_C

#include "Utility/pre-post-conditions.h"

namespace GrAL { 

namespace vtk_ugrid {

template <int D>
UGridVTKArchetypes<D>::UGridVTKArchetypes() {
  GRAL_ERROR << "Generic version should never be called\n";
  GRAL_ABORT;
}

template <>
UGridVTKArchetypes<2>::UGridVTKArchetypes();

template <>
UGridVTKArchetypes<3>::UGridVTKArchetypes();


template <int D>
typename UGridVTKArchetypes<D>::archetype_type const& 
UGridVTKArchetypes<D>::ArchetypeOf(UGridVTKArchetypes<D>::Cell const& c) const {
  return archetypes[archetype_of(c)];
}
 

template <int D>
typename UGridVTKArchetypes<D>::archetype_handle 
UGridVTKArchetypes<D>::archetype_of(UGridVTKArchetypes<D>::Cell const& c) const {
  GRAL_ERROR << "Generic version should never be called\n";
  GRAL_ABORT;
  return -1;
} 

template <>
UGridVTKArchetypes<2>::archetype_handle 
UGridVTKArchetypes<2>::archetype_of(UGridVTKArchetypes<2>::Cell const& c) const;

template <>
UGridVTKArchetypes<3>::archetype_handle 
UGridVTKArchetypes<3>::archetype_of(UGridVTKArchetypes<3>::Cell const& c) const;

template <int D>
UGridVTKArchetypes<D>::~UGridVTKArchetypes()
{
  // empty body
}

} // namespace vtk_ugrid

} // namespace GrAL 


#endif
