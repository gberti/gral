#ifndef GRAL_GB_BASE_GRID_FUNCTION_ADAPTER_H
#define GRAL_GB_BASE_GRID_FUNCTION_ADAPTER_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

template<class CellMap>
class facet_on_cell_grid_function
{
 private:
  CellMap * the_map;
 public:
  typedef typename  CellMap::grid_type grid_type;
  typedef typename  grid_types<grid_type>::FacetOnCellIterator 
    FacetOnCellIterator;
  typedef typename  CellMap::value_type         cell_value_type;
  typedef typename  cell_value_type::value_type value_type;

  facet_on_cell_grid_function(CellMap& cm) { the_map = &cm; }

  value_type const& operator()(FacetOnCellIterator const& fc) const
    { return (*the_map)(fc.TheCell())[fc.local_number()];}
  value_type      & operator[](FacetOnCellIterator const& fc)
    { return (*the_map)[fc.TheCell()][fc.local_number()];}
};

template<class CellMap>
inline
facet_on_cell_grid_function<CellMap>
  MakeFacetOnCellGridFunction(CellMap & cm)
  { return facet_on_cell_grid_function<CellMap>(cm) ;}


  /*! \brief Iterator for a grid function via the element iterator of the underlying grid
    
  \ingroup gridfunctions
  \todo The class \ref restr_gf_iterator_adapter solves a similar problem.
        Maybe these classes should be fused.
   */
  template<class G, class GF>
  class gf_const_iterator 
  {
    typedef gf_const_iterator<G,GF>      self;

    typedef grid_types<G>                gt;
    typedef typename GF::element_type    element_type;
    typedef typename GF::value_type      value_type;
    typedef element_traits<element_type> et;
    typedef typename sequence_iterator<gt, typename et::element_type_tag>::type element_iterator;

    element_iterator  it;
    ref_ptr<GF const> gf;
  public:
    gf_const_iterator(element_iterator i, GF const& f) : it(i), gf(f) {}

    self&      operator++()       { ++it; return *this;}
    value_type operator* () const { return (*gf)(*it);}
    
    bool operator==(self const& rhs) const { return   it == rhs.it;}
    bool operator!=(self const& rhs) const { return !(it == rhs.it);}
  };


} // namespace GrAL 

#endif
