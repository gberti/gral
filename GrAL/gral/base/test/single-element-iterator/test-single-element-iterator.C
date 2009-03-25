
/*! \file
 */


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Iterators/single-element-iterator.h"

#include "Gral/Base/polygon.h"


namespace GrAL {
  template class single_element_iterator<grid_types<polygon1d::polygon>::Cell>;
}


int main() {
  using namespace GrAL;

  polygon1d::polygon P(3);
  typedef grid_types<polygon1d::polygon> gt;

  {
    gt::Cell c = * P.FirstCell();
    single_element_iterator<gt::Cell> ci(c);
    REQUIRE_ALWAYS(!ci.IsDone(), "", 1);
    single_element_iterator<gt::Cell> ci2(c);
    REQUIRE_ALWAYS(ci == ci2, "", 1);
    REQUIRE_ALWAYS(c  == *ci, "", 1);
    REQUIRE_ALWAYS(c.handle()  == ci.handle(), "", 1);
    ++ci;
    REQUIRE_ALWAYS(ci.IsDone(), "", 1);
    REQUIRE_ALWAYS(ci != ci2, "", 1);
    ++ci2;
    REQUIRE_ALWAYS(ci == ci2, "", 1);
  }

  {
    single_element_iterator<gt::Cell> ci;
    REQUIRE_ALWAYS( ci.IsDone(), "", 1);
    single_element_iterator<gt::Cell> ci2;
    REQUIRE_ALWAYS(ci == ci2, "", 1);
  }
}
