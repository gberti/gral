
// $LICENSE

#include <fstream.h>

#include "Container/bijective-mapping.h"

#include "Gral/Distributed/construct-composite.h"
#include "Gral/Distributed/overlap-pattern.h"
#include "Gral/Distributed/composite-grid.h"
#include "Gral/Distributed/composite-grid-function.h"

#include "Gral/Distributed/copy-composite.h"
#include "Gral/Distributed/composite-grid-io.h"
#include "Gral/Distributed/overlapping-grid-io.h"
#include "Gral/Distributed/overlap-io.h"
#include "Gral/Distributed/overlap-range-io.h"

#include "Gral/IO/complex2d-format.h"

#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/boundary.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/enlarge.h"
#include "Gral/Grids/Complex2D/grid-functions.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"


/*! \file 

  Tests:
    - 2x2, 1 part  : OK
    - 2x2, 2 parts : OK
    - 4x1, 1 part  : OK
    - 4x1, 2 parts : OK
    - 4x2, 2 parts : OK

    \Todo Test different stencils
    \Todo Test 'bad' partitionings
 */


int main(int argc, char* argv[]) {

  typedef Complex2D                 fine_grid_type;
  typedef Complex2D                 coarse_grid_type;
  typedef stored_geometry_complex2D fine_geom_type;
  typedef grid_types<fine_grid_type> fgt;
  
  string gridfile       = "grid.complex2d";
  string partitionfile  = "grid.part";
  string pattern_string = "VC";

  //---------- construct master grid --------------

  fine_grid_type MasterG;
  fine_geom_type MasterGeom(MasterG);
  ifstream grd(gridfile.c_str());
  IstreamComplex2DFmt InG(grd);
  ConstructGrid(MasterG, MasterGeom, InG, InG);

  //----- create partitioning ---------

  partitioning<fine_grid_type> Prtng(MasterG);
  ifstream prt(partitionfile.c_str());
  Prtng.read_partition(prt);
  prt.close();


  //--- create composite grid -----

  typedef CompositeGrid<coarse_grid_type,fine_grid_type> comp_grid_type;
  typedef grid_types<coarse_grid_type>                   cgt;
  typedef cgt::Cell                                      CoarseCell;


  comp_grid_type  compG;
  typedef bijective_mapping<fgt::vertex_handle, fgt::vertex_handle> vertex_corr_map;
  typedef bijective_mapping<fgt::cell_handle,   fgt::cell_handle>   cell_corr_map;
  grid_function<CoarseCell, vertex_corr_map>    master2part_v;
  grid_function<CoarseCell, cell_corr_map>      master2part_c;
  overlap_pattern                               ovlp_pattern(pattern_string);
 
  ConstructComposite(compG,
                     Prtng,MasterGeom,
                     ovlp_pattern,
                     master2part_v, master2part_c);


  grid_function<cgt::Cell,fine_geom_type> compGeom(compG.TheCoarseGrid());

  {
    OstreamComplex2DFmt OG(cout);
    ConstructGrid0(OG,compG.TheCoarseGrid());
    cout << "master -> part (vertices):\n";
    for(cgt::CellIterator C(compG.TheCoarseGrid()); !C.IsDone(); ++C) {
      cout << "part " << C.handle() << '\n'; 
      for(fgt::VertexIterator v(MasterG); !v.IsDone(); ++v) {
        if(master2part_v(*C).defined(v.handle()))
          cout << v.handle() << " -> " << master2part_v(*C)(v.handle()) << '\n';
      }
    }
    cout << '\n';
    for(cgt::CellIterator C(compG.TheCoarseGrid()); !C.IsDone(); ++C) {
      fine_grid_type const& G_C = compG.Grid(*C);
      compGeom[*C].set_grid(G_C);
      for(fgt::VertexIterator v(MasterG); !v.IsDone(); ++v) {
        if(master2part_v(*C).defined(v.handle()))
          compGeom[*C].coord(G_C.vertex(master2part_v(*C)(v.handle())))
            = MasterGeom.coord(*v);
      }
      OstreamComplex2DFmt OG_C(cout);
      ConstructGrid(OG_C, compG.Grid(*C), compGeom(*C));
      cout << '\n';
  }
  }


  comp_grid_type CG2;
  // bijective_mapping<int,int> coarse_corr_c;
  cell_morphism<coarse_grid_type, coarse_grid_type> coarse_corr_c(compG.TheCoarseGrid(),
                                                                  CG2.TheCoarseGrid());
  // it is not so simple here to use vertex/cell_morphism 
  // instead of  bijective_mapping<> ! (Initialization!)
  grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_v(compG.TheCoarseGrid());
  grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_c(compG.TheCoarseGrid());

  CopyComposite(CG2,compG, coarse_corr_c,fine_corr_v, fine_corr_c);

  check_composite_grid(CG2);

  
   {
    OstreamComplex2DFmt OG(cout);
    ConstructGrid0(OG,CG2.TheCoarseGrid());
    cout << "master -> part (vertices):\n";
    for(cgt::CellIterator C(compG.TheCoarseGrid()); !C.IsDone(); ++C) {
      cgt::Cell C2 = coarse_corr_c(*C);
      cout << "part " << C2.handle() << '\n'; 
      for(fgt::VertexIterator mv(MasterG); !mv.IsDone(); ++mv) {
        if(master2part_v(*C).defined(mv.handle()))
          cout << mv.handle() << " -> " 
               << fine_corr_v(C2)(master2part_v(*C)(mv.handle())) << '\n';
      }
    }
    cout << '\n';
    grid_function<cgt::Cell,fine_geom_type> CGeom2(CG2.TheCoarseGrid());
    for(cgt::CellIterator C(compG.TheCoarseGrid()); !C.IsDone(); ++C) {
      cgt::Cell C2 = coarse_corr_c(*C);
      fine_grid_type const& G_C2 = CG2.Grid(C2);
      CGeom2[C2].set_grid(G_C2);
      for(fgt::VertexIterator mv(MasterG); !mv.IsDone(); ++mv) {
        if(master2part_v(*C).defined(mv.handle()))
          CGeom2[C2].coord(G_C2.vertex( fine_corr_v(C2)(master2part_v(*C)(mv.handle()))))
            = MasterGeom.coord(*mv);
      }
      OstreamComplex2DFmt OG_C(cout);
      ConstructGrid(OG_C, CG2.Grid(C2), CGeom2(C2));
      cout << '\n';
    }
   }


   typedef OstreamComplex2DFmt coarse_grid_output_t;
   typedef OstreamComplex2DFmt fine_grid_output_t;
   typedef  overlap_output<
     coarse_grid_output_t, 
     fine_grid_output_t, 
     overlap_range_output> overlap_output_t;
   typedef overlapping_grid_output<
     coarse_grid_output_t, 
     fine_grid_output_t,
     overlap_output_t> overlapping_grid_output_t;

   {
     // use a block: compOut must get destructed such that files get closed!
     composite_grid_output<coarse_grid_output_t, overlapping_grid_output_t> compOut("compgrid");
     
     cell_morphism<coarse_grid_type, coarse_grid_output_t> coarse_corr_c_out(compG  .TheCoarseGrid(),
                                                                             compOut.TheCoarseGrid());
     // it is not so simple here to use vertex/cell_morphism 
     // instead of  bijective_mapping<> ! (Initialization!)
     grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_v_out(compG.TheCoarseGrid());
     grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_c_out(compG.TheCoarseGrid());
     
     CopyComposite(compOut,compG, coarse_corr_c_out, fine_corr_v_out, fine_corr_c_out);
   }

   {
     
     typedef IstreamComplex2DCombFmt coarse_grid_input_t;
     typedef IstreamComplex2DCombFmt fine_grid_input_t;
     typedef  overlap_input<
       coarse_grid_type, 
       fine_grid_input_t, 
       overlap_range_input>  overlap_input_t;
     typedef overlapping_grid_input<
       coarse_grid_type, 
       fine_grid_input_t,
       overlap_input_t> overlapping_grid_input_t;
     
     typedef composite_grid_input<
       coarse_grid_input_t,
       coarse_grid_type,
       overlapping_grid_input_t>  composite_input_t;

     composite_input_t compIn("compgrid");
     comp_grid_type compG2;
     cell_morphism<composite_input_t::coarse_grid_type, coarse_grid_type> 
       coarse_corr_c_in(compIn.TheCoarseGrid(),
                        compG2.TheCoarseGrid());
     // it is not so simple here to use vertex/cell_morphism 
     // instead of  bijective_mapping<> ! (Initialization!)
     grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_v_in(compIn.TheCoarseGrid());
     grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_c_in(compIn.TheCoarseGrid());
     
     CopyComposite(compG2,compIn, coarse_corr_c_in, fine_corr_v_in, fine_corr_c_in);

     check_composite_grid(compG2);

     // re-output the grid compG2 to see if there is any difference
     {
       composite_grid_output<coarse_grid_output_t, overlapping_grid_output_t> compOut("compgridB");
       cell_morphism<coarse_grid_type, coarse_grid_output_t> coarse_corr_c_out(compG2 .TheCoarseGrid(),
                                                                             compOut.TheCoarseGrid());
       grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_v_out(compG2.TheCoarseGrid());
       grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_c_out(compG2.TheCoarseGrid());
       
       CopyComposite(compOut,compG2, coarse_corr_c_out, fine_corr_v_out, fine_corr_c_out);
     }

   }
   
}
