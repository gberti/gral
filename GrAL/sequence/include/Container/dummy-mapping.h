#ifndef NMWR_GB_DUMMY_MAPPING_H
#define NMWR_GB_DUMMY_MAPPING_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

/*! \brief dummy mapping to substitute for an unneeded mapping parameter.
    \ingroup mappings

    sample use: see Grids/Complex2D/generic/complex2d-construct.C
*/

template<class T1, class T2>
class dummy_mapping {
private:
  T2 dummy;
public:
  dummy_mapping() {}
 
  const T2& operator()(const T1&) const { return dummy;}
  T2&       operator[](const T1&)       { return dummy;}

  typedef T1 argument_type;
  typedef T2 result_type;
};

} // namespace GrAL 

#endif
