#include <math.h>

#ifdef EXTERNC 
extern "C" {
#endif

  /*#define restrict __restrict__*/

void loopC(int nloop, int nc, int* restrict til, int nv, 
	   double* restrict geom, double* restrict surf)
{
    int N, c, vc;
    for(N = 0; N < nloop; ++N) {
	for(c = 0; c < nc; ++c) {
	    surf[c] = 0.0;
	    for(vc = 0; vc < 3; ++vc) {
		int vc1 = (vc+1)%3;
		double dx = (geom[2*til[3*c+vc ]   ] - geom[2*til[3*c+vc1]   ]);
		double dy = (geom[2*til[3*c+vc ]+1 ] - geom[2*til[3*c+vc1] +1]);
                surf[c] +=  sqrt(dx*dx+dy*dy);
	    }
	}
    }
}

#ifdef EXTERNC 
}
#endif

