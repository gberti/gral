#include "Gral/Base/multi-grid-functions.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/CartesianND/all.h"

template multi_grid_function<cartesian2d::CartesianGrid2D,int>;
template multi_grid_function<Complex2D,int>;




template<class MGF, int D>
struct mgf_tester {

  static void act(MGF & mgf) {
    typedef typename MGF::grid_type  grid_type;
    typedef grid_types<grid_type>    gt;
    typedef typename gt::sequence_iterator_d<D>::type ElementIterator;

    mgf_tester<MGF,D-1>::act(mgf);
    for(ElementIterator e(mgf.TheGrid()); ! e.IsDone(); ++e)
      mgf[*e] = typename MGF::value_type(D);
  }
};

template<class MGF>
struct mgf_tester<MGF,0> 
{
  static void act(MGF &) {}
};


template<class MGF>
void test_mgf(MGF& mgf)
{
  typedef typename MGF::grid_type grid_type;
  mgf_tester<MGF, grid_types<grid_type>::dim>::act(mgf);
}

int main() {

  {
    typedef   cartesian2d::CartesianGrid2D grid_type;
    typedef grid_types<grid_type> gt;
    grid_type R(2,2);
    
    multi_grid_function<grid_type, int> mgf(R,1);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      mgf[*v] = v.handle();
    
    test_mgf(mgf);
  }

  {
    typedef   cartesiannd::grid<2> grid_type;
    typedef grid_types<grid_type> gt;
    grid_type R(gt::index_type(2,2));
    
    multi_grid_function<grid_type, int> mgf(R,1);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      mgf[*v] = v.handle();
    
    test_mgf(mgf);
  }
}
