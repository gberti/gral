#ifndef NMWR_GB_CONTAINER_PROXIES_H
#define NMWR_GB_CONTAINER_PROXIES_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
