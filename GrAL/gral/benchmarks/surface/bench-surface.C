#include <iostream>
#include <iomanip>
#include <sys/times.h>
#include <time.h>

#include "IO/control-device.h"

#define private public
#define protected public

#include "Gral/Grids/Complex2D/all.h"


#include "./stored-geometry.h"

#include "./neumann-triang2d.h"
#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/mapped-geometry.h"

#include "./identity2d.h"
#include "Geometry/coords.h"
#include "Geometry/algebraic-primitives.h"


#include "Gral/Grids/Triang2D/all.h"
#undef  private
#undef  protected



typedef coordN<2>    coord_type;
typedef point_traits<coord_type> pt;

typedef identity2d<coord_type>  source_mapping_type;

typedef cartesian2d::mapped_geometry<source_mapping_type> source_geom_type;

extern "C" {
  void  loopf_(int* nloop,
	       int* ntri,
	       int* til,
	       int* nv,
	       double* geom,
	       double* surf);
    void loopC(int nloop, int nc, int* til, int nv, double* geom, double* surf);

}


template<class P>
class geom_triang2d {
public:
    typedef grid_types<Triang2D> gt;
    typedef gt::Vertex Vertex;
    typedef gt::FacetOnCellIterator FacetOnCellIterator;
    typedef P coord_type;
    typedef point_traits<P>         pt;
    typedef algebraic_primitives<P> ap;
    //public:
private:
    grid_function<Vertex,P> coords_;
    // P* restrict coords_;
    const Triang2D * const g;
    //    const int * const cells;
    int *  cells;
public:
    geom_triang2d(Triang2D const& T) 
	:  coords_(T),
	//coords_(new P[T.NumOfVertices()]), 
	g(&T), cells(T.cells) {}
#define CT coords_.table    
    P const& coord(Vertex const& V) const { return CT[V.handle().h];}
    P      & coord(Vertex const& V)       { return CT[V.handle().h];}
    
    P   outer_area_normal2(FacetOnCellIterator const& fc) const
    {
	int v1 = cells[3*fc.c.c+fc.fc];
	int v2 = cells[3*fc.c.c+ (fc.fc)%3];
	//pt::x(normal) = pt::y(coords_[v2]) - pt::y(coords_[v1]);
	//pt::y(normal) = pt::x(coords_[v1]) - pt::x(coords_[v2]);

		return coord_type(pt::y(CT[v2]) - pt::y(CT[v1]),
				  pt::x(CT[v1]) - pt::x(CT[v2]));
	//	return normal;

    }

    P  outer_area_normal(FacetOnCellIterator const& fc) const
    {
	return coord_type(pt::y(coords_[fc.V2()]) - pt::y(coords_[fc.V1()]),
			  pt::x(coords_[fc.V1()]) - pt::x(coords_[fc.V2()]));
    }
  
    double volume(FacetOnCellIterator const& fc) const 
    { return ap::distance(coord(fc.V1()), coord(fc.V2())) ;}

    double volume2(FacetOnCellIterator const& fc) const {

     	int v1 = cells[3*fc.c.c+fc.fc];
	int v2 = cells[3*fc.c.c+ (fc.fc)%3];
	double dx = pt::x(CT[v1]) -pt::x(CT[v2]);
	double dy = pt::y(CT[v1]) -pt::y(CT[v2]);
#undef CT
	return sqrt(dx*dx+dy*dy);
    }
private:
    mutable coord_type normal;
};

struct grid_types< neumann_triang2d<cartesian2d::RegGrid2D> >
  : public grid_types_nt2d<cartesian2d::RegGrid2D > {};

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace cartesian2d;

  typedef grid_types<Complex2D> gt; 
  typedef stored_geometry_complex2D geometry_type;

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"bench.in","main");
 
  int nx = 10;
  int ny = 10;

  RegisterAt(Ctrl,"-nx",nx);
  RegisterAt(Ctrl,"-ny",ny);

  int nloop = 1;
  RegisterAt(Ctrl,"-nloop", nloop);


  Ctrl.update();

  
  CartesianGrid2D R(nx,ny);
  neumann_triang2d<CartesianGrid2D> NTriang(R);

  source_mapping_type    mapping;
  source_geom_type GeomR(R,mapping);

  Complex2D CC;
  geometry_type GeomCC(CC);
  ConstructGrid(CC, GeomCC, NTriang, GeomR);


  Triang2D T2D;
  ConstructGrid0(T2D,CC);
  typedef grid_types<Triang2D> tgt;
  geom_triang2d<coord_type>  GeomT2D(T2D);
  gt::VertexIterator v = CC.FirstVertex();
  tgt::VertexIterator w(T2D);
  for(; !v.IsDone(); ++v,++w)
      GeomT2D.coord(*w) = GeomCC.coord(*v);


  ostream& timeout(cout);
  timeout << setprecision(12);
  //  timeout.setf(ios::scientific);



  int* connect = new int[3*CC.NumOfCells()];
  for(gt::CellIterator c = CC.FirstCell(); ! c.IsDone(); ++c) {
    int lv = 0;
    for(gt::VertexOnCellIterator vc = (*c).FirstVertex(); ! vc.IsDone(); ++vc, ++lv) {
      connect[3*c.handle() + lv] = vc.handle();
    }
  }

  clock_t t0, t1;


  cerr << "Benchmark cell surface:\n";
  grid_function<tgt::Cell,double> surface(T2D);
  t0 = clock();
  for(int N = 0; N < nloop; ++N) {
      for(tgt::CellIterator c(T2D); ! c.IsDone(); ++c) {
	  surface[c] = 0.0;
	  for(tgt::FacetOnCellIterator fc(*c); ! fc.IsDone(); ++fc) {
	      surface[*c] += GeomT2D.volume(fc);
	  }   
      }
  }
  t1 = clock(); 
  timeout << "Triang2D geom:                    " 
	  << (t1-t0)*1.0/CLOCKS_PER_SEC << " sec." << endl;




  t0 = clock();
  for(int N = 0; N < nloop; ++N) {
      for(tgt::CellIterator c(T2D); ! c.IsDone(); ++c) {
	  surface[c] = 0.0;
	  for(tgt::FacetOnCellIterator fc(*c); ! fc.IsDone(); ++fc) {
	      surface[*c] += GeomT2D.volume2(fc);
	  }   
      }
  }
  t1 = clock();
  timeout << "Triang2D geom (low level):        " 
	  << (t1-t0)*1.0/CLOCKS_PER_SEC << " sec." << endl;



      
  int nv = T2D.NumOfVertices();
  int nc = T2D.NumOfCells();
  coord_type * ll_geom = new coord_type[nv];
  for(gt::VertexIterator v = CC.FirstVertex(); ! v.IsDone(); ++v)
    ll_geom[v.handle()] = GeomCC.coord(*v);

  double * geom = new double[2*nv];
  for(int v = 0; v< nv; ++v) {
      geom[2*v]   = pt::x(ll_geom[v]);
      geom[2*v+1] = pt::y(ll_geom[v]);
  }

  double * surf = new double[nc];


  t0 = clock();
  loopf_(&nloop, &nc, connect, &nv, geom, surf);
  t1 = clock();
  timeout << "F77:                                " 
	  << (t1-t0)*1.0/CLOCKS_PER_SEC << " sec."  << endl;
  

  t0 = clock();
  loopC(nloop, nc, connect, nv, geom, surf);
  t1 = clock();
  timeout << "C:                                  " 
	  << (t1-t0)*1.0/CLOCKS_PER_SEC << " sec."  << endl;


}
