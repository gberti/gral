#ifndef GRAL_BASE_GB_TYPE_TAGS_H
#define GRAL_BASE_GB_TYPE_TAGS_H



// $LICENSE


//----------------------------------------------------------------
/*!
  \brief workaround for missing explicit
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
//----------------------------------------------------------------

template<class T> 
class tp {};

/*! \defgroup elementtags Element tags
    \ingroup  traits
 */

/*! \ingroup elementtags
 */
enum grid_element_tag { vertex_tag, edge_tag, facet_tag, cell_tag };

//! \ingroup elementtags
class vertex_type_tag {};
//! \ingroup elementtags
class edge_type_tag   {};
//! \ingroup elementtags
class facet_type_tag  {};
//! \ingroup elementtags
class cell_type_tag   {};

//! \ingroup elementtags
class edge2d_type_tag  : public edge_type_tag {};
//! \ingroup elementtags
class facet1d_type_tag : public facet_type_tag {};
//! \ingroup elementtags
class facet2d_type_tag : public facet_type_tag {};
//! \ingroup elementtags
class cell2d_type_tag  : public cell_type_tag {};
//! \ingroup elementtags
class cell3d_type_tag  : public cell_type_tag {};


#endif
