#ifndef IS_SPECIALIZED_H
#define IS_SPECIALIZED_H

template<class V>
struct is_specialized {
  static bool specialized() { return false;}
  static const char* name() { return "";}
};

#endif
