#ifndef NMWR_GB_CONTAINER_PROXIES_H
#define NMWR_GB_CONTAINER_PROXIES_H


// $LICENSE

namespace GrAL {

/*! \brief 
Wrap reference to T such that only writing is possible
 */
template<class T>
class write_only_proxy {
private:
  T* t;
public:
  write_only_proxy(T& tt) : t(&tt) {}
  void operator=(const T& rs) { (*t) = rs;}
};

} // namespace GrAL 


#endif
