#ifndef GRAL_IS_SPECIALIZED_H
#define GRAL_IS_SPECIALIZED_H

// $LICENSE

namespace GrAL {

template<class V>
struct is_specialized {
  static bool specialized() { return false;}
  static const char* name() { return "";}
};

} // namespace GrAL {
#endif
