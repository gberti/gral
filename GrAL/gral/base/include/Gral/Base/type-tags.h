
#ifndef GRAL_BASE_GB_TYPE_TAGS_H
#define GRAL_BASE_GB_TYPE_TAGS_H

#include <boost/type_traits.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

//----------------------------------------------------------------
/*!  \brief workaround for missing explicit
     specialization of function template arguments.

  This empty frame is to be specialized for every element type.
  Its purpose is to create a unique tagging type for that element
  types in order to select appropriate functions by the type of
  the element.

  \b Examle:
  \code
   template<class Elt> class client {
   ...
   the_range = server.get_range(tp<Elt>);
   ...
   };

 class server {

  vertex_range get_range(tp<Vertex>);
  edge_range   get_range(tp<Edge>);
   // etc.
   // without tp<>, we would need something like this:
   // template<class E> get_range();   empty frame
   // vertex_range get_range<Vertex>();  specialization
   // edge_range   get_range<Edge>();    is this possible at all??
  };
  \endcode

 This approach allows in class client to write code independent
 of the type of Elt, be it Vertex, Edge or Cell.
*/

template<class T> 
class tp {};

/*! \defgroup elementtags Element tags
    \ingroup  traits
     Workaround for missing explicit
     specialization of function template arguments.
 */

/*! \ingroup elementtags
 */
enum grid_element_tag { vertex_tag, edge_tag, facet_tag, face_tag, cell_tag, invalid_tag };

/*! \brief Type tag for vertices 
    \ingroup elementtags
*/
class vertex_type_tag {};

/*! \ingroup elementtags
 */
class edge_type_tag   {};

/*! \ingroup elementtags
 */
class face_type_tag  {};

/*! \ingroup elementtags
 */
class facet_type_tag  {};

/*! \ingroup elementtags
 */
class cell_type_tag   {};

  template<int D>
  struct element_type_tag_d {};

/*! \brief Map dimensions to element tags.
   \ingroup elementtags
 */
template<int GD, int D, int CD>
struct int2element_tag { typedef element_type_tag_d<D> tag; typedef tag type; };

template<int GD, int CD>
struct int2element_tag<GD,0, CD> { typedef vertex_type_tag tag; typedef tag type; };

template<int GD, int CD>
struct int2element_tag<GD,1, CD> { typedef edge_type_tag tag; typedef tag type; };
template<int GD, int CD>
struct int2element_tag<GD,2, CD> { typedef face_type_tag tag; typedef tag type; };

template<int GD, int D>
struct int2element_tag<GD,D,1> { typedef facet_type_tag tag; typedef tag type; };

template<int GD, int D>
struct int2element_tag<GD,D,0> { typedef cell_type_tag tag; typedef tag type; };

template<>
struct int2element_tag<3,2,1> { typedef facet_type_tag tag; typedef tag type; };

template<>
struct int2element_tag<2,1,1> { typedef edge_type_tag tag; typedef tag type; };

template<>
struct int2element_tag<2,2,0> { typedef cell_type_tag tag; typedef tag type; };

template<>
struct int2element_tag<1,1,0> { typedef edge_type_tag tag; typedef tag type; };

template<>
struct int2element_tag<1,0,1> { typedef vertex_type_tag tag; typedef tag type; };

template<>
struct int2element_tag<0,0,0> { typedef vertex_type_tag tag; typedef tag type; };

template<class GT, int ED> 
struct int2etag : public int2element_tag<GT::dimension_tag::dim, ED, GT::dimension_tag::dim - ED> {};




  namespace tags {

    struct undefined {};

    //------ dummy types for each typedef in grid_types  -------

    struct Vertex {}; struct vertex_handle {}; struct VertexIterator {};
    struct Edge   {}; struct edge_handle   {}; struct EdgeIterator   {};
    struct Face   {}; struct face_handle   {}; struct FaceIterator   {};
    struct Facet  {}; struct facet_handle  {}; struct FacetIterator  {};
    struct Cell   {}; struct cell_handle   {}; struct CellIterator   {};

#define INC(E,A) struct E ## On ## A ## Iterator {}

    INC(Vertex, Vertex); INC(Edge, Vertex); INC(Face, Vertex); INC(Facet, Vertex); INC(Cell, Vertex);
    INC(Vertex, Edge  ); INC(Edge, Edge  ); INC(Face, Edge  ); INC(Facet, Edge  ); INC(Cell, Edge  );
    INC(Vertex, Face  ); INC(Edge, Face  ); INC(Face, Face  ); INC(Facet, Face  ); INC(Cell, Face  );
    INC(Vertex, Facet ); INC(Edge, Facet ); INC(Face, Facet ); INC(Facet, Facet ); INC(Cell, Facet );
    INC(Vertex, Cell  ); INC(Edge, Cell  ); INC(Face, Cell  ); INC(Facet, Cell  ); INC(Cell, Cell  );
    
#undef INC

    struct grid_type {};
    struct archetype_type {};
    struct archetype_handle {};
    struct archetype_iterator {};
    struct archgt {};
    
  } // namespace tags


  

/*! \brief Tags for differentiating between fixed and variable dimensions
    \ingroup traits
 */ 
namespace type_tags {
  struct fixed {};
  struct variable {};
}


/*! \brief Combinatorial element dimension

   \ingroup traits
 */
template<int N>
struct element_dim_tag 
{   
  typedef type_tags::fixed fixity_tag;
  enum {dim = N}; 
};

template<>
struct element_dim_tag<-1> 
{  
  typedef type_tags::variable fixity_tag;
  enum { dim = -1 };
};

/*! \brief Combinatorial element co-dimension

   \ingroup traits
 */
template<int N>
struct element_codim_tag 
{    
  typedef type_tags::fixed fixity_tag;
  enum {dim = N}; 
};

template<>
struct element_codim_tag<-1>
{    
  typedef type_tags::variable fixity_tag;
  enum {dim = -1}; 
};


/*! \brief Combinatorial grid dimension

   \ingroup traits
 
    The special value <tt> N == -1 </tt> is reserved 
    for grids with runtime-variable dimension.
 */
template<int N>
struct grid_dim_tag 
{ 
  typedef type_tags::fixed fixity_tag;
  enum {dim = N}; 

  template<int K> struct elem_dim   { typedef element_dim_tag  <K>   tag;};
  template<int K> struct elem_codim { typedef element_codim_tag<N-K> tag;};
};

template<>
struct grid_dim_tag<-1>
{ 
  typedef type_tags::variable fixity_tag;
  enum {dim = -1}; 
  template<int K> struct elem_dim   { typedef element_dim_tag  <-1> tag;};
  template<int K> struct elem_codim { typedef element_codim_tag<-1> tag;};

};

  /*! \brief Meta-function for returning the fixity type of grid dimension
      
      \returns either \c type_tags::fixed or \c type_tags::variable
   */
  template<class GT> struct fixity { typedef typename GT::dimension_tag::fixity_tag type;};

  template<class E> struct etag { typedef typename E::element_type_tag type;};

  /*! \brief Meta-function for returning the dimension of the grid or element
      
      Returns -1 if grid dimension is not compile-time constant. 
      \todo Make dim<Grid> possible (currently only dim<GT> or dim<GT,Element>).
  */
  template<class GT, class E = void> struct dim;
  template<class GT, class E>        struct codim;



  /*! \brief Meta-function returning the dimension of the element tag
   */
  template<class GT, class TAG>  struct etag2dim;

  /*! \brief Meta-function returning the co-dimension of the element tag
   */
  template<class GT, class TAG>  struct etag2codim;

  template<class GT, class E> struct dim   : boost::mpl::int_<etag2dim  <GT,typename etag<E>::type>::value> {};
  template<class GT, class E> struct codim : boost::mpl::int_<etag2codim<GT,typename etag<E>::type>::value> {};

  template<class GT> struct dim<GT,void> : boost::mpl::int_<GT::dimension_tag::dim> {};

  template<class GT> struct dimgt  : boost::mpl::int_<GT::dimension_tag::dim> {};
  

  template<class GT, class TAG, class FIX> struct etag2dim_aux {};
  template<class GT, class TAG> struct etag2dim_aux<GT,TAG, type_tags::fixed>    
  : boost::mpl::int_<dim<GT>::value - etag2codim<GT,TAG>::value> {};
  template<class GT, class TAG> struct etag2dim_aux<GT,TAG, type_tags::variable> : boost::mpl::int_<-1> {};

  template<class GT, class TAG, class FIX> struct etag2codim_aux {};
  template<class GT, class TAG> struct etag2codim_aux<GT,TAG, type_tags::fixed>    
  : boost::mpl::int_<dim<GT>::value - etag2dim<GT,TAG>::value> {};
  template<class GT, class TAG> struct etag2codim_aux<GT,TAG, type_tags::variable> : boost::mpl::int_<-1> {};

  template<class GT> struct etag2dim<GT,vertex_type_tag> : boost::mpl::int_<0> {};
  template<class GT> struct etag2dim<GT,edge_type_tag>   : boost::mpl::int_<1> {};
  template<class GT> struct etag2dim<GT,face_type_tag>   : boost::mpl::int_<2> {};
  template<class GT> struct etag2dim<GT,facet_type_tag> : etag2dim_aux<GT, facet_type_tag, typename fixity<GT>::type>  {};
  template<class GT> struct etag2dim<GT,cell_type_tag>  : etag2dim_aux<GT, cell_type_tag,  typename fixity<GT>::type>  {};

  template<class GT> struct etag2codim<GT,vertex_type_tag> : etag2codim_aux<GT, vertex_type_tag, typename fixity<GT>::type>  {};
  template<class GT> struct etag2codim<GT,edge_type_tag>   : etag2codim_aux<GT, edge_type_tag,   typename fixity<GT>::type>  {};
  template<class GT> struct etag2codim<GT,face_type_tag>   : etag2codim_aux<GT, face_type_tag,   typename fixity<GT>::type>  {};
  template<class GT> struct etag2codim<GT,facet_type_tag>  : boost::mpl::int_<1> {};
  template<class GT> struct etag2codim<GT,cell_type_tag>   : boost::mpl::int_<0> {};


/*! \defgroup type_categories Type categories 

    \ingroup traits     

    Can be used to branch according to conceptual type of argument.

    \note Experimental. We need perhaps constructs like
    <code> convertible_to<T::category, grid_element_category> </code>.
*/

struct grid_entity_category {};

struct grid_atom_category          : public virtual grid_entity_category {};
struct grid_global_entity_category : public virtual grid_entity_category {};


// grid atoms
/*!  \brief Element category
  \ingroup type_categories
 */
struct grid_element_category        : public virtual grid_atom_category {};
struct grid_iterator_category       : public virtual grid_atom_category {};
struct grid_element_handle_category : public virtual grid_atom_category {};

  /*
struct grid_vertex_handle_category : public virtual grid_element_handle_category {};
struct grid_edge_handle_category   : public virtual grid_element_handle_category {};
struct grid_face_handle_category   : public virtual grid_element_handle_category {};
struct grid_facet_handle_category  : public virtual grid_element_handle_category {};
struct grid_cell_handle_category   : public virtual grid_element_handle_category {};
  */

template<int D> 
struct grid_element_handle_category_d  : public virtual grid_element_handle_category  {};
template<int D> 
struct grid_element_handle_category_cd : public virtual grid_element_handle_category  {};


typedef grid_element_handle_category_d <0> grid_vertex_handle_category;
typedef grid_element_handle_category_d <1> grid_edge_handle_category;
typedef grid_element_handle_category_d <2> grid_face_handle_category;
typedef grid_element_handle_category_cd<1> grid_facet_handle_category;
typedef grid_element_handle_category_cd<0> grid_cell_handle_category;


template<int D>
struct grid_element_category_d  : public virtual grid_element_category  {};
template<int D>
struct grid_element_category_cd : public virtual grid_element_category  {};

typedef grid_element_category_d <0> grid_vertex_category;
typedef grid_element_category_d <1> grid_edge_category;
typedef grid_element_category_d <2> grid_face_category;
typedef grid_element_category_cd<1> grid_facet_category;
typedef grid_element_category_cd<0> grid_cell_category;



struct grid_sequence_iterator_category  : public virtual grid_iterator_category {};
struct grid_incidence_iterator_category : public virtual grid_iterator_category {};

  // dimension(s) can be deduced from value_type (+anchor_type)
template<int D>
struct grid_sequence_iterator_category_d    : public virtual grid_sequence_iterator_category {};
template<int D>
struct grid_sequence_iterator_category_cd  : public virtual grid_sequence_iterator_category {};

typedef grid_sequence_iterator_category_d <0> grid_vertex_iterator_category;
typedef grid_sequence_iterator_category_d <1> grid_edge_iterator_category;
typedef grid_sequence_iterator_category_d <2> grid_face_iterator_category;
typedef grid_sequence_iterator_category_cd<1> grid_facet_iterator_category;
typedef grid_sequence_iterator_category_cd<0> grid_cell_iterator_category;


template<int ED,  int AD>
struct grid_incidence_iterator_category_d    : public virtual grid_incidence_iterator_category {};
template<int ED,  int ACD>
struct grid_incidence_iterator_category_d_cd : public virtual grid_incidence_iterator_category {};
template<int ECD, int ACD>
struct grid_incidence_iterator_category_cd   : public virtual grid_incidence_iterator_category {};
template<int ECD, int AD>
struct grid_incidence_iterator_category_cd_d : public virtual grid_incidence_iterator_category {};

// could define the usual 5*4 + 2 typedefs for incidence iterators here ...


template<class ECAT>
struct get_sequence_iterator_category { typedef grid_sequence_iterator_category type;};
template<int D>
struct get_sequence_iterator_category<grid_element_category_d <D> > { typedef grid_sequence_iterator_category_d <D> type;};
template<int D>
struct get_sequence_iterator_category<grid_element_category_cd<D> > { typedef grid_sequence_iterator_category_cd<D> type;};


template<class ECAT>
struct get_element_handle_category { typedef grid_element_handle_category type;};
template<int D>
struct get_element_handle_category<grid_element_category_d <D> > { typedef grid_element_handle_category_d <D> type;};
template<int D>
struct get_element_handle_category<grid_element_category_cd<D> > { typedef grid_element_handle_category_cd<D> type;};

template<class ECAT, class ACAT>
struct get_incidence_iterator_category { typedef grid_incidence_iterator_category type;};
template<int DE, int DA>
struct get_incidence_iterator_category<grid_element_category_d<DE>,
                                       grid_element_category_d<DA> >   { typedef grid_incidence_iterator_category_d<DE,DA> type; }; 
template<int CDE, int CDA>
struct get_incidence_iterator_category<grid_element_category_cd<CDE>,
				       grid_element_category_cd<CDA> > { typedef grid_incidence_iterator_category_cd<CDE,CDA> type; }; 
template<int DE, int CDA>
struct get_incidence_iterator_category<grid_element_category_d<DE>,
				       grid_element_category_cd<CDA> > { typedef grid_incidence_iterator_category_d_cd<DE,CDA> type; }; 
template<int CDE, int DA>
struct get_incidence_iterator_category<grid_element_category_cd<CDE>,
				       grid_element_category_d<DA> >   { typedef grid_incidence_iterator_category_cd_d<CDE,DA> type; }; 
    								 


template<int D, int CD>
struct get_grid_element_category         { typedef grid_element_category_d <D>  type;};
template<int CD>
struct get_grid_element_category<-1, CD> { typedef grid_element_category_cd<CD> type;};
template<>
struct get_grid_element_category<-1, -1> { typedef grid_element_category        type;};



template<int D, int CD>
struct get_grid_element_handle_category_d         { typedef grid_element_handle_category_d <D>  type;};
template<int CD>
struct get_grid_element_handle_category_d<-1, CD> { typedef grid_element_handle_category_cd<CD> type;};
template<>
struct get_grid_element_handle_category_d<-1, -1> { typedef grid_element_handle_category        type;};




// grid global entities

/*!  \brief Range  category
  \ingroup type_categories
 */
struct grid_range_category     : public virtual grid_global_entity_category  {};
template<int D>
struct grid_range_category_d   : public virtual grid_range_category {};

struct grid_category           : public virtual grid_range_category {};
template<int D>
struct grid_category_d         : public virtual grid_category, public virtual grid_range_category_d<D> {};

// perhaps grid_view_category<base::grid_category> is better?
/*
   template<class Base>
   struct grid_view_category { typedef Base base_category; };
   template<class Base, int D>
   struct grid_view_category_d : public virtual grid_view_category<Base> {};

*/

struct grid_view_category      : public virtual grid_range_category {};
template<int D>
struct grid_view_category_d    : public virtual grid_view_category, public virtual grid_range_category_d<D> {};

struct cartesian_grid_view_category : public virtual grid_view_category {};

template<int D>
struct cartesian_grid_view_category_d 
  : public virtual cartesian_grid_view_category, 
    public virtual grid_view_category_d<D>
{};

struct cartesian_grid_category : public virtual grid_category  {};

template<int D>
struct cartesian_grid_category_d : public virtual cartesian_grid_category, public virtual grid_category_d<D> {};

struct triangulation_category : public virtual grid_category {};
template<int D>
struct triangulation_category_d : public virtual triangulation_category {};



struct grid_function_category         : public virtual grid_global_entity_category {};

struct total_grid_function_category   : public virtual grid_function_category {};

struct partial_grid_function_category : public virtual grid_function_category {};

template<class gf_category>
struct grid_function_view_category 
  : public virtual grid_function_category 
{
  // BOOST_STATIC_ASSERT((1== is_grid_function_category<gf_category>::value));  
  typedef gf_category base_category; 
};


struct grid_geometry_category : public virtual grid_global_entity_category {};

/* prototype category-based  template overloading:

   template<class X>
   class action : public cond<is_element<X>, action_for_element<X>,
                              is_grid   <X>, action_for_grid   <X>,
                              ...
                              default<X>,    action_default<X> >
  {};

*/


  template<class GRID> class grid_types;
  template<class T>    struct has_category;

  template<class T, int HAS_CAT> struct cat_aux {};

  template<class T> struct cat_aux<T,1> { typedef typename T::category type; };
  template<class T> struct cat_aux<T,0> { 
    typedef grid_types<typename T::grid_type> gt;
    typedef typename gt::template categories<T>::type type; 
  };

  template<class T>
  struct category  : public cat_aux<T, has_category<T>::value> {};


  template<class BASE, class DERIVED>
  //struct is_base_or_same { enum { value = boost::is_base_of<BASE,DERIVED>::value || boost::is_same<BASE,DERIVED>::value }; };
  // old version
  struct is_base_or_same { enum { value = boost::is_base_and_derived<BASE,DERIVED>::value || boost::is_same<BASE,DERIVED>::value }; };
  
  template<class CAT, class T>
  struct is_of_category { enum { value = is_base_or_same<CAT, typename category<T>::type>::value}; };
  
  template<class T> struct is_grid_range    : boost::mpl::bool_<is_of_category<grid_range_category,    T>::value> {};
  template<class T> struct is_grid_element  : boost::mpl::bool_<is_of_category<grid_element_category,  T>::value> {};
  template<class T> struct is_grid_iterator : boost::mpl::bool_<is_of_category<grid_iterator_category, T>::value> {};
  template<class T> struct is_grid_sequence_iterator  : boost::mpl::bool_<is_of_category<grid_sequence_iterator_category,  T>::value> {};
  template<class T> struct is_grid_incidence_iterator : boost::mpl::bool_<is_of_category<grid_incidence_iterator_category, T>::value> {};
  


  template<class T, int IS_SPEC> struct grid_types_of_aux {};
  template<class T> struct grid_types_of_aux<T,1> { typedef grid_types<T> type; };
  template<class T> struct grid_types_of_aux<T,0> { typedef grid_types<typename T::grid_type> type; };

  template<class T> struct grid_types_of : public grid_types_of_aux<T, grid_types<T>::is_specialized> {};

//  template<class CAT>
//  struct is_grid_range_category { enum { value = is_base_or_same<grid_range_category, CAT>::value};};


// mapping between element tags and categories.
// In the long run, element_type_tags could disappear.


  // direct translation  category->element_tag
  template<class GT, class CAT> struct  category2element_tag_direct { typedef void type; };

  template<class GT>  struct  category2element_tag_direct<GT,grid_vertex_category> { typedef vertex_type_tag type;};
  template<class GT>  struct  category2element_tag_direct<GT,grid_edge_category>   { typedef edge_type_tag type;};
  template<class GT>  struct  category2element_tag_direct<GT,grid_face_category>   { typedef face_type_tag type;};
  template<class GT>  struct  category2element_tag_direct<GT,grid_facet_category>  { typedef facet_type_tag type;};
  template<class GT>  struct  category2element_tag_direct<GT,grid_cell_category>   { typedef cell_type_tag type;};

  // dimension/codimension based translation category->element_tag
  template<class GT, class CAT, int D, int CD>  struct category2element_tag_dim { typedef CAT type; };
  template<class GT, class CAT, int CD>  struct category2element_tag_dim<GT,CAT,0,CD> { typedef vertex_type_tag type;};
  template<class GT, class CAT, int CD>  struct category2element_tag_dim<GT,CAT,1,CD> { typedef edge_type_tag   type;};
  template<class GT, class CAT, int CD>  struct category2element_tag_dim<GT,CAT,2,CD> { typedef face_type_tag   type;};
  template<class GT, class CAT, int  D>  struct category2element_tag_dim<GT,CAT,D,1>  { typedef facet_type_tag type;};
  template<class GT, class CAT, int  D>  struct category2element_tag_dim<GT,CAT,D,0>  { typedef cell_type_tag type;};

  template<class GT, class CAT>  struct category2element_tag_dim<GT,CAT,0,0>  { typedef vertex_type_tag type;};
  template<class GT, class CAT>  struct category2element_tag_dim<GT,CAT,0,1>  { typedef vertex_type_tag type;};
  template<class GT, class CAT>  struct category2element_tag_dim<GT,CAT,1,0>  { typedef cell_type_tag   type;};
  template<class GT, class CAT>  struct category2element_tag_dim<GT,CAT,2,0>  { typedef cell_type_tag   type;};
  template<class GT, class CAT>  struct category2element_tag_dim<GT,CAT,1,1>  { typedef edge_type_tag   type;};
  template<class GT, class CAT>  struct category2element_tag_dim<GT,CAT,2,1>  { typedef face_type_tag   type;};


  // get dimension/codimension from 
  template<class CAT, int Is0, int Is1, int Is2> struct edim_aux   { enum { value = -1 }; };
  template<class CAT>  struct edim_aux<CAT,1,0,0>   { enum { value = 0 }; };
  template<class CAT>  struct edim_aux<CAT,0,1,0>   { enum { value = 1 }; };
  template<class CAT>  struct edim_aux<CAT,0,0,1>   { enum { value = 2 }; };

  template<class CAT> struct edim : public edim_aux<CAT, 
						    is_base_or_same<grid_element_category_d<0>, CAT>::value,
						    is_base_or_same<grid_element_category_d<1>, CAT>::value,
						    is_base_or_same<grid_element_category_d<2>, CAT>::value> 
  {};
						   
 
  template<class CAT, int D, int IsD, int IsD1, int IsCD1, int IsCD0>  struct ecodim_aux { enum { value = -1 }; };
  template<class CAT, int D, int IsD  , int IsD1 > struct ecodim_aux<CAT,D,IsD,IsD1, 1,     0    > { enum { value = 1 }; };
  template<class CAT, int D, int IsD  , int IsD1 > struct ecodim_aux<CAT,D,IsD,IsD1, 0,     1    > { enum { value = 0 }; };
  template<class CAT, int D, int IsCD1, int IsCD0> struct ecodim_aux<CAT,D,0,  1,    IsCD1, IsCD0> { enum { value = 1 }; };
  template<class CAT, int D, int IsCD1, int IsCD0> struct ecodim_aux<CAT,D,1,  0,    IsCD1, IsCD0> { enum { value = 0 }; };
  
  template<int D, class CAT> struct ecodim : public ecodim_aux<CAT, 
							       D,
							       is_base_or_same<grid_element_category_d <D>,   CAT>::value,
							       is_base_or_same<grid_element_category_d <D-1>, CAT>::value,
							       is_base_or_same<grid_element_category_cd<1>,   CAT>::value,
							       is_base_or_same<grid_element_category_cd<0>,   CAT>::value>
  {};

  // branch according to whether CAT is s.t. like grid_vertex_category or  grid_element_category_d<0>
  template<class GT, class CAT, int IS_ELEMENT_D_CAT> struct  category2element_tag_aux {};

  template<class GT, class CAT>
  struct category2element_tag_aux<GT, CAT, 0>
    : public category2element_tag_direct<GT,CAT> {};

  template<class GT, class CAT> 
  struct category2element_tag_aux<GT, CAT, 1> 
    : public category2element_tag_dim<GT,CAT,edim<CAT>::value, ecodim<GT::dimension_tag::dim,CAT>::value> {};

  template<class GT, class ECAT> struct category2element_tag :
    public category2element_tag_dim<GT, ECAT, edim<ECAT>::value, ecodim<GT::dimension_tag::dim,ECAT>::value> {};



} // namespace GrAL 


#endif
