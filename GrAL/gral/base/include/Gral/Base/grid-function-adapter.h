#ifndef GRAL_GB_BASE_GRID_FUNCTION_ADAPTER_H
#define GRAL_GB_BASE_GRID_FUNCTION_ADAPTER_H


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

#endif
