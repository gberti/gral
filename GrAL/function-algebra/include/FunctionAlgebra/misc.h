#ifndef NMWR_GB_RFUNCTION_MISC_H
#define NMWR_GB_RFUNCTION_MISC_H

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


inline point makepoint(int dim,const string& coords)
{
 strstream in;
 in  << coords;
 point p(dim,no_init);
 in >> p;
 return p;
}

#endif
