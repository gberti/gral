#ifndef NMWR_GB_GRAL_BASE_GRID_MORPHISM_H
#define NMWR_GB_GRAL_BASE_GRID_MORPHISM_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"

/*! \defgroup gridmorphisms Grid Morphisms
 
 */

/*! grid vertex isomorphism 
   \ingroup gridmorphisms

   \todo Specialize if grids have consecutive handles
   \todo Add support for inverse mapping
   \todo Make it independent of element being a vertex type
   (using map_element_iter_name<> like in partial_grid_function)
 */
template<class G_DEF, class G_IMG>
class vertex_morphism
{
  typedef G_DEF                         grid_type_def;
  typedef G_IMG                         grid_type_img;
  typedef grid_types<G_IMG>             gtimg;
  typedef grid_types<G_DEF>             gtdef;
  typedef typename gtimg::Vertex        VertexImg;
  typedef typename gtdef::Vertex        VertexDef;
  typedef typename gtimg::vertex_handle vertex_handle_img;
  typedef typename gtdef::vertex_handle vertex_handle_def;

  grid_function<VertexDef,vertex_handle_img> f; // contains ref to g_def
  grid_type_img const*                       g_img;
  
public:
  typedef VertexImg result_type;
  typedef VertexDef argument_type;

  vertex_morphism() : g_img(0) {}
  vertex_morphism(G_DEF const& g_def, G_IMG const& gg_img)
    : f(g_def), g_img(&gg_img) {}

  VertexImg operator()(VertexDef const& v) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return g_img->vertex(f(v));
  }

  vertex_handle_img operator()(vertex_handle_def v) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f(f.TheGrid().vertex(v));
  }

  vertex_handle_img & operator[](vertex_handle_def v)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f[f.TheGrid().vertex(v)];
  }

  grid_type_def const& DefGrid() const { return f.TheGrid();}
  grid_type_img const& ImgGrid() const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return *g_img;
  }
};


/*! grid cell isomorphism 
   \ingroup gridmorphisms

   \todo Specialize if grids have consecutive handles
   \todo Add support for inverse mapping
   \todo construct from vertex morphism
   \see vertex_morphism
 */


template<class G_DEF, class G_IMG>
class cell_morphism
{
  typedef G_DEF grid_type_def;
  typedef G_IMG grid_type_img;
  typedef grid_types<G_IMG> gtimg;
  typedef grid_types<G_DEF> gtdef;
  typedef typename gtimg::Cell CellImg;
  typedef typename gtdef::Cell CellDef;
  typedef typename gtimg::cell_handle cell_handle_img;
  typedef typename gtdef::cell_handle cell_handle_def;

  grid_function<CellDef,cell_handle_img> f;
  grid_type_img const* g_img;
  
public:
  typedef CellImg result_type;
  typedef CellDef argument_type;

  cell_morphism() : g_img(0) {}
  cell_morphism(G_DEF const& g_def, G_IMG const& gg_img)
    : f(g_def), g_img(&gg_img) {}

  CellImg operator()(CellDef const& v) const { 
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return g_img->cell(f(v));
  }

  cell_handle_img operator()(cell_handle_def v) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f(f.TheGrid().cell(v));
  }

  cell_handle_img & operator[](cell_handle_def v)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f[f.TheGrid().cell(v)];
  }

  /*
  struct cell_handle_img_proxy {
    cell_handle_img & h;
    cell_handle_img_proxy(cell_handle_img& hh) : h(hh) {}
    void operator=(CellImg const& c) { h = c.handle();}
  };

  cell_handle_img_proxy operator[](CellDef const& c) {
        REQUIRE((g_img != 0), "No image grid!\n",1);
        return cell_handle_img_proxy(f[c]);
  }
  */

  grid_type_def const& DefGrid() const { return f.TheGrid();}
  grid_type_img const& ImgGrid() const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return *g_img;
  }
};

#endif
