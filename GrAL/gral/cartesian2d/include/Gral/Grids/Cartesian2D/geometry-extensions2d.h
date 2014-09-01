#ifndef GRAL_GB_CARTESIAN2D_GEOMETRY_EXTENSIONS_2D_H
#define GRAL_GB_CARTESIAN2D_GEOMETRY_EXTENSIONS_2D_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

namespace cartesian2d {
//--------------------- dimension dependend geometry parts --------------

template<class BASE, class GEOM, unsigned DIM>
class dimension_dependend_extensions2d 
  : public BASE // GEOM::geom_base_type 
{
  typedef  typename GEOM::geom_base_type base;
public:
  typedef typename base::index_type index_type;
  dimension_dependend_extensions2d() {}
  // dimension_dependend_extensions2d(base const& b) : base(b) {}
};

/*! \brief Dimension specific functionality for 2D
     \ingroup cartesian2dgeom
   
     \internal
 */
template<class BASE, class GEOM>
class dimension_dependend_extensions2d<BASE, GEOM, 2>
  : public BASE { // GEOM::geom_base_type {
public:
  //typedef typename GEOM::geom_base_type base;
  typedef BASE base;
  typedef typename base::Vertex Vertex;
  typedef typename base::Edge   Edge;
  typedef typename base::Facet  Facet;
  typedef typename base::Cell   Cell;

  typedef typename base::CellIterator        CellIterator;
  typedef typename base::CellOnCellIterator  CellOnCellIterator;
  typedef typename base::FacetOnCellIterator FacetOnCellIterator;

  typedef typename base::coord_type coord_type;
  typedef typename base::index_type index_type;

  using base::coord;
protected:
  GEOM      * _sub()       { return static_cast<GEOM      *>(this);}
  GEOM const* _sub() const { return static_cast<GEOM const*>(this);}

  base       & basic_geom()       { return *this;}
  base  const& basic_geom() const { return *this;}
public:
  dimension_dependend_extensions2d() {}

  // coord_type from base
  typedef algebraic_primitives<coord_type>  ap;

  /*! \brief Normed normal of facet \c nb
       
      \return
        Normal \f$n\f$  to facet \c nb
       -  \f$n\f$  is pointing away from \c nb.TheCell() 
       - \f$\|n\| = 1\f$ 
  */
  coord_type 
  outer_normal(FacetOnCellIterator const& nb) const 
  {
    return ap::normed_normal(coord(nb.V1()) - coord(nb.V2()));
  }
  
  /*! \brief Normal of facet \c nb
       
      \return
        Normal \f$n\f$  to facet \c nb
       -  \f$n\f$  is pointing away from \c nb.TheCell() 
       - The Euclidean norm  \f$\|n\|\f$ is equal to the volume (i.e. length) of the facet.
   */
  coord_type 
  outer_area_normal(FacetOnCellIterator const& nb) const 
  {
    return ap::normal_with_same_length(coord(nb.V1()) - coord(nb.V2()));
  }

  
  /*! \brief Normal of facet \c f
       
      \return
        Normal \f$n\f$  to facet \c f.
        The Euclidean norm  \f$\|n\|\f$ is equal to the volume (i.e. length) of the facet.
   */
  coord_type 
  area_normal(Facet const& f) const 
  {
    typename GEOM::segment_type S(f,basic_geom());
    return ap::normal_with_same_length(S.End()-S.Start());
  }
  
  /*! \brief Normed normal of facet \c f
       
      \return
        Normal \f$n\f$  to facet \c f with \f$\|n\|=1\f$.
   */
  coord_type 
  normal(Edge const& e) const 
  {
    typename GEOM::segment_type S(e, basic_geom());
    return ap::normed_normal(S.End()-S.Start());
  }
};

} // namespace cartesian2d

} // namespace GrAL 

#endif
