#include "Gral/Grids/Triang2D/test-triang2d.h"
#include "Gral/Grids/Triang2D/partial-grid-functions.h"

#include "Gral/Algorithms/cell-neighbor-search.h"


struct nb_type {
  nb_type(int * nbs) : nb(nbs) {}
  int& operator[](Triang2D::FacetOnCellIterator const& fc)
  { return nb[3*fc.TheCell().handle() + fc.local_handle()]; }
private:
  int * nb;
};


TestTriang2D::TestTriang2D()
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

void TestTriang2D::register_at(ControlDevice& Ctrl, const std::string&) {
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

void TestTriang2D::test_iterators(const Triang2D& CC,
				  std::ostream& out) 
{

  typedef grid_types<Triang2D> gt;
 
  // enumerate all vertices 
  grid_function<gt::Vertex,int> VNum(CC);
  {  
    int vnum = 0;
    gt::VertexIterator v(CC);
    for(; ! v.IsDone(); ++v,++vnum){
      VNum[*v] = vnum;
    }
  }

  // ... and all cells.
  grid_function< gt::Cell,int> CNum(CC);
  {
    int cnum = 1;
    gt::CellIterator c(CC);
    for(; ! c.IsDone(); ++c,++cnum){
      CNum[*c] = cnum;
    }
  }


  // demonstrate grid-function iterators.
  // note that the order is not necessarily the same as the
  // order of the cells!
   grid_function< gt::Cell,int>::iterator i1;
  for(i1 = CNum.begin(); i1 != CNum.end(); ++i1)
    (*i1) -= 1;

  // the other way of iterating over a grid-function
  //  grid_types<grid_function<gt::Cell,int>::Grid>::CellIterator ci = CNum.TheGrid().FirstCell();
   grid_function< gt::Cell,int>::ElementIterator ci = CNum.FirstElement();
   for(; ! ci.IsDone(); ++ci) {
     CNum[*ci] += 1;
     CNum[*ci] -= 1;
   }

  // calculate cell neighbors
  int* nbs = new int[3*CC.NumOfCells()];
  nb_type Nbs(nbs);
  CalculateNeighborCells(Nbs,CC);

  out << CC.NumOfVertices() << " vertices,  " << CC.NumOfCells()  << " cells.\n";


  // loop over all vertices of grid CC
  if (test_vertices) {
    for( gt::VertexIterator vv(CC); ! vv.IsDone(); ++vv) {
      out << "Vertex " << VNum[vv] << '\n';
    }
  }

  // loop over all cells of grid CC
  if (test_cells) {
    for( gt::CellIterator c(CC);!c.IsDone();++c){
      out << "Cell " << CNum(*c) <<  " has "
	  << (*c).NumOfVertices() << "  Vertices:  ";
      
      // loop over all vertices vc of cell c, print number of vc
      if( test_cell_vertices) {
	 gt::VertexOnCellIterator vc;
	for(vc = (*c).FirstVertex(); ! vc.IsDone(); ++vc)
	  out << VNum[*vc] << " ";
      }

      // loop over all neighbors  
      if(test_cell_neighbours) {
	out << "\n"
	    << "  Neighbors ";
	for(gt::FacetOnCellIterator fc(c); ! fc.IsDone(); ++fc)
	  out << Nbs[fc] << ' ';
      }
      
      // loop over all edges e of c, print e.V(1) and e.V(2)
      if(test_cell_edges) {
	out << "\n"
	    << "  Edges:  ";
	 gt::EdgeOnCellIterator e((*c));
	for(; ! e.IsDone(); ++e) {
	  out << " No. " << e.local_handle() << " : ("
	      << (*e).V1().handle()  << ","
	      << (*e).V2().handle()  << "), ";
	}
	out << '\n';
      } 

 
      for(gt::EdgeOnCellIterator ec(*c); ! ec.IsDone(); ++ec)
	for(gt::VertexOnEdgeIterator vec(*ec); ! vec.IsDone(); ++vec) {
	  gt::Vertex sv  = CC.switched_vertex(*vec,*ec);
	  gt::Vertex sv1 = CC.switched_vertex(sv,*ec);
	  REQUIRE_ALWAYS( sv1 == *vec, "sv1= " << sv1.handle() << " vec=" << vec.handle(), 1);
	  gt::Edge se  = CC.switched_edge(*vec, *ec, *c);
	  gt::Edge se1 = CC.switched_edge(*vec, se,  *c);
	  REQUIRE_ALWAYS( se1 == *ec, "se1= " << se1.handle() << " ec=" << ec.handle(), 1);
	}

    } // for ( gt::CellIterator c=CC.FirstCell(); ...
    
  } // if (test_cells)
  
  
  // loop over all edges of grid CC, print start/end, normal and length.
  
  
}
