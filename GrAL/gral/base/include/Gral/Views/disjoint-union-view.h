#ifndef GB_GRAL_DISJOINT_UNION_VIEW_H
#define GB_GRAL_DISJOINT_UNION_VIEW_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/cell-vertex-input-grid.h"
#include <functional>

#include "Utility/pre-post-conditions.h"
#include "Container/my-hash-map.h"

namespace GrAL {

  /*! \defgroup disjoint_union_view View for handling formal unions of grids
   
  \ingroup gridviews

  Components of interest to a user are:
  - disjoint_union_view::grid_view
  - disjoint_union_view::geom_view

  The templates grid_types and grid_function are specialized.

  \see \ref gridviews module
  \see Test in \ref test-disjoint-union.C
  */


  /*! \brief enlosing namespace for disjoint_union_view
    \ingroup disjoint_union_view
    \see Module disjoint_union_view
  */
  namespace disjoint_union_view {

    template<class GRID1, class GRID2>    class vertex_handle_view;
    template<class GRID1, class GRID2>    class cell_handle_view;
    template<class GRID1, class GRID2>    class Vertex_view;
    template<class GRID1, class GRID2>    class Cell_view;
    template<class GRID1, class GRID2>    class VertexOnCellIterator_view;
    template<class GRID1, class GRID2>    class grid_view;


    template<class GRID1, class GRID2>
    struct grid_types_base
    {

      // used by element classes below

      typedef GRID1 grid_type_1;
      typedef GRID2 grid_type_2;

      typedef grid_types<GRID1> gt1;
      typedef grid_types<GRID2> gt2;

      typedef typename gt1::vertex_handle vertex_handle_1;
      typedef typename gt2::vertex_handle vertex_handle_2;
      typedef typename gt1::cell_handle   cell_handle_1;
      typedef typename gt2::cell_handle   cell_handle_2;

      typedef typename gt1::Vertex Vertex1;
      typedef typename gt2::Vertex Vertex2;
      typedef typename gt1::Cell   Cell1;
      typedef typename gt2::Cell   Cell2;
      typedef typename gt1::VertexIterator       VertexIterator1;
      typedef typename gt2::VertexIterator       VertexIterator2;
      typedef typename gt1::CellIterator         CellIterator1;
      typedef typename gt2::CellIterator         CellIterator2;
      typedef typename gt1::VertexOnCellIterator VertexOnCellIterator1;
      typedef typename gt2::VertexOnCellIterator VertexOnCellIterator2;

      // public use

      // should use max of both dimensions.
      typedef typename gt1::dimension_tag dimension_tag;

      typedef grid_view  <GRID1, GRID2>  grid_type;

      typedef vertex_handle_view<GRID1,GRID2> vertex_handle;
      typedef cell_handle_view  <GRID1,GRID2> cell_handle;

      typedef Vertex_view<GRID1, GRID2>  Vertex;
      typedef Vertex_view<GRID1, GRID2>  VertexIterator;
      typedef Cell_view  <GRID1, GRID2>  Cell;
      typedef Cell_view  <GRID1, GRID2>  CellIterator;

      typedef VertexOnCellIterator_view<GRID1, GRID2>  VertexOnCellIterator;

    };

    template<class GRID1, class GRID2>
    class vertex_handle_view 
      : public grid_types_base<GRID1,GRID2> 
    {
      typedef vertex_handle_view<GRID1,GRID2> self;
      typedef grid_types_base<GRID1,GRID2>    base;
      typedef typename base::vertex_handle_1 vertex_handle_1;
      typedef typename base::vertex_handle_2 vertex_handle_2;
    private:
      vertex_handle_1 v1_;
      vertex_handle_2 v2_;
      int w_;
    public:
      vertex_handle_view() 
      { ENSURE( ( (-1 == (w_ = -1)) || true), "Debug-only initialization",1); }
      vertex_handle_view(vertex_handle_1 vv1, 
			 vertex_handle_2 vv2, 
			 int w)
	: v1_(vv1), v2_(vv2), w_(w) 
      { 
	REQUIRE( (w_ == 1 || w_ == 2), "invalid value, w_ == " << w_,1); 
      }
      
      int which() const {
	REQUIRE( (w_ == 1 || w_ == 2), "invalid value, w_ == " << w_,1); 
	return w_;
      }
      vertex_handle_1 v1() const {
	REQUIRE((which() == 1), "v1 invalid!\n",1);
	return v1_;
      }
      vertex_handle_2 v2() const {
	REQUIRE((which() == 2), "v2 invalid!\n",1);
	return v2_;
      }
      
      bool operator==(self const& rhs) const
      { return (which() == rhs.which()) 
	  && (which() == 1 ? v1_ == rhs.v1_ : v2_ == rhs.v2_);
      }
      bool operator!=(self const& rhs) const 
      { return !((*this) == rhs);}
      bool operator < (self const& rhs) const {
	return (which() < rhs.which() || 
		(which() == 1 ? v1_ < rhs.v1_ : v2_ < rhs.v2_));
      }

    };


    template<class GRID1, class GRID2>
    struct cell_handle_view 
      : public grid_types_base<GRID1,GRID2> {
      typedef cell_handle_view<GRID1,GRID2> self;
      typedef grid_types_base<GRID1,GRID2>    base;
      typedef typename base::cell_handle_1 cell_handle_1;
      typedef typename base::cell_handle_2 cell_handle_2;
    private:
      cell_handle_1 c1_;
      cell_handle_2 c2_;
      int w_;
    public:
      cell_handle_view() 
      { ENSURE( ( (-1 == (which = -1)) || true), "Debug-only initialization",1); }
      cell_handle_view(cell_handle_1 cc1, cell_handle_2 cc2, int w)
	: c1_(cc1), c2_(cc2), w_(w) 
      {
	REQUIRE( (w_ == 1 || w_ == 2), "invalid value, w_ == " << w_,1); 
      }
      
      int which() const {
	REQUIRE( (w_ == 1 || w_ == 2), "invalid value, w_ == " << w_,1); 
	return w_;
      }
      cell_handle_1 c1() const { 
	REQUIRE( (which() == 1), "c1 invalid!\n",1);
	return c1_;
      }
      cell_handle_2 c2() const { 
	REQUIRE( (which() == 2), "c2 invalid!\n",1);
	return c2_;
      }
  
      bool operator==(self const& rhs) const {
	return (which() == rhs.which()) 
	  && (which() == 1 ? c1_ == rhs.c1_ : c2_ == rhs.c2_);
      }
      bool operator!=(self const& rhs) const 
      { return !((*this) == rhs);}

    };


    template<class GRID1, class GRID2>
    class element_base :
      public grid_types_base<GRID1,GRID2> {
      typedef grid_types_base<GRID1,GRID2> base;
    public:
      typedef typename base::grid_type grid_type;
    public:
      element_base(): g(0) {}
      element_base(grid_type const& gg) : g(&gg) {}
      
      grid_type const& TheGrid() const { return *g;}
      bool bound() const { return (g != 0);}
    private:
      grid_type const* g;
    };


    /*! \brief View two separate grids as a single one

    This view is useful for passing several separate grids
    to algorithms expecting a single grid, for example for 
    output of several grids in one file.

    The view is restricted to be a model of $GrAL Cell-VertexInputGridRange
    regardless of the capabilities of the template parameters
    (which must support at least the $GrAL Cell-VertexInputGridRange functionality).

    \todo support archetypes.
    \todo grid_function_view
    */
    template<class GRID1, class GRID2>
    class grid_view : public grid_types_base<GRID1,GRID2> 
    {
      typedef grid_types_base<GRID1,GRID2>  base;
    public:
      typedef typename base::grid_type_1 grid_type_1;
      typedef typename base::grid_type_2 grid_type_2;
    private:
      grid_type_1 const& g1;
      grid_type_2 const& g2;

    public:
      grid_view(grid_type_1 const& gg1,
		grid_type_2 const& gg2)
	: g1(gg1), g2(gg2) {}


      unsigned NumOfVertices() const 
      { return Grid1().NumOfVertices() + Grid2().NumOfVertices();}
      unsigned NumOfCells   () const 
      { return Grid1().NumOfCells   () + Grid2().NumOfCells   ();}

      Vertex_view<GRID1,GRID2>  FirstVertex() const;
      Cell_view  <GRID1,GRID2>  FirstCell()   const;

      grid_type_1 const& Grid1() const { return g1;}
      grid_type_2 const& Grid2() const { return g2;}

      // morphisms G1 <-> Union
      // class injection1;
      // class projection1 {};
    };




    //---- Vertex class --------


    template<class GRID1, class GRID2>
    class Vertex_view
      : public element_base<GRID1, GRID2> {
      typedef Vertex_view<GRID1, GRID2> self;
      typedef element_base<GRID1,GRID2> base;
    public:
      typedef typename base::VertexIterator1 VertexIterator1;
      typedef typename base::VertexIterator2 VertexIterator2;
      typedef typename base::Vertex1         Vertex1;
      typedef typename base::Vertex2         Vertex2;
      typedef typename base::vertex_handle_1  vertex_handle_1;
      typedef typename base::vertex_handle_2  vertex_handle_2;
      typedef typename base::vertex_handle    vertex_handle;

      typedef typename base::grid_type       grid_type;
    private:
      VertexIterator1 v1_;
      VertexIterator2 v2_;
    public:
      Vertex_view() {}
      Vertex_view(grid_type const& g) 
	: base(g), v1_(g.Grid1()), v2_(g.Grid2())
      {} 
      Vertex_view(grid_type const& g, vertex_handle v) 
	: base(g)
      { 
	if(v.which() == 1) {
	  v1_ = VertexIterator1(g.Grid1(),v.v1());
	  v2_ = VertexIterator2(g.Grid2());
	}
	else {
	  v1_ = VertexIterator1(g.Grid1().EndVertex());
	  v2_ = VertexIterator2(g.Grid2(), v.v2());
	}
      }
      self& operator++() { 
	if (! v1_.IsDone()) ++v1_;
	else                ++v2_;
	return *this;
      }
     
     
      int which()  const { return (v1_.IsDone() ? 2 : 1);}
      Vertex1 v1() const { return *v1_;}
      Vertex2 v2() const { return *v2_;}

      self const& operator*() const { return *this;} 
      bool IsDone() const { return (v2_.IsDone());}
      vertex_handle handle() const {
	return (! v1_.IsDone() ?  // if v1_.IsDone(), v1_.handle() is not defined.
		vertex_handle(v1_.handle(),       v2_.handle(), 1) :
		vertex_handle(vertex_handle_1(),  v2_.handle(), 2));
      }

      bool operator==(self const& rhs) const { 
	return (which() == rhs.which()) 
	  && (which() == 1 ? v1_ == rhs.v1_ : v2_ == rhs.v2_);
      }
      bool operator!=(self const& rhs) const 
      { return !((*this) == rhs);}

    };


    //---- Cell class --------

    template<class GRID1, class GRID2>
    class Cell_view
      : public element_base<GRID1,GRID2> {
      typedef Cell_view   <GRID1,GRID2> self;
      typedef element_base<GRID1,GRID2> base;
      friend class VertexOnCellIterator_view<GRID1,GRID2>;
    public:
      typedef typename base::CellIterator1 CellIterator1;
      typedef typename base::CellIterator2 CellIterator2;
      typedef typename base::Cell1         Cell1;
      typedef typename base::Cell2         Cell2;
      typedef typename base::cell_handle_1  cell_handle_1;
      typedef typename base::cell_handle_2  cell_handle_2;
      typedef typename base::cell_handle    cell_handle;

      typedef typename base::grid_type      grid_type;
    private:
      CellIterator1 c1_;
      CellIterator2 c2_;
    public:
      Cell_view() {}
      Cell_view(grid_type const& g) 
	: base(g), c1_(g.Grid1()), c2_(g.Grid2()) 
      {}
      
      Cell_view(grid_type const& g, cell_handle c) 
	: base(g)
      {
	if(c.which() == 1) {
	  c1_ = CellIterator1(g.Grid1(),c.c1());
	  c2_ = CellIterator2(g.Grid2());
	}
	else {
	  REQUIRE(c.which() == 2, "invalid value which() == " << c.which(),1);
	  c1_ = CellIterator1(g.Grid1().EndCell());
	  c2_ = CellIterator2(g.Grid2(), c.c2());
	}
      }
      self& operator++() { 
	if (!c1_.IsDone()) ++c1_;
	else               ++c2_;
	return *this;
      }
      self const& operator*() const { return *this;}

      // note: c2_.IsDone() is not enough: 2nd grid might be empty. 
      bool IsDone() const { return c2_.IsDone();}
      cell_handle handle() const { 
	return (! c1_. IsDone() ?
		cell_handle(c1_.handle(),    c2_.handle(), 1) :
		cell_handle(cell_handle_1(), c2_.handle(), 2));
      }

      int which() const { return (c1_.IsDone() ? 2 : 1);}
      Cell1 c1() const { return *c1_;}
      Cell2 c2() const { return *c2_;}

      VertexOnCellIterator_view<GRID1,GRID2> FirstVertex() const;
      unsigned NumOfVertices() const { 
	return (which() == 1 ? (*c1_).NumOfVertices()  : (*c2_).NumOfVertices());
      }
     
      bool operator==(self const& rhs) const 
      { return (which() == rhs.which()) 
	  && (which() == 1 ? c1_ == rhs.c1_ : c2_ == rhs.c2_);}
      bool operator!=(self const& rhs) const 
      { return !((*this) == rhs);}
    };





    template<class GRID1, class GRID2>
    class VertexOnCellIterator_view 
      : public element_base<GRID1,GRID2> {
      typedef VertexOnCellIterator_view<GRID1,GRID2> self;
      typedef element_base             <GRID1,GRID2> base;
    public:
      typedef typename base::VertexOnCellIterator1 VertexOnCellIterator1;
      typedef typename base::VertexOnCellIterator2 VertexOnCellIterator2;
      typedef typename base::Vertex    Vertex;
      typedef typename base::Cell      Cell;
      typedef typename base::vertex_handle vertex_handle;

      typedef typename base::grid_type grid_type;
      // using base::vertex_handle_1;
      // using base::vertex_handle_2;
      using base::TheGrid;
    private:
      VertexOnCellIterator1 vc1;
      VertexOnCellIterator2 vc2;
      Cell c;
    public:
      VertexOnCellIterator_view() {}
      VertexOnCellIterator_view(Cell const& cc) 
	: base(cc.TheGrid()), c(cc)
      { 
	if(c.which() == 1) vc1 = c.c1().FirstVertex();
	else               vc2 = c.c2().FirstVertex();
      }
      vertex_handle handle() const { 
	if(c.which() == 1)
	  return  vertex_handle(vc1.handle()     ,typename base::vertex_handle_2(), 1);
	else           
	  return  vertex_handle(typename base::vertex_handle_1(),vc2.handle(),      2);
      }
      self& operator++() { if (which() == 1) ++vc1; else ++vc2; return *this;}
      Vertex operator*() const { return Vertex(TheGrid(), handle());}
      bool IsDone() const { return (which() == 1 ? vc1.IsDone() : vc2.IsDone());}
      int which() const { return c.which();}
    
      bool operator==(self const& rhs) const 
      { return (which() == 1 ? vc1 == rhs.vc1 : vc2 == rhs.vc2);}
      bool operator!=(self const& rhs) const 
      { return !((*this) == rhs);}
    };


    //----- inline functions ------

    template<class GRID1, class GRID2>
    inline
    VertexOnCellIterator_view<GRID1, GRID2>
    Cell_view<GRID1,GRID2>::FirstVertex() const
    { return typename base::VertexOnCellIterator(*this);}

    template<class GRID1, class GRID2>
    inline
    Vertex_view<GRID1,GRID2> 
    grid_view<GRID1,GRID2>::FirstVertex() const { return typename base::VertexIterator(*this);}
 
    template<class GRID1, class GRID2>
    inline
    Cell_view<GRID1,GRID2>
    grid_view<GRID1,GRID2>::FirstCell()   const { return typename base::CellIterator(*this);}



    //---- grid geometry ------------

    template<class GEOM1, class GEOM2>
    class geom_view {
    public:
      typedef GEOM1 geom_type_1;
      typedef GEOM2 geom_type_2;
      typedef typename GEOM1::grid_type  grid_type_1;
      typedef typename GEOM2::grid_type  grid_type_2;
      typedef typename GEOM1::coord_type coord_type_1;
      typedef typename GEOM2::coord_type coord_type_2;

      typedef grid_view<grid_type_1, grid_type_2> grid_type;
      typedef grid_types<grid_type> gt;
      typedef typename gt::Vertex Vertex;
      // does not always work ...
      typedef coord_type_1 coord_type;
    private:
      grid_view<grid_type_1, grid_type_2> const* g;
      GEOM1 const* geom1;
      GEOM2 const* geom2;
   
    public:
      geom_view() : g(0) {}
      geom_view(grid_type const& gg, 
		geom_type_1 const& geo1,
		geom_type_2 const& geo2) : g(&gg), geom1(&geo1), geom2(&geo2) {}


      coord_type coord(Vertex const& v) const {
	if(v.which() == 1)
	  return geom1->coord(v.v1());
	else {
	  coord_type p;
	  assign_point(p, geom2->coord(v.v2()));
	  return p;
	}
      }
      grid_type const& TheGrid() const { return *g;}
    };

    /*

    template<class GF1, class GF2>
    class grid_function_view {
    public:
    typedef typename GF1::grid_type grid_type_1;
    typedef typename GF2::grid_type grid_type_2;
    typedef grid_view<grid_type_1, grid_type_2> grid_type;

    // typedef ???? element_type;


    // both value types should be the same,
    // or at least on convertible to the other.
    typedef typename GF1::value_type value_type;
    private:
    grid_type const* g;
    GF1       const* gf1;
    GF2       const* gf2;
    public:
    grid_function_view() : g(0) {}
    grid_function_view(grid_type const& gg,
    GF1       const& ggf1,
    GF2       const& ggf2)
    : g(&gg), gf1(&ggf1), gf2(&ggf2) {}
       
    value_type const& operator()(element_type const& E) const
    { return (e.which() == 1 ? (*gf1)(e.first()) : (*gf2)(e.second()));}
    };
    */

  } // namespace disjoint_union_view


  // specialization of grid_types

  template<class GRID1, class GRID2>
  struct grid_types<disjoint_union_view::grid_view<GRID1,GRID2> >
    : public disjoint_union_view::grid_types_base<GRID1,GRID2>
  { };

} // namespace GrAL

// specialization of hash<> for element handles

namespace STDEXT {
  template<class T> struct hash;
 
  template<class GRID1, class GRID2>
  struct hash<GrAL::disjoint_union_view::vertex_handle_view<GRID1,GRID2> >
  {
    typedef GrAL::disjoint_union_view::vertex_handle_view<GRID1,GRID2>  vertex_handle;
    typedef GrAL::disjoint_union_view::grid_types_base<GRID1,GRID2> gta;
    typedef hash<typename gta::vertex_handle_1> hash1;
    typedef hash<typename gta::vertex_handle_2> hash2;
    typedef vertex_handle key_type;
    typedef vertex_handle argument_type;
    typedef size_t        result_type;
    hash1 h1;
    hash2 h2;

    hash() {}
    // this is unsatisfactory, because it will typically map
    // different vertices onto the same value.
    size_t operator()(vertex_handle const& v) const 
    { return (v.which() == 1 ? h1(v.v1()) : h2(v.v2())); } 
  };

  template<class GRID1, class GRID2>
  struct hash<GrAL::disjoint_union_view::cell_handle_view<GRID1,GRID2> >
  {
    typedef GrAL::disjoint_union_view::cell_handle_view<GRID1,GRID2>  cell_handle;
    typedef GrAL::disjoint_union_view::grid_types_base<GRID1,GRID2> gta;
    typedef hash<typename gta::cell_handle_1> hash1;
    typedef hash<typename gta::cell_handle_2> hash2;
    typedef cell_handle key_type;
    typedef cell_handle argument_type;
    typedef size_t      result_type;

    hash1 h1;
    hash2 h2;

    hash() {}
    // this is unsatisfctory, because it will typically map
    // different vertices onto the same value.
    size_t operator()(cell_handle const& c) const 
    { return (c.which() == 1 ? h1(c.c1()) : h2(c.c2())); } 
  };
};  


namespace GrAL {
  // specialization of element_traits

  template<class GRID1, class GRID2>
  struct element_traits<disjoint_union_view::Vertex_view<GRID1,GRID2> >
    : public element_traits_vertex_base<disjoint_union_view::grid_view<GRID1,GRID2> > 
  {
    typedef element_traits_vertex_base<disjoint_union_view::grid_view<GRID1,GRID2> >  base;
    class hasher_type : public base::hasher_type_elem_base

    {
      typedef disjoint_union_view::grid_types_base<GRID1,GRID2> gt;
      typedef typename gt::vertex_handle_1 vertex_handle_1;
      typedef typename gt::vertex_handle_2 vertex_handle_2;
      typedef typename gt::Vertex          Vertex;

      typedef typename element_traits<vertex_handle_1>::hasher_type hash1;  
      typedef typename element_traits<vertex_handle_2>::hasher_type hash2;

      hash1 h1;
      hash2 h2;  
    public:
      size_t operator()(Vertex const& v) const
      { return (v.which() == 1 ? 
		h1(v.handle().v1()) : 
		h2(v.handle().v2() + v.TheGrid().Grid1().NumOfVertices()));
      }
    };
  };



  template<class GRID1, class GRID2>
  struct element_traits<disjoint_union_view::Cell_view<GRID1,GRID2> >
    : public element_traits_cell_base<disjoint_union_view::grid_view<GRID1,GRID2> > 
  {
    typedef element_traits_cell_base<disjoint_union_view::grid_view<GRID1,GRID2> > base;

    class hasher_type  : public base::hasher_type_elem_base
    {
      typedef disjoint_union_view::grid_types_base<GRID1,GRID2> gt;
      typedef typename gt::cell_handle_1 cell_handle_1;
      typedef typename gt::cell_handle_2 cell_handle_2;
      typedef typename gt::Cell          Cell;

      typedef typename element_traits<cell_handle_1>::hasher_type hash1;  
      typedef typename element_traits<cell_handle_2>::hasher_type hash2;

      hash1 h1;
      hash2 h2;  
    public:
      size_t operator()(Cell const& v) const
      { return (v.which() == 1 ? 
		h1(v.handle().v1()) : 
		h2(v.handle().v2() + v.TheGrid().Grid1().NumOfCells()));
      }
    };
  };


  template<class GRID1, class GRID2, class T>
  class grid_function<disjoint_union_view::Vertex_view<GRID1,GRID2>, T>
  {
    typedef grid_types<disjoint_union_view::grid_view<GRID1,GRID2> > gt;
    typedef typename gt::Vertex Vertex;
  private:
    typedef typename gt::grid_type grid_type;
    grid_type const*                       g;
    grid_function<typename gt::Vertex1, T> gf1;  
    grid_function<typename gt::Vertex2, T> gf2;

  public:
    grid_function() : g(0) {}
    grid_function(grid_type const& gg) : g(&gg), gf1(gg.Grid1()), gf2(gg.Grid2()) {}

    T const& operator()(Vertex const& v) const { 
      return (v.which() == 1 ? gf1(v.v1()) : gf2(v.v2()));
    }
    T      & operator[](Vertex const& v)       { 
      return (v.which() == 1 ? gf1[v.v1()] : gf2[v.v2()]);
    }

    unsigned size() const { return gf1.size() + gf2.size();}
    typename gt::grid_type const& TheGrid() const { return *g;}
  };


  template<class GRID1, class GRID2, class T>
  class grid_function<disjoint_union_view::Cell_view<GRID1,GRID2>, T>
  {
    typedef grid_types<disjoint_union_view::grid_view<GRID1,GRID2> > gt;
    typedef typename gt::Cell  Cell;
  private:
    typedef typename gt::grid_type grid_type;
    grid_type const*                       g;
    grid_function<typename gt::Cell1, T> gf1;  
    grid_function<typename gt::Cell2, T> gf2;

  public:
    grid_function() : g(0) {}
    grid_function(grid_type const& gg) : g(&gg), gf1(gg.Grid1()), gf2(gg.Grid2()) {}

    T const& operator()(Cell const& c) const { 
      return (c.which() == 1 ? gf1(c.c1()) : gf2(c.c2()));
    }
    T      & operator[](Cell const& c)       { 
      return (c.which() == 1 ? gf1[c.c1()] : gf2[c.c2()]);
    }

    unsigned size() const { return gf1.size() + gf2.size();}
    typename gt::grid_type const& TheGrid() const { return *g;}
  };

} // namespace GrAL 

#endif




