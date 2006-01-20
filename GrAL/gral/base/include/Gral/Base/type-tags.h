
#ifndef GRAL_BASE_GB_TYPE_TAGS_H
#define GRAL_BASE_GB_TYPE_TAGS_H

#include <boost/type_traits.hpp>

// $LICENSE

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


/*! \ingroup elementtags
 */
class edge2d_type_tag  : public edge_type_tag {};

/*! \ingroup elementtags
 */
class facet1d_type_tag : public facet_type_tag {};

/*! \ingroup elementtags
 */
class facet2d_type_tag : public facet_type_tag {};

/*! \ingroup elementtags
 */
class cell2d_type_tag  : public cell_type_tag {};

/*! \ingroup elementtags
 */
class cell3d_type_tag  : public cell_type_tag {};

/*! \brief Map dimensions to element tags.
   \ingroup elementtags
 */
template<int GD, int D, int CD>
struct int2element_tag {};

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
struct grid_element_category  : public virtual grid_atom_category {};
struct grid_iterator_category : public virtual grid_atom_category {};


struct  grid_element_category_d_base : public virtual grid_element_category {};
template<int D>
struct grid_element_category_d : public virtual grid_element_category_d_base {};

// are these useful?
struct grid_vertex_category : public virtual grid_element_category_d<0> {};
struct grid_edge_category   : public virtual grid_element_category_d<1> {};
struct grid_face_category   : public virtual grid_element_category_d<2> {};

struct grid_facet_category  : public virtual grid_element_category {};
struct grid_cell_category   : public virtual grid_element_category {};

template<int GD>
struct grid_facet_category_d : public virtual grid_element_category_d<GD-1>, public virtual grid_facet_category {};

template<int GD>
struct grid_cell_category_d  : public virtual grid_element_category_d<GD>,   public virtual grid_cell_category {};



struct grid_sequence_iterator_category  : public virtual grid_iterator_category {};
struct grid_incidence_iterator_category : public virtual grid_iterator_category {};

template<int D>
struct grid_sequence_iterator_category_d  : public virtual grid_sequence_iterator_category {};

template<int ED, int AD>
struct grid_incidence_iterator_category_d : public virtual grid_incidence_iterator_category {};

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




struct grid_function_category         : public virtual grid_global_entity_category {};

struct total_grid_function_category   : public virtual grid_function_category {};

struct partial_grid_function_category : public virtual grid_function_category {};



struct grid_geometry_category : public virtual grid_global_entity_category {};



template<class BASE, class DERIVED>
//struct is_base_or_same { enum { value = boost::is_base_of<BASE,DERIVED>::value || boost::is_same<BASE,DERIVED>::value }; };
// old version
struct is_base_or_same { enum { value = boost::is_base_and_derived<BASE,DERIVED>::value || boost::is_same<BASE,DERIVED>::value }; };

template<class CAT, class T>
struct is_of_category { enum { value = is_base_or_same<CAT, typename T::category>::value}; };


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
  template<class CAT, int Is0, int Is1, int Is2> struct dim_aux   { enum { value = -1 }; };
  template<class CAT>  struct dim_aux<CAT,1,0,0>   { enum { value = 0 }; };
  template<class CAT>  struct dim_aux<CAT,0,1,0>   { enum { value = 1 }; };
  template<class CAT>  struct dim_aux<CAT,0,0,1>   { enum { value = 2 }; };

  template<class CAT> struct dim : public dim_aux<CAT, 
						   is_base_or_same<grid_element_category_d<0>, CAT>::value,
						   is_base_or_same<grid_element_category_d<1>, CAT>::value,
						   is_base_or_same<grid_element_category_d<2>, CAT>::value> 
  {};
						   
 
  template<class CAT, int D, int IsD, int IsD1> struct codim_aux { enum { value = -1 }; };
  template<class CAT, int D> struct codim_aux<CAT,D,1,0> { enum { value = 0 }; };
  template<class CAT, int D> struct codim_aux<CAT,D,0,1> { enum { value = 1 }; };
  
  template<int D, class CAT> struct codim : public codim_aux<CAT, 
							      D,
							      is_base_or_same<grid_element_category_d<D>,   CAT>::value,
							      is_base_or_same<grid_element_category_d<D-1>, CAT>::value >
  {};

  // branch according to whether CAT is s.t. like grid_vertex_category or  grid_element_category_d<0>
  template<class GT, class CAT, int IS_ELEMENT_D_CAT> struct  category2element_tag_aux {};

  template<class GT, class CAT>
  struct category2element_tag_aux<GT, CAT, 0>
    : public category2element_tag_direct<GT,CAT> {};
  template<class GT, class CAT> 
  struct category2element_tag_aux<GT, CAT, 1> 
    : public category2element_tag_dim<GT,CAT,dim<CAT>::value, codim<GT::dimension_tag::dim,CAT>::value> {};

  template<class GT, class ECAT> struct category2element_tag :
    public category2element_tag_aux<GT, ECAT, is_base_or_same<grid_element_category_d_base, ECAT>::value> {};



} // namespace GrAL 


#endif
