#include "Gral/Base/multi-grid-functions.h"
#include "Gral/Base/partial-multi-grid-functions.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/CartesianND/all.h"

template class multi_grid_function<cartesian2d::CartesianGrid2D,int>;
template class multi_grid_function<Complex2D,int>;

template class partial_multi_grid_function<cartesian2d::CartesianGrid2D,int>;
template class partial_multi_grid_function<Complex2D,int>;




template<class MGF, int D>
struct mgf_tester {

  static void act(MGF & mgf) {
    typedef typename MGF::grid_type  grid_type;
    typedef grid_types<grid_type>    gt;
    typedef typename gt::template sequence_iterator_d<D>::type ElementIterator;

    mgf_tester<MGF,D-1>::act(mgf);
    for(ElementIterator e(mgf.TheGrid()); ! e.IsDone(); ++e) {
      mgf[*e] = typename MGF::value_type(D);
      REQUIRE_ALWAYS(mgf(*e) == typename MGF::value_type(D), "mgf(*e)=" << mgf(*e), 1);
    }
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
    
    multi_grid_function        <grid_type, int> mgf (R,1);
    partial_multi_grid_function<grid_type, int> pmgf(R,1);

    pmgf.set_default(-1);
    REQUIRE_ALWAYS(pmgf.get_default() == -1, "pmgf::get_default()=" << pmgf.get_default() ,1);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v ) 
      REQUIRE_ALWAYS(pmgf(*v) == -1, "pmgf(*v)=" << pmgf(*v), 1);      
    for(gt::EdgeIterator e(R); ! e.IsDone(); ++e)
      REQUIRE_ALWAYS(pmgf(*e) == -1, "pmgf(*e)=" << pmgf(*e), 1);
    for(gt::CellIterator c(R); ! c.IsDone(); ++c)
      REQUIRE_ALWAYS(pmgf(*c) == -1, "pmgf(*c)=" << pmgf(*c), 1);

    for(gt::VertexIterator v(R); !v.IsDone(); ++v ) {
      mgf [*v] = v.handle();
      pmgf[*v] = v.handle();
    }

    test_mgf(mgf);
    test_mgf(pmgf);

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
