
#ifndef GRAL_BASE_GB_TYPE_TAGS_H
#define GRAL_BASE_GB_TYPE_TAGS_H



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
struct int2element_tag<GD,0, CD> { typedef vertex_type_tag tag;};

template<int GD, int CD>
struct int2element_tag<GD,1, CD> { typedef edge_type_tag tag;};
template<int GD, int CD>
struct int2element_tag<GD,2, CD> { typedef face_type_tag tag;};

template<int GD, int D>
struct int2element_tag<GD,D,1> { typedef facet_type_tag tag;};

template<int GD, int D>
struct int2element_tag<GD,D,0> { typedef cell_type_tag tag;};

template<>
struct int2element_tag<3,2,1> { typedef facet_type_tag tag;};

template<>
struct int2element_tag<2,1,1> { typedef edge_type_tag tag;};

template<>
struct int2element_tag<2,2,0> { typedef cell_type_tag tag;};

template<>
struct int2element_tag<1,1,0> { typedef edge_type_tag tag;};

template<>
struct int2element_tag<1,0,1> { typedef vertex_type_tag tag;};



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

struct grid_atom_category          : public grid_entity_category {};
struct grid_global_entity_category : public grid_entity_category {};


// grid atoms
/*!  \brief Element category
  \ingroup type_categories
 */
struct grid_element_category : public grid_atom_category {};
struct grid_iterator_category : public grid_atom_category {};

// are these useful?
struct grid_vertex_category : public grid_element_category {};
struct grid_edge_category   : public grid_element_category {};
struct grid_face_category   : public grid_element_category {};
struct grid_facet_category  : public grid_element_category {};
struct grid_cell_category   : public grid_element_category {};


struct grid_sequence_iterator_category  : public grid_iterator_category {};
struct grid_incidence_iterator_category : public grid_iterator_category {};


// grid global entities

/*!  \brief Range  category
  \ingroup type_categories
 */
struct grid_range_category  : public grid_global_entity_category  {};

struct grid_category : public grid_range_category {};

struct cartesian_grid_category : public grid_category {};

struct grid_view_category : public grid_range_category {};



struct grid_function_category : public grid_global_entity_category {};

struct total_grid_function_category : public grid_function_category {};

struct partial_grid_function_category : public grid_function_category {};



struct grid_geometry_category : public grid_global_entity_category {};

} // namespace GrAL 


#endif
