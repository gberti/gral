
#include "Gral/Grids/ComplexND/complexnd.h"


namespace complexnd {
  template class ComplexND<complexnd::ANY>;
  template class ComplexND<2>;

  typedef ComplexND<complexnd::ANY> ComplexAnyD;
  typedef ComplexND<1>              Complex1D;

  template class element_t<ComplexAnyD, complexnd::ANY,complexnd::ANY>;

  // Any Cell
  template class element_t<ComplexAnyD, complexnd::ANY, 0>;

  // 1D Cell
  template class element_t<Complex1D,   1,  0>;
}

int main() {

}
