#ifndef NMWR_GB_GRAL_BASE_GRID_MORPHISM_H
#define NMWR_GB_GRAL_BASE_GRID_MORPHISM_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"


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
   \todo make \code morphism[Vertex] = Vertex \endcode possible 
         (currently assignment possible only for vertex_handle)
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

   Representation for a one-to-one mapping between vertices of 
   two grids. Despite the name, this correspondence does not need
   to come from a grid isomorphism.

   \ingroup gridmorphisms
   \see  \ref gridmorphisms module

   \todo Specialize if grids have consecutive handles
   \todo Add support for inverse mapping
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
    g_img = rhs.img;
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
    return CellImg(*g_img,c.handle());
  }

  cell_handle_img operator()(cell_handle_def c) const {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f(CellDef(f.TheGrid(),c));
  }

  cell_handle_img & operator[](cell_handle_def c)  {
    REQUIRE((g_img != 0), "No image grid!\n",1);
    return f[CellDef(f.TheGrid(),c)];
  }

  /* This would allow for operator[](Cell) also.
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
  */

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

#endif
