
// $LICENSE

#include "Gral/Grids/Complex2D/test-complex2d.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"
#include "Gral/Grids/Complex2D/boundary.h"

namespace GrAL {

TestComplex2D::TestComplex2D()
  : test_vertices(true), 
    test_cells   (true),
    test_edges   (true), 
    test_boundary_facets(true),
    test_boundary_range(true),
    
    test_cell_vertices(true), 
    test_cell_edges   (true),
    test_cell_neighbours (true), 
    test_cell_nb_edges   (true),
    test_cell_nb_iter (false),
    test_cell_edge_flip(false)
{}

void TestComplex2D::register_at(ControlDevice& Ctrl, ::std::string const&) {
  RegisterAt(Ctrl, "test_vertices",test_vertices);
  RegisterAt(Ctrl, "test_cells",test_cells);
  RegisterAt(Ctrl, "test_edges",test_edges);
  RegisterAt(Ctrl, "test_boundary_facets",test_boundary_facets);
  RegisterAt(Ctrl, "test_boundary_range",test_boundary_range);

  RegisterAt(Ctrl, "test_cell_vertices",test_cell_vertices);
  RegisterAt(Ctrl, "test_cell_edges",test_cell_edges);
  RegisterAt(Ctrl, "test_cell_nb_edges",test_cell_nb_edges);
  RegisterAt(Ctrl, "test_cell_neighbours",test_cell_neighbours);
  RegisterAt(Ctrl, "test_cell_nb_iter",test_cell_nb_iter);
  RegisterAt(Ctrl, "test_cell_edge_flip",test_cell_edge_flip);
}

void TestComplex2D::test_complex2d_iterators(const Complex2D& CC,
					     const stored_geometry_complex2D& geom,
					     ::std::ostream& out) 
{

  typedef grid_types<Complex2D> gt;
 
  // enumerate all vertices (superflous, just for demonstration)
  grid_function< gt::Vertex,int> VNum(CC);
  {  
    int vnum = 0;
    gt::VertexIterator v(CC.FirstVertex());
    for(; ! v.IsDone(); ++v,++vnum){
      VNum[*v] = vnum;
    }
  }

  // ... and all cells.
  grid_function< gt::Cell,int> CNum(CC);
  {
    int cnum = 1;
    gt::CellIterator c(CC.FirstCell());
    for(; ! c.IsDone(); ++c,++cnum){
      CNum[*c] = cnum;
    }
  }

  // demonstrate grid-function iterators.
  // note that the order is not the same as the
  // order of the cells!
   grid_function< gt::Cell,int>::iterator i1;
  for(i1 = CNum.begin(); i1 != CNum.end(); ++i1)
    (*i1) -= 1;

  // the other way of iterating over a grid-function
  //  grid_types<grid_function<gt::Cell,int>::Grid>::CellIterator ci = CNum.TheGrid().FirstCell();
   grid_function< gt::Cell,int>::ElementIterator ci = CNum.FirstElement();
  for(; ! ci.IsDone(); ++ci)
    CNum[*ci] += 1;


  // confusion in mutator.h when compiling with g++
  // ostream_iterator<int> outp(cout," ");
  // copy(CNum.begin(),CNum.end(), outp);

  out << CC.NumOfVertices() << " vertices,  " << CC.NumOfCells()  << " cells.\n";


  // loop over all vertices of grid CC
  if (test_vertices) {
    for( gt::VertexIterator vv = CC.FirstVertex(); ! vv.IsDone(); ++vv) {
      out << "Vertex " << VNum[*vv] << " has "
	  << (*vv).NumOfCells() << " adj. Cells: ";
      // loop over all adjacent cells cv of vv
      for(gt::CellOnVertexIterator cv = (*vv).FirstCell(); ! cv.IsDone(); ++cv)
	out << "( No. " << CNum(*cv) 
	    << ", center " << geom.center(*cv) << ")  ";
      out << '\n';
    }
  }

  // loop over all cells of grid CC
  if (test_cells) {
    for( gt::CellIterator c=CC.FirstCell();!c.IsDone();++c){
      out << "Cell " << CNum(*c) <<  " has "
	  << "volume " << geom.volume(*c) << "; "
	  << (*c).NumOfVertices() << "  Vertices:  ";
      
      // loop over all vertices vc of cell c, print number of vc
      if( test_cell_vertices) {
	 gt::VertexOnCellIterator vc;
	for(vc = (*c).FirstVertex(); ! vc.IsDone(); ++vc)
	  out << VNum[*vc] << " ";
      }
      // loop over all neighbors of c, print number of nb or "B"  if on boundary.
      if (test_cell_neighbours) {
	 gt::CellNeighbourIterator cc;
	out << " and " << (*c).NumOfNeighbours() << " Nbs': \n";
	for(cc = (*c).FirstNeighbour(); ! cc.IsDone(); ++cc) {
	  out << "    " << CNum(*cc) << " :";
	  if( test_cell_nb_edges) {
	    gt::Edge E_c_cc(*c ,*cc); // alle 3 Versionen
	    gt::Edge E_cc_c(*cc ,*c); // sollten dasselbe ergeben
	    gt::Edge E_cc(cc);        // (bis evtl auf die Richtung)
	    out << "[ Edge(*c,*nb) : " 
		<< VNum(E_c_cc.V1()) << " -> " << VNum(E_c_cc.V2()) << ";  "
		<< "Edge(*nb,*c) : "
		<< VNum(E_cc_c.V1()) << " -> " << VNum(E_cc_c.V2()) << ";  "
		<< "Edge(nb) : "
		<< VNum(E_cc.V1()) << " -> " << VNum(E_cc.V2()) << "]\n";
	  }
	  
	  if(test_cell_nb_iter) {
	     gt::CellNeighbourIterator cc1 = cc.CyclicSucc();
	     gt::CellNeighbourIterator cc2 = cc.CyclicPred();

	    out << "CyclicPred(nb) : ";
	    out << CNum(*cc1);
	    out << " CyclicSucc(nb) : ";
	    out << CNum(*cc2);
	  }
	  if(test_cell_edge_flip) {
	     gt::CellNeighbourIterator ccnb(cc);
	     gt::CellNeighbourIterator ccsucc = ccnb.CyclicSucc();
	     gt::CellNeighbourIterator ccpred = ccnb.CyclicPred();
             gt::Edge ed(ccnb);
            
             gt::Vertex v1(ed.V1()), v2(ed.V2());
	     out << " v1: " << v1.handle() << " v2: " << v2.handle() << " ";
	     gt::Edge   e1(ed), e2(ed);

	    (*c).FlipEdge(v1,e1); 
	    (*c).FlipEdge(v2,e2);
	     gt::Edge   e12(e1), e21(e2);
	    (*c).FlipEdge(e1.V2(),e12);
	    (*c).FlipEdge(e2.V1(),e21);

	    out << '\n' 
		<< "e   =                         " << VNum(ed.V1()) << " -> " << VNum(ed.V2()) << "\n"
		<< "e1  = Flip(v1,e):             " << VNum(e1.V1()) << " -> " << VNum(e1.V2()) << ";  "
		<< "e2  = Flip(v2,e):             " << VNum(e2.V1()) << " -> " << VNum(e2.V2()) << "\n"
		<< "e12 = Flip(e1.V2(),e1) ( == e)" << VNum(e12.V1()) << " -> " << VNum(e12.V2()) << ";  "
		<< "e21 = Flip(e2.V1(),e2) ( == e)" << VNum(e21.V1()) << " -> " << VNum(e21.V2()) << "\n";
	  }	  
	  typedef  gt::FacetOnCellIterator FacetOnCellIterator;
	  out << "      outward normal = " << geom.outer_normal(FacetOnCellIterator(cc)) << "\n";
	}
	out << "\n";
      } // if(test_neigbours)  
  
      // loop over all edges e of c, print e.V(1) and e.V(2)
      if(test_cell_edges) {
	out << "  Edges:  ";
	 gt::EdgeOnCellIterator e((*c).FirstEdge());
	for(; ! e.IsDone(); ++e) {
	  out << " No. " << e.LocalNumber() << " : ("
	      << geom.coord((*e).V1()) << ","
	      << geom.coord((*e).V2()) << "), ";
	}
	out << '\n';
      } 
      
    } // for ( gt::CellIterator c=CC.FirstCell(); ...
  } // if (test_cells)
  
  
  // loop over all edges of grid CC, print start/end, normal and length.
  
  if(test_edges) {
     gt::EdgeIterator e(CC.FirstEdge());
    for(; ! e.IsDone(); ++e) {
       gt::Edge E(*e);
      out << "Edge ["
	  << VNum(E.V1()) << " <-> " << VNum(E.V2()) << "]  " 
	  << "  [" << geom.coord(E.V1()) << " <-> "
	  << geom.coord(E.V2())  << "]  " 
	  << "adj. cells: " 
	  << (CC.IsInside(E.C1()) ? CNum(E.C1()) : -1)
	  << "," 
	  << (CC.IsInside(E.C2()) ? CNum(E.C2()) : -1)
	  << "  "
	  << " volume = "  << geom.volume(E) 
	  << " normal = "  << geom.normal(E) << "\n";
    }
  }
  
  //  loop over all boundary facets.
  if(test_boundary_facets){
    out << "\nBoundary-Edges: \n";
      gt::BoundaryFacetIterator bf(CC.FirstBoundaryFacet());
     for(; ! bf.IsDone(); ++bf) {
       out << "Edge (" 
	   << geom.coord((*bf).V1()) << ","
	   << geom.coord((*bf).V2()) 
	   << ")\n";
     }
     
     out << "\n";
  } 

  if(test_boundary_range) {
    typedef BoundaryRange<Complex2D> bd_range;
    bd_range Bd(CC);
   
   
    out << "-----------------------------------------------------------\n"
	<< "                       boundary\n"
	<< "-----------------------------------------------------------\n";
    
    
    // loop over all edges on all boundary components

    out << "\nBoundary-Edges:\n";
    partial_grid_function< gt::Edge,int> BdFNum(CC,-1);
     bd_range::FacetIterator bf(Bd.FirstFacet());
    for(int fcnt = 0; ! bf.IsDone(); ++bf, ++fcnt) {
      BdFNum[*bf] = fcnt;
      out << "# " << BdFNum(*bf)
	  << "  (V1,V2) =  (" << VNum((*bf).V1()) << "," << VNum((*bf).V2())  << ")"
	  << "  coords =  ("<< geom.coord((*bf).V1()) << ","<< geom.coord((*bf).V2()) << ")\n";
    }
    out << "\n";

    // loop over all vertices on all boundary components
    
    partial_grid_function< gt::Vertex,int> BdVNum(CC,-1);
    out << "\nBoundary Vertices:\n";
    bd_range::VertexIterator bv(Bd.FirstVertex());
    for(int vcnt=0; ! bv.IsDone(); ++bv,++vcnt) {
      BdVNum[*bv] = vcnt;
      out << "# " << BdVNum(*bv) 
	  << "  VNum = " << VNum(*bv)
	  << "  coord = " << geom.coord(*bv) << "\n";
    }
    out << "\n";
 
  }
}

} // namespace GrAL 
