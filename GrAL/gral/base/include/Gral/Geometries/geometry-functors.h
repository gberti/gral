#ifndef GRAL_BASE_GB_GEOMETRY_FUNCTORS_H
#define GRAL_BASE_GB_GEOMETRY_FUNCTORS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
/*! \defgroup geometricfunctors Geometric Function Objects
    \ingroup  functors 

  Function objects that map grid elements to geometric items.
  These are models of STL unary functions.

  \b  Contents:
  - template<class Geom> class vertex2coord_map;
  - template<class Geom> class edge2length_map;

  <b> Central operations: </b>
  <geom_item> <tt> operator()(const Element&) const; </tt>
   - vertex2coord_map: Vertex -> coord_type 
   - edge2length_map:  Edge   -> double


  \see Module functors
  \see $GrAL grid-geometries
*/
//----------------------------------------------------------------


//----------------------------------------------------------------
//                 vertex2coord_map
/*! \brief Function object mapping vertices to coordinates 
   \ingroup geometricfunctors

   \templateparams
   - Geom: grid-geometry type, model of $GrAL VertexGridGeometry

   \see Module geometricfunctors
*/
//----------------------------------------------------------------

template<class Geom>
class vertex2coord_map {
public:
  typedef typename Geom::grid_type  grid_type;
  typedef grid_types<grid_type>     gt;
  typedef typename gt::Vertex       Vertex;
  typedef typename Geom::coord_type coord_type;

  typedef Vertex                    argument_type;
  typedef coord_type                result_type;
private:
  const Geom* geom;
public:
  vertex2coord_map(const Geom& geo) : geom(&geo) {}
 
  const result_type& operator()(const Vertex& v) const { return geom->coord(v);}
};

/*! \brief Creator function for vertex2coord_map
    \ingroup geometricfunctors
    \relates vertex2coord_map
 */
template<class Geom>
inline vertex2coord_map<Geom> vertex2coord(const Geom& g)
{ return vertex2coord_map<Geom>(g);}


//----------------------------------------------------------------
//                 edge2length_map
/*! \brief Function object mapping vertices to coordinates 
   \ingroup geometricfunctors

   \templateparams
   - Geom: grid-geometry type, model of $GrAL VertexGridGeometry

   \see Module geometricfunctors
*/
//----------------------------------------------------------------
template<class Geom>
class edge2length_map {
public:
  typedef typename Geom::grid_type grid_type;
  typedef grid_types<grid_type>    gt;
  typedef typename  gt::Edge       Edge;

  typedef Edge                     argument_type;
  typedef double                   result_type;
private:
  const Geom* geom;
public:
  edge2length_map(const Geom& geo) : geom(&geo) {}
 
  result_type operator()(const Edge& e) const { return geom->length(e);}
};

/*! \brief Creator function for edge2length_map
    \ingroup geometricfunctors
    \relates edge2length_map
 */
template<class Geom>
inline edge2length_map<Geom> edge2length(const Geom& g) 
{ return edge2length_map<Geom>(g);}

#endif
