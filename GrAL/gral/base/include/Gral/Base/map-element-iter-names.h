#ifndef GRAL_GB_MAP_ELEMENT_ITER_NAMES_H
#define GRAL_GB_MAP_ELEMENT_ITER_NAMES_H

// $LICENSE


/*! \brief Map generic names like 'ElementIterator' to specific ones like 'CellIterator'

    This templates is specialized for different element types.
    It maps names defined in a generic base class \c ER to element-type specific names.
    For example, if the element is a vertex type, it maps 
    - template parameter \c EIt to \c VertexIterator
    - \c FirstElement() to \c FirstVertex()
    - \c EndElement() to \c EndVertex()
    - \c size() to \c NumOfVertices()

    \ingroup elements

    \see Paper <A HREF="http://www.math.tu-cottbus.de/INSTITUT/lsnmwr/papers/tmpw00">  
         Generic Components for Grid Data Structures and Algorithms with C++ </A> 
    \see partial_grid_function<>, element_range_ref<>
    
    Usage example:
    \code
    template<class ELEM>
    class ElemRange_base {
    public:
     typedef ... ElemIter;
     ElemIter FirstElement();
     unsigned NumOfElements();
    };

    template<class ELEM>
    class ElemRange 
      : public ElemRange_base, 
        public map_element_iter_name<ElemIter, 
                                     ElemRange<ELEM>, 
                                     typename  element_traits<ELEM>::element_type_tag> 
    {
     // we now have VertexIterator etc. defined, if element_type_tag == vertex_type_tag,
     // and analoguously for edge_type_tag etc.
    };
    \endcode

 */
template<class EIt, class ER, class element_tag>
struct map_element_iter_name {};

// specialization: Derive ElementIterator EIt from ER
template<class ER, class element_tag>
struct map_element_iter_name_2 
  : public map_element_iter_name<typename ER::ElementIterator, ER, element_tag>  
{};


//! Specialization for vertex types
template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, vertex_type_tag>
{ 
private:
  ER const& asER() const { return static_cast<ER const&>(*this);}
public:
  typedef EIt VertexIterator;
  VertexIterator FirstVertex() const { return asER().FirstElement();}
  VertexIterator EndVertex  () const { return asER().EndElement(); }
  unsigned NumOfVertices()     const { return asER().size();}
};

//! Specialization for edge types
template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, edge_type_tag>
{ 
private:
  ER const& asER() const { return static_cast<ER const&>(*this);}
public:
  typedef EIt EdgeIterator;
  EdgeIterator FirstEdge() const { return asER().FirstElement(); }
  EdgeIterator EndEdge  () const { return asER().EndElement();   }
  unsigned NumOfEdges()    const { return asER().size(); }
};

//! Specialization for facet types
template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, facet_type_tag>
{ 
private:
  ER const& asER() const { return static_cast<ER const&>(*this);}
public:
  typedef EIt FacetIterator;
  FacetIterator FirstFacet() const {  return asER().FirstElement();}
  FacetIterator EndFacet  () const {  return asER().EndElement();}
  unsigned NumOfFacets()     const {  return asER().size();}
};

/*! \brief Specialization for 2D edge
 
    Choose edge AND facet in 2D 
    if types Edge and Facet coincide.
*/
template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, edge2d_type_tag> : 
  public map_element_iter_name<EIt, ER, edge_type_tag>,
  public map_element_iter_name<EIt, ER, facet_type_tag> {};

//! Specialization for cell types
template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, cell_type_tag>
{ 
private:
  ER const& asER() const { return static_cast<ER const&>(*this);}
public:
  typedef EIt CellIterator;
  CellIterator FirstCell() const { return asER().FirstElement(); }
  CellIterator EndCell  () const { return asER().EndElement(); }
  unsigned NumOfCells()    const { return asER().size();}
};


template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, cell2d_type_tag>
  : public map_element_iter_name<EIt, ER, cell_type_tag> {};

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, cell3d_type_tag>
  : public map_element_iter_name<EIt, ER, cell_type_tag> {};


#endif
