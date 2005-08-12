#ifndef GRAL_GB_TRANSFORMATION_TRANSFORMATION_PATTERN_H
#define GRAL_GB_TRANSFORMATION_TRANSFORMATION_PATTERN_H

// $LICENSE_NEC_2005

/*! \defgroup transformation_pattern Transformation patterns 

*/


namespace GrAL {

  /*! \brief  Concept of a patch-based mesh transformation pattern

  \ingroup transformation_pattern

     A transformation_pattern transforms an original <em> host patch <em>
     (in the simplest case a single cell) into a <em> transformed patch </em>,
     for instance, a refinement of a single cell.
  */
  template<...>
  class transformation_pattern 
  {
    /*! \brief grid type used to represent the local transformation / refinement
     */
    typedef pattern_grid_type;
    /*! \brief geometry type used to compute local coordinates of new vertices
     */
    typedef pattern_geom_type;
    /*! \brief Local coordinates of new vertices
        must be convertible   to   \c coord_type of pattern_grid_type::archetype_geometry
        must be constructible from \c coord_type of pattern_geom_type
    */
    typedef coord_type;

    //! \brief archetype of pattern_grid_type
    typedef archetype_type;
    //! \brief cell type of pattern_grid_type;
    typedef cell_type;
    /*! \brief Iterator over cell_type
     */
    typedef cell_iterator;

    //!  \brief grid to be refined or transformed
    typedef host_grid_type;
    //!  \brief cell type of \c host_grid_type
    typedef host_cell_type;
    //!  \brief vertex type of \c host_grid_type
    typedef host_vertex_type;
    
    /*!  \brief globally unique identifier for new vertices

      This is used to identify vertices introduced by several patterns simultaneously.
      In general, this has to use element handles etc. from HostGrid().
     */
    typedef new_vertex_global_identifier;
    typedef vertex_type;

   /*! \brief Iterator over new vertices.
    */
    typedef new_vertex_iterator;
    /*! \brief Iterator over old (host) vertices.

        \c value_type is convertible to \c host_vertex_type
     */
    typedef old_vertex_iterator;

    typedef removed_vertex_iterator;

    typedef vertex_on_cell_iterator;

    /*! \brief Set up data common to all patterns on \c g
     
        The data common to all patterns on g includes pattern grids
	and archetype lists.

    */
    static void init(host_grid_type const& g);

    new_vertex_global_identifier global_id(new_vertex_iterator);

    new_vertex_iterator FirstNewVertex() const;
    new_vertex_iterator EndNewVertex()   const;
    removed_vertex_iterator FirstRemovedVertex() const;
    removed_vertex_iterator EndRemovedVertex()   const;

    cell_iterator FirstCell() const;
    cell_iterator EndCell()   const;

    /*! \brief Number of cells in the transformed patch
     */
    unsigned NumOfNewCells()  const;

    /*! \brief Number of cells in the original patch
     */
    unsigned NumOfHostCells() const;

    /*! \brief Number of vertices of the transformed patch which are not in the host grid.
     */
    unsigned NumOfNewVertices()     const;

    /* ! total number of vertices of the transformed patch.
       
       We have 
       <tt> NumOfVertices() == NumOfHostVertices() + NumOfNewVertices() - NumOfRemovedVertices() </tt>
    */
    unsigned NumOfVertices()        const;
 
   /*! \brief Number of vertices in the host patch.
     */
    unsigned NumOfHostVertices()    const;
 

    /*! \brief Number of host vertices which are removed.

      For a true refinement pattern, this number is zero.
    */
    unsigned NumOfRemovedVertices() const;

    //! The grid this pattern lives on (refines or transforms)
    host_grid_type HostGrid() const;

    /*! \brief Host cell this new vertex lives in.
    
    For single-cell patterns, this is a trivial function.
    */
    host_cell_type   host_cell (new_vertex_iterator v) const;

    /*! \brief Local coordinates of \c v in the archetype geometry of <tt> host_cell(v) </tt>
      
        A geometry \c Geom for \c  HostGrid() can transform this into a global coordinate
	by using <tt> Geom.coord(host_cell(v), local_coord(v)) </tt>.
    */
    coord_type local_coord(new_vertex_iterator v) const;

    /*! \brief  Host vertex corresponding to old_v

        \pre  <tt> old(old_v) == true </tt>

     */
    host_vertex_type host_vertex(vertex_type old_v) const;

    /*! \brief True iff \c v corresponds to a vertex of the host grid
     */
    bool old(vertex_type v) const;
  };



} // namespace GrAL

#endif
