#ifndef GRAL_BASE_GB_LAYERED_SUBRANGE_H
#define GRAL_BASE_GB_LAYERED_SUBRANGE_H


// $LICENSE

#include <vector>
#include "Gral/Subranges/enumerated-subrange.h"

//----------------------------------------------------------------
/*! \defgroup layeredsubrange Layered Grid Subrange
    \ingroup subranges

   Ranges which additionally define layers,
   i.e. consecutive subranges. 

  With layered subranges, it is possible to iterate
  efficiently over certain subranges, namely, unions of adjacent
  layers. This is useful e. g. for overlap ranges or the like.
*/
//----------------------------------------------------------------



//----------------------------------------------------------------
/*! \brief  Layered vertex range 
   \ingroup layeredsubrange
   \see Module \ref layeredsubrange

   A layered vertex range gives efficient access to consecutive
   layers of vertices.

 */
//----------------------------------------------------------------

template<class Grid>
class layered_vertex_range 
  : public enumerated_vertex_range<Grid>
{
public:
  typedef enumerated_vertex_range<Grid>        base;
  typedef typename base::range_type_ref        range_type_ref;
  typedef typename base::grid_type             grid_type;
private:
  //---   DATA  ----
  std::vector<int> layers;

  using base::TheContainer;
  using base::TheGrid;
public:
  layered_vertex_range() {}
  layered_vertex_range(const grid_type& g) : base(g) {}

  void append_layer() { layers.push_back(TheContainer().size());}
  //FIXME: should either make sure that last layer is empty or remove cells!
  void remove_layer() { layers.pop_back();} 

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

  range_type_ref AllLayers() const { return base::range();}
  range_type_ref LastLayer() const { return Layer(NumOfLayers());}
  
  
private:
  
  int begin_l(unsigned i) const { return layers[i-1];}
  int end_l  (unsigned i) const 
    { return ( i < layers.size() ? layers[i] : TheContainer().size()); }
};



//----------------------------------------------------------------
/*! \brief  Layered cell range 
   \ingroup layeredsubrange
   \see Module \ref layeredsubrange

   A layered cell range gives efficient access to consecutive
   layers of cells.

 */
//----------------------------------------------------------------

template<class Grid>
class layered_cell_range 
: public enumerated_cell_range<Grid>
{
public:
  typedef enumerated_cell_range<Grid>          base;
  typedef typename base::range_type_ref        range_type_ref;
  typedef typename base::grid_type             grid_type;
private:
  //---   DATA  ----
  std::vector<int> layers;

  using base::TheContainer;
  using base::TheGrid;
public:
  layered_cell_range() {}
  layered_cell_range(const grid_type& g) : base(g) {}

  void append_layer() { layers.push_back(TheContainer().size());}
  //FIXME: should either make sure that last layer is empty or remove cells!
  void remove_layer() { layers.pop_back();} 

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
  range_type_ref AllLayers() const { return base::range();}
  range_type_ref LastLayer() const { return Layer(NumOfLayers());}

private:
  int begin_l(unsigned i) const { return layers[i-1];}
  int end_l  (unsigned i) const 
    { return ( i < layers.size() ? layers[i] : TheContainer().size()); }

};



#endif
