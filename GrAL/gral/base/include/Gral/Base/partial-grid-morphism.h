#ifndef GRAL_GB_BASE_PARTIAL_GRID_MORPHISM_H
#define GRAL_GB_BASE_PARTIAL_GRID_MORPHISM_H

// LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"

#include "Utility/pre-post-conditions.h"

//#include "Gral/Base/partial-grid-functions.h"

template<class GDEF, class GIMG, class DIM>
class partial_grid_morphism_aux;

/*! \brief Partial morphism of grids
 
   This class implements injective mappings between the elements of two grids.
   Defined are operators() (read access) and [] (write access)
   for each element type in the grid, and also for element handles 
   (this requires element handles to be distinct types, and not just
   typedef int, see e.g. vertex_handle_int in module \ref elements.)
 */
template<class GDEF, class GIMG>
class partial_grid_morphism :
  public partial_grid_morphism_aux
     <GDEF,GIMG, typename grid_types<GDEF>::dimension_tag>
{
  typedef partial_grid_morphism_aux
     <GDEF,GIMG, typename grid_types<GDEF>::dimension_tag>
  base;
public:
  partial_grid_morphism() {}
  partial_grid_morphism(GDEF const& gdef, GIMG const& gimg)
    : base(gdef,gimg) {}

};


/*
template<class GDEF, class GIMG>
class partial_grid_morphism_base 
{
protected:
  GDEF const* g_def;
  GIMG const* g_img;
  
  partial_grid_morphism_base() {}
  partial_grid_morphism_base(GDEF const& gdef, GIMG const& gimg)
    : g_def(&gdef), g_img(&gimg) {}
  //  void init(GDEF const& gdef, GIMG const& gimg) { g_def = &gdef; g_img
public:
  GDEF const& DefGrid() const { 
    REQUIRE(g_def != 0, "no domain of definition!\n",1);
    return *g_def;
  }
  GIMG const& ImgGrid() const {
    REQUIRE(g_img != 0, "no image grid!\n",1);
    return *g_img;
  }
 
};


template<class GDEF, class GIMG,
         class EDEF, class EIMG,
         class GTDEF = grid_types<GDEF>, class GTIMG = grid_types<GIMG> >
class partial_element_morphism 
  : public virtual partial_grid_morphism_base<GDEF,GIMG>
{
private:
  typedef partial_grid_morphism_base<GDEF,GIMG> base;

  typedef EDEF ElementDef;
  typedef EIMG ElementImg;
  typedef element_traits<EIMG> etimg;
  typedef typename etimg::handle_type element_handle_img;

  partial_grid_function<ElementDef, element_handle_img> phi;

  struct ElementImg_Proxy {
   element_handle_img& h;
    ElementImg_Proxy(element_handle_img & hh) : h(hh) {}
    // must be const member, else compiler will complain
    // about non-const op on temporary
    void operator=(ElementImg const& e) const
      { h = e.handle();}
  };

protected:
  partial_element_morphism(GDEF const& gdef, GIMG const& gimg)
    : base(gdef,gimg), phi(gdef) {}

public:
   ElementImg operator()(ElementDef const& e) const 
    {
      return ElementImg(ImgGrid(), phi(e));
    }

  ElementImg_Proxy operator[](ElementDef const& e)
    {
      return ElementImg_Proxy(phi[e]);
    }
};


template<class GDEF, class GIMG>
class partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<2> >
  : public partial_element_morphism
   <GDEF,
    GIMG, 
    typename grid_types<GDEF>::Vertex,
    typename grid_types<GIMG>::Vertex>,
   public partial_element_morphism
   <GDEF,
    GIMG, 
    typename grid_types<GDEF>::Edge,
    typename grid_types<GIMG>::Edge>

{
  typedef partial_element_morphism
   <GDEF,
    GIMG, 
    typename grid_types<GDEF>::Vertex,
    typename grid_types<GIMG>::Vertex> base_vertex;
  typedef partial_element_morphism
   <GDEF,
    GIMG, 
    typename grid_types<GDEF>::Edge,
    typename grid_types<GIMG>::Edge> base_edge;
protected:
    partial_grid_morphism_aux(GDEF const& gdef, GIMG const& gimg)
      : base_vertex(gdef,gimg),
        base_edge(gdef,gimg) {}
public:
  using base_vertex::operator[];
  using base_edge::operator[];
  using base_vertex::operator();
  using base_edge::operator();
};
*/

/*! \brief Specialization of partial grid morphism for 2D case.


 */
template<class GDEF, class GIMG>
class partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<2> >
{
  typedef partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<2> > self;
public:
  typedef GDEF grid_type_def;
  typedef GIMG grid_type_img;
  typedef grid_types<GDEF> gtdef;
  typedef grid_types<GIMG> gtimg;
  typedef typename gtdef::Vertex VertexDef;
  typedef typename gtdef::vertex_handle vertex_handle_def;
  typedef typename gtdef::Edge   EdgeDef;
  typedef typename gtdef::edge_handle edge_handle_def;
  typedef typename gtdef::Cell   CellDef;
  typedef typename gtdef::cell_handle cell_handle_def;
  typedef typename gtimg::Vertex VertexImg;
  typedef typename gtimg::vertex_handle vertex_handle_img;
  typedef typename gtimg::Edge   EdgeImg;
  typedef typename gtimg::edge_handle edge_handle_img;
  typedef typename gtimg::Cell   CellImg;
  typedef typename gtimg::cell_handle cell_handle_img;

private:
  GDEF const* g_def;
  GIMG const* g_img;
  partial_grid_function<VertexDef,vertex_handle_img> phi_v;
  partial_grid_function<EdgeDef,  edge_handle_img>   phi_e;
  partial_grid_function<CellDef,  cell_handle_img>   phi_c;

  partial_grid_function<VertexImg,vertex_handle_def> phi_v_inv;
  partial_grid_function<EdgeImg,  edge_handle_def>   phi_e_inv;
  partial_grid_function<CellImg,  cell_handle_def>   phi_c_inv;

public:
  partial_grid_morphism_aux() : g_def(0), g_img(0) {}

  partial_grid_morphism_aux(GDEF const& gdef,
			    GIMG const& gimg)
    : g_def(&gdef), g_img(&gimg),
      phi_v(gdef), phi_e(gdef), phi_c(gdef),
      phi_v_inv(gimg), phi_e_inv(gimg), phi_c_inv(gimg)
    {}

  bool empty() const { return phi_v.empty() && phi_e.empty() && phi_c.empty();}
  bool full()  const { return phi_v.full() && phi_e.full() && phi_c.full();}

  void clear()
    { 
      phi_v.clear(); phi_v_inv.clear();
      phi_e.clear(); phi_e_inv.clear();
      phi_c.clear(); phi_c_inv.clear();

    }
  GDEF const& DefGrid() const { 
    REQUIRE(g_def != 0, "no domain of definition!\n",1);
    return *g_def;
  }
  GIMG const& ImgGrid() const {
    REQUIRE(g_img != 0, "no image grid!\n",1);
    return *g_img;
  }

  bool defined(VertexDef const& v) const { return phi_v.defined(v);}
  bool defined(EdgeDef   const& e) const { return phi_e.defined(e);}
  bool defined(CellDef   const& c) const { return phi_c.defined(c);}


  // --- vertex --- 

  VertexImg operator()(VertexDef const& v) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return VertexImg(*g_img,phi_v(v));
  }

  vertex_handle_img operator()(vertex_handle_def v) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_v(VertexDef(*g_def,v));
  }
  

  template<class MORPHISM>
  struct VertexImg_proxy {
    MORPHISM       & m;
    VertexDef const& v;
    //vertex_handle_img& h;
    //VertexImg_proxy(vertex_handle_img & hh) : h(hh) {}
    VertexImg_proxy(MORPHISM & mm, VertexDef const& vv)
      : m(mm), v(vv) {} 

    // must be const member, else compiler will complain
    // about non-const op on temporary
    void operator=(VertexImg const& vi) const
      {
	m.phi_v    [v ] = vi.handle();
	m.phi_v_inv[vi] = v.handle();
      }
  };
  friend class VertexImg_proxy<self>;

  VertexImg_proxy<self> operator[](VertexDef const& v)  {
    return VertexImg_proxy<self>(*this,v); 
  }

  // works if handle types are distinct!
  vertex_handle_img & operator[](vertex_handle_def v)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_v[VertexDef(*g_def,v)];
  }
  
  // --- edge ---

  EdgeImg operator()(EdgeDef const& e) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return EdgeImg(*g_img,phi_e(e));
  }

  edge_handle_img operator()(edge_handle_def e) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_e(EdgeDef(*g_def,e));
  }

  template<class MORPHISM>
  struct EdgeImg_proxy {
    MORPHISM       & m;
    EdgeDef   const& e;
    EdgeImg_proxy(MORPHISM & mm, EdgeDef const& ee)
      : m(mm), e(ee) {} 


    // must be const member, else compiler will complain
    // about non-const op on temporary
    void operator=(EdgeImg const& ei) const
      {
	m.phi_e    [e ] = ei.handle();
	m.phi_e_inv[ei] = e.handle();
      }

  };
  friend class EdgeImg_proxy<self>;

  EdgeImg_proxy<self> operator[](EdgeDef const& e)  {
    return EdgeImg_proxy<self>(*this,e);
  }

  edge_handle_img & operator[](edge_handle_def e)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_e[EdgeDef(*g_def,e)];
  }


  //------ cells ---------

  CellImg operator()(CellDef const& c) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return CellImg(*g_img,phi_c(c));
  }

  // cell handles must be distinct types
  cell_handle_img operator()(cell_handle_def v) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_v(CellDef(*g_def,v));
  }
  



  template<class MORPHISM>
  struct CellImg_proxy {
    MORPHISM       & m;
    CellDef   const& c;
    CellImg_proxy(MORPHISM & mm, CellDef const& cc)
      : m(mm), c(cc) {} 


    // must be const member, else compiler will complain
    // about non-const op on temporary
    void operator=(CellImg const& ci) const
      {
	m.phi_c    [c ] = ci.handle();
	m.phi_c_inv[ci] = c.handle();
      }

  };
  friend class CellImg_proxy<self>;

  CellImg_proxy<self> operator[](CellDef const& c)  {
    return CellImg_proxy<self>(*this,c);
  }

  // cell handles must be distinct types.
  cell_handle_img & operator[](cell_handle_def v)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_v[CellDef(*g_def,v)];
  }


  friend class inverse_type;
  class inverse_type {
    typedef partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<2> > mapping;
    
    mapping const& m;
    typedef typename mapping::VertexDef VertexDef;
    typedef typename mapping::VertexImg VertexImg;
    typedef typename mapping::EdgeDef   EdgeDef;
    typedef typename mapping::EdgeImg   EdgeImg;
    typedef typename mapping::CellDef   CellDef;
    typedef typename mapping::CellImg   CellImg;
  public:
    inverse_type(mapping const& mm) : m(mm) {}

    VertexDef operator()(VertexImg const& v) const 
      { return VertexDef(m.DefGrid(),m.phi_v_inv(v)); }
    EdgeDef operator()(EdgeImg const& e) const 
      { return EdgeDef(m.DefGrid(),m.phi_e_inv(e)); }
    CellDef operator()(CellImg const& c) const 
     { return CellDef(m.DefGrid(),m.phi_c_inv(c)); }

    bool defined(VertexImg const& v) const { return m.phi_v_inv.defined(v);}
    bool defined(EdgeImg   const& e) const { return m.phi_e_inv.defined(e);}
    bool defined(CellImg   const& c) const { return m.phi_c_inv.defined(c);}

  };


  inverse_type inverse() const { return inverse_type(*this);}
};


/*! \brief Specialization of grid morphism for 3D case.

   \todo Implement inverse morphism (inherit from base::inverse_type

 */

template<class GDEF, class GIMG>
class partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<3> >
  : public partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<2> >
{
  typedef partial_grid_morphism_aux<GDEF,GIMG,grid_dim_tag<2> > base;
public:
  typedef typename gtimg::Facet   FacetImg;
  typedef typename gtimg::facet_handle facet_handle_img;
  typedef typename gtdef::Facet   FacetDef;
  typedef typename gtdef::facet_handle facet_handle_def;
private:
  partial_grid_function<FacetDef,  facet_handle_img>   phi_f;
  //  partial_grid_function<FacetImg,  facet_handle_def>   phi_f_inv;

public:
  partial_grid_morphism_aux() {}
  partial_grid_morphism_aux(GDEF const& gdef, GIMG const& g_img)
    : base(g_def,g_img), phi_f(gdef) {}

  void clear() { 
    base::clear();
    phi_f.clear();
    // phi_f_inv.clear();
  }


  FacetImg operator()(FacetDef const& f) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return FacetImg(*g_img,phi_f(v));
  }

  facet_handle_img operator()(facet_handle_def f) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_f(FacetDef(*g_def,f));
  }

  struct FacetImg_proxy {
    facet_handle_img& h;
    FacetImg_proxy(facet_handle_img & hh) : h(hh) {}
    // must be const member, else compiler will complain
    // about non-const op on temporary
    void operator=(FacetImg const& f) const
      { h = f.handle();}
  };

  FacetImg_proxy operator[](FacetDef const& f)  {
    return FacetImg_proxy(phi_f[f]);
  }

  facet_handle_img & operator[](facet_handle_def f)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return phi_f[FacetDef(*g_def,f)];
  }

};


#endif




