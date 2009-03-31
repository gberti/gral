#ifndef NMWR_GB_GRAL_BASE_GRID_MORPHISM_H
#define NMWR_GB_GRAL_BASE_GRID_MORPHISM_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/vertex-set.h"

#include "Container/bijective-mapping.h"

namespace GrAL {

/*! \brief Grid vertex isomorphism 

   Representation for a one-to-one mapping between vertices of 
   two grids. Despite the name, this correspondence does not need
   to come from a grid isomorphism.

   \ingroup gridmorphisms
   \see \ref gridmorphisms module 


   \todo Specialize if both grids have consecutive handles
   \todo Make it independent of element being a vertex type
   (using map_element_iter_name<> like in partial_grid_function)
   \todo Can temporary vertex objects be avoided?
 */
template<class G_DEF, class G_IMG>
class vertex_morphism
{
  typedef vertex_morphism<G_DEF,G_IMG>  self;

  typedef G_DEF                         grid_type_def;
  typedef G_IMG                         grid_type_img;
  typedef grid_types<G_IMG>             gtimg;
  typedef grid_types<G_DEF>             gtdef;
  typedef typename gtimg::Vertex        VertexImg;
  typedef typename gtdef::Vertex        VertexDef;
  typedef typename gtimg::vertex_handle vertex_handle_img;
  typedef typename gtdef::vertex_handle vertex_handle_def;

  typedef vertex_morphism<grid_type_img, grid_type_def> inverse_type;

  grid_function<VertexDef,vertex_handle_img> f; // contains ref to g_def
  grid_type_img const*                       g_img;
  mutable  inverse_type   const*  inv;
  mutable  bool           inverse_owned;     

  void copy(self const& rhs) {
    f = rhs.f;
    g_img = rhs.g_img;
    inv = 0;
    inverse_owned = false;
  }

public:
  typedef VertexImg result_type;
  typedef VertexDef argument_type;

  vertex_morphism() : g_img(0), inv(0), inverse_owned(false) {}
  vertex_morphism(G_DEF const& g_def, G_IMG const& gg_img)
    : f(g_def), g_img(&gg_img), inv(0), inverse_owned(false) {}
  vertex_morphism(G_DEF const& g_def, G_IMG const& gg_img, 
                inverse_type const& i)
    : f(g_def), g_img(&gg_img), inv(&i), inverse_owned(false) {}

  vertex_morphism   (self const& rhs) { copy(rhs);}
  self & operator=  (self const& rhs) { if(this != &rhs) copy(rhs); return *this;}

  ~vertex_morphism() { clear();}

  inverse_type const& inverse()  const {
    if(inv == 0) init_inverse();
    return *inv;
  }
  
  void set_grids(G_DEF const& g_def, G_IMG const& gg_img) {
    f.set_grid(g_def);
    g_img = &gg_img;
    inverse_owned = false;
  }


  VertexImg operator()(VertexDef const& v) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return VertexImg(*g_img,f(v));
  }

  vertex_handle_img operator()(vertex_handle_def v) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f(VertexDef(f.TheGrid(),v));
  }

  vertex_handle_img & operator[](vertex_handle_def v)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f[VertexDef(f.TheGrid(),v)];
  }

  struct vertex_handle_img_proxy {
    vertex_handle_img & h;
    vertex_handle_img_proxy(vertex_handle_img& hh) : h(hh) {}
    void operator=(VertexImg const& v) { h = v.handle();}
    void operator=(vertex_handle_img const& v) { h = v;}
  };

  vertex_handle_img_proxy operator[](VertexDef const& v) {
        REQUIRE((g_img != 0), "No image grid!\n",1);
        return vertex_handle_img_proxy(f[v]);
  }

  grid_type_def const& DefGrid() const { return f.TheGrid();}
  grid_type_img const& ImgGrid() const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return *g_img;
  }

private:
  void clear() { if (inverse_owned) delete inv;}

  void init_inverse() const {
    inverse_type * pinv = new inverse_type(ImgGrid(),DefGrid(), *this);
    inverse_owned = true;
    for(typename gtdef::VertexIterator v(DefGrid()); ! v.IsDone(); ++v)
      (*pinv)[(*this)(*v).handle()] = v.handle();
    inv = pinv;
  }
};




/*! \brief Grid cell isomorphism 

   Representation for a one-to-one mapping between cells of 
   two grids. Despite the name, this correspondence does not need
   to come from a grid isomorphism.

   \ingroup gridmorphisms
   \see  \ref gridmorphisms module

   \todo Specialize if grids have consecutive handles
   \todo construct from vertex morphism
   \see vertex_morphism
*/
template<class G_DEF, class G_IMG>
class cell_morphism
{
  typedef cell_morphism<G_DEF,G_IMG> self;

  typedef G_DEF grid_type_def;
  typedef G_IMG grid_type_img;
  typedef grid_types<G_IMG> gtimg;
  typedef grid_types<G_DEF> gtdef;
  typedef typename gtimg::Cell CellImg;
  typedef typename gtdef::Cell CellDef;
  typedef typename gtimg::cell_handle cell_handle_img;
  typedef typename gtdef::cell_handle cell_handle_def;
  typedef cell_morphism<grid_type_img, grid_type_def> inverse_type;

  grid_function<CellDef,cell_handle_img> f;
  grid_type_img         const*    g_img;
  mutable  inverse_type const*  inv;
  mutable  bool           inverse_owned;     

  void copy(self const& rhs) {
    f = rhs.f;
    g_img = rhs.g_img;
    inv = 0;
    inverse_owned = false;
  }

public:
  typedef CellImg result_type;
  typedef CellDef argument_type;


  cell_morphism() : g_img(0), inv(0), inverse_owned(false) {}
  cell_morphism(G_DEF const& g_def, G_IMG const& gg_img)
    : f(g_def), g_img(&gg_img), inv(0), inverse_owned(false) {}

  cell_morphism(G_DEF const& g_def, G_IMG const& gg_img, 
                inverse_type const& i)
    : f(g_def), g_img(&gg_img), inv(&i), inverse_owned(false) {}

  cell_morphism   (self const& rhs) { copy(rhs);}
  self & operator=(self const& rhs) { if(this != &rhs) copy(rhs); return *this;}

  ~cell_morphism() { clear();}


  inverse_type const& inverse()  const {
    if(inv == 0) init_inverse();
    return *inv;
  }

  CellImg operator()(CellDef const& c) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return CellImg(*g_img,f(c));
  }

  cell_handle_img operator()(cell_handle_def c) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f(CellDef(f.TheGrid(),c));
  }

  cell_handle_img & operator[](cell_handle_def c)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f[CellDef(f.TheGrid(),c)];
  }


  struct cell_handle_img_proxy {
    cell_handle_img & h;
    cell_handle_img_proxy(cell_handle_img& hh) : h(hh) {}
    void operator=(CellImg const& c) { h = c.handle();}
    void operator=(cell_handle_img const& c) { h = c;}
  };

  cell_handle_img_proxy operator[](CellDef const& c) {
        REQUIRE((g_img != 0), "No image grid!\n",1);
        return cell_handle_img_proxy(f[c]);
  }
 

  grid_type_def const& DefGrid() const { return f.TheGrid();}
  grid_type_img const& ImgGrid() const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return *g_img;
  }

private:
  void clear() { if (inverse_owned) delete inv;}

  void init_inverse() const {
    inverse_type * pinv = new inverse_type(ImgGrid(),DefGrid(), *this);
    inverse_owned = true;
    for(typename gtdef::CellIterator c(DefGrid()); ! c.IsDone(); ++c)
      (*pinv)[(*this)(*c).handle()] = c.handle();
    inv = pinv;
  }
};


/*!  \brief Grid element morphism 

   Representation for a one-to-one mapping between elements of type E of 
   two grids. Despite the name, this correspondence does not need
   to come from a grid isomorphism.

   \ingroup gridmorphisms
   \see  \ref gridmorphisms module

 */
  template<class E_DEF, class E_IMG>
  class element_morphism {
    typedef element_morphism<E_DEF,E_IMG> self;
    
    typedef element_traits<E_DEF> et_def;
    typedef element_traits<E_IMG> et_img;
    
    typedef typename et_def::grid_type   grid_type_def;
    typedef typename et_img::grid_type   grid_type_img;
    typedef grid_types<grid_type_def>    gtdef;
    typedef grid_types<grid_type_img>    gtimg;
    typedef E_DEF                        element_def;
    typedef E_IMG                        element_img;
    typedef typename et_def::handle_type element_handle_def;
    typedef typename et_img::handle_type element_handle_img;

    typedef element_morphism<element_img, element_def>    inverse_type;
    typedef vertex_morphism<grid_type_def, grid_type_img> vertex_morphism_type;

    // data
    grid_function<element_def,element_handle_img> f;
    ref_ptr<grid_type_img const>    g_img;
    mutable  ref_ptr<inverse_type const>  inv;

    void copy(self const& rhs) {
      f = rhs.f;
      g_img = rhs.g_img;
    }

  public:
    typedef element_img result_type;
    typedef element_def argument_type;
    
    element_morphism() {}
    
    element_morphism(grid_type_def const& g_def, grid_type_img const& gg_img)
      : f(g_def), g_img(gg_img) {}

    element_morphism(grid_type_def const& g_def, grid_type_img const& gg_img, inverse_type const& inverse)
      : f(g_def), g_img(gg_img), inv(inverse) {}


      /*! \brief Initialise from a vertex isomorphism
	
          This constructor assumes that every element is uniquely determined by its vertex set.
      */
    element_morphism(vertex_morphism_type const & v_corr)
      : f(v_corr.DefGrid()), g_img(v_corr.ImgGrid()) 
    { init(v_corr); }

	
    void init(vertex_morphism_type const & v_corr);
    
    self & operator=(self const& rhs) { if(this != &rhs) copy(rhs); return *this;}

    inverse_type const& inverse()  const {
      if(inv == 0) init_inverse();
      return *inv;
    }

    result_type operator()(argument_type const& e) const { 
      REQUIRE((g_img != 0), "No image grid!\n",1);
      return element_img(*g_img,f(e));
    }

    element_handle_img operator()(element_handle_def h) const {
      REQUIRE((g_img != 0), "No image grid!\n",1);
      return f(element_def(f.TheGrid(),h));
    }

    element_handle_img & operator[](element_handle_def h)  {
      REQUIRE((g_img != 0), "No image grid!\n",1);
      return f[element_def(f.TheGrid(),h)];
    }


    struct element_handle_img_proxy {
      element_handle_img & h;
      element_handle_img_proxy(element_handle_img& hh) : h(hh) {}
      void operator=(element_img const& c) { h = c.handle();}
      void operator=(element_handle_img const& c) { h = c;}
    };
    
    element_handle_img_proxy operator[](element_def const& c) {
      REQUIRE((g_img != 0), "No image grid!\n",1);
      return element_handle_img_proxy(f[c]);
    }
    
    
    grid_type_def const& DefGrid() const { return f.TheGrid();}
    grid_type_img const& ImgGrid() const {
      REQUIRE((g_img != 0), "No image grid!\n",1);
      return *g_img;
    }
    
  private:
    void init_inverse() const {
      // must use a writable version of inverse
      GrAL::ref_ptr<inverse_type> w_inv = new_ref_ptr(new inverse_type(ImgGrid(),DefGrid(), *this));
      for(typename et_def::ElementIterator e(DefGrid()); ! e.IsDone(); ++e)
	(*w_inv)[(*this)(*e).handle()] = e.handle();
      inv = w_inv;
    }
  };

  template<class E_DEF, class E_IMG>
  void element_morphism<E_DEF, E_IMG>::init(typename element_morphism<E_DEF, E_IMG>::vertex_morphism_type const& v_corr)
  {
    bijective_mapping<vertex_set<element_img>, element_handle_img> vertex_set_to_element_img;
    for(typename et_img::ElementIterator e = GrAL::begin_x(ImgGrid()); e != GrAL::end_x(ImgGrid()); ++e) {
      vertex_set<element_img> vs_img(*e);
      vertex_set_to_element_img[vs_img] = e.handle();
    }
    typename et_def::ElementIterator e_end = GrAL::end_x(DefGrid());
    for(typename et_def::ElementIterator e = GrAL::begin_x(DefGrid()); e != e_end; ++e) {
      vertex_set<element_def> vs_def(*e);
      vertex_set<element_img> vs_img(vs_def.size());
      for(int v = 0; v < (int)vs_def.size(); ++v)
	vs_img[v] = v_corr(vs_def[v]);
      f[*e] = vertex_set_to_element_img(vs_img);
    }
  }



} // namespace GrAL 

#endif
