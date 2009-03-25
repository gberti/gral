#ifndef NMWR_GB_RFUNCTION_MISC_H
#define NMWR_GB_RFUNCTION_MISC_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {


inline double hermite(double a, double b, double x)
{
  double t = (x-a)/(b-a);
  if (t <= 0.0) return 0.0;
  else if (t<= 1.0) return (t*t*(3.0-2.0*t));
  else return 1.0;
}

inline double d_hermite(double a, double b, double x)
{
  double t = (x-a)/(b-a);
  if (t <= 0.0) return 0.0;
  else if (t<= 1.0) return (6*t*(1.0-t)/(b-a) );
  else return 0.0;
}


} // namespace GrAL 

#endif
