#ifndef NMWR_GB_OVERLAPPING_GRID_INPUT_C
#define NMWR_GB_OVERLAPPING_GRID_INPUT_C

// $LICENSE

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

#endif
