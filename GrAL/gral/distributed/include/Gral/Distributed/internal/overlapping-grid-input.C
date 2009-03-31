#ifndef NMWR_GB_OVERLAPPING_GRID_INPUT_C
#define NMWR_GB_OVERLAPPING_GRID_INPUT_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

template<class COARSEG, class FINEG, 
         class OVLP>
void overlapping_grid_input<COARSEG,FINEG,OVLP>::init(COARSEG     const& cg,
                                                      std::string const& basenm_)
{
  the_coarse_grid = &cg;
  basenm = basenm_;
  the_fine_grid.init(basenm + ".grid");
  init_overlap();
}

} // namespace GrAL 

#endif
