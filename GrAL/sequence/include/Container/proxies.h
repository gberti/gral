#ifndef NMWR_GB_CONTAINER_PROXIES_H
#define NMWR_GB_CONTAINER_PROXIES_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


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

#endif
