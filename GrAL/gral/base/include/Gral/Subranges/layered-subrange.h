#ifndef NMWR_GB_LAYERED_SUBRANGE_H
#define NMWR_GB_LAYERED_SUBRANGE_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Grids/enumerated-subrange.h"

//----------------------------------------------------------------
// template<class Grid> class layered_vertex_range;
// template<class Grid> class layered_cell_range;
//
// A layered range is a range which additionally defines layers,
// i.e. consecutive subranges. Therefore it is possible to iterate
// efficiently over certain subranges, namely, unions of adjacent
// layers. This is useful e. g. for overlap ranges or the like.
//
//----------------------------------------------------------------



//----------------------------------------------------------------
//                 layered vertex range 
//----------------------------------------------------------------

template<class Grid>
class layered_vertex_range 
  : public enumerated_vertex_range<Grid>
{
public:
  typedef enumerated_vertex_range<Grid>        base;
  typedef typename base::range_type_ref        range_type_ref;

private:
  //---   DATA  ----
  vector<int> layers;

public:
  layered_vertex_range() {}
  layered_vertex_range(const grid_type& g) : base(g) {}

  void append_layer() { layers.push_back(TheContainer().size());}

  //---------------------- iteration  -------------------------------

  // layered range functionality
  unsigned NumOfLayers() const { return layers.size();}

  range_type_ref  Layer(unsigned ly) const 
    { 
      REQUIRE(((0 < ly) && (ly <= layers.size())),
	      "Layer " << ly << "out of range [1," << layers.size() << "] !\n",1);
      return range_type_ref(begin_l(ly),end_l(ly),TheContainer(),TheGrid());
    }

  range_type_ref Layers(unsigned l1, unsigned l2)
    {
      REQUIRE(((0 < l1) && (l2 <= layers.size())),
	      "Layers " << l1 << "," << l2 << "out of range [1," << layers.size() << "] !\n",1);
      REQUIRE(( l1 <= l2), "layers " << l1 << "," << l2 << " not ordered!\n",1);
   
      return (range_type_ref(begin_l(l1),end_l(l2),TheContainer(), TheGrid()));
    }

  range_type_ref AllLayers() const { return range();}
  range_type_ref LastLayer() const { return Layer(NumOfLayers());}

  
private:
  
  int begin_l(unsigned i) const { return layers[i-1];}
  int end_l  (unsigned i) const 
    { return ( i < layers.size() ? layers[i] : TheContainer().size()); }
};


//----------------------------------------------------------------
//                   layered cell range
//----------------------------------------------------------------


template<class Grid>
class layered_cell_range 
: public enumerated_cell_range<Grid>
{
public:
  typedef enumerated_cell_range<Grid>          base;
  typedef typename base::range_type_ref        range_type_ref;

private:
  //---   DATA  ----
  vector<int> layers;

public:
  layered_cell_range() {}
  layered_cell_range(const grid_type& g) : base(g) {}

  void append_layer() { layers.push_back(TheContainer().size());}

  //---------------------- iteration  -------------------------------

  // layered range functionality
  unsigned NumOfLayers() const { return layers.size();}
 
  range_type_ref  Layer(unsigned ly) const 
    { 
      REQUIRE(((0 < ly) && (ly <= layers.size())),
	      "Layer " << ly << "out of range [1," << layers.size() << "] !\n",1);
      return range_type_ref(begin_l(ly),end_l(ly),TheContainer(),TheGrid());
    }

  range_type_ref Layers(unsigned l1, unsigned l2)
    {
      REQUIRE(((0 < l1) && (l2 <= layers.size())),
	      "Layers " << l1 << "," << l2 << "out of range [1," << layers.size() << "] !\n",1);
      REQUIRE(( l1 <= l2), "layers " << l1 << "," << l2 << " not ordered!\n",1);
   
       return (range_type_ref(begin_l(l1),end_l(l2),TheContainer(), TheGrid()));
    }
  range_type_ref AllLayers() const { return range();}
  range_type_ref LastLayer() const { return Layer(NumOfLayers());}

private:
  int begin_l(unsigned i) const { return layers[i-1];}
  int end_l  (unsigned i) const 
    { return ( i < layers.size() ? layers[i] : TheContainer().size()); }

};



#endif
