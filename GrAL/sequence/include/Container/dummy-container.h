#ifndef NMWR_GB_DUMMY_CONTAINER_H
#define NMWR_GB_DUMMY_CONTAINER_H


// $LICENSE

namespace GrAL {

/*! \brief dummy container to substitute for an unneeded container parameter.
 
  sample use: see Grids/Algorithms/find-boundary-components.h
*/

template<class T>
class dummy_container {
public:
  typedef T      value_type;
  typedef T*     pointer;
  typedef T&     reference;
  typedef T*     iterator;
  typedef T*     const_iterator;
  typedef size_t size_type;

  dummy_container() {}
 
  iterator begin() { return (T*)0;}
  iterator end  () { return (T*)0;}
  const_iterator begin() const { return (T*)0;}
  const_iterator end  () const { return (T*)0;}

  size_type size() const { return 0;}

  void push_back(T const&) const {}
};

} // namespace GrAL 
#endif
