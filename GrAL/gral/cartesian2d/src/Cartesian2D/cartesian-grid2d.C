
// $LICENSE

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"

namespace GrAL {

namespace cartesian2d {

typedef RegGrid2D::index_type index_type;


// static data
RegGrid2D::SD RegGrid2D::sd;

// constructur for static data
RegGrid2D::SD::SD()
{
  // this works as long as we use a cyclic order for VertexOnCell iteration
  the_archetype[0] = archetype_type(polygon1d::polygon(4));
}

index_type  RegGrid2D::side_offset_[4] = { index_type(0,-1),  // S
					   index_type(1,0),   // E
					   index_type(0,1),   // N
					   index_type(-1,0) };// W 

/*  side_offset == outer normal:
              ^
              |
    +------------------+
    |                  |
    |                  |
    |                  |
  <-|                  |->
    |                  |
    |                  |
    +------------------+
              |
              +
*/


index_type  RegGrid2D::corner_offset_[4] = { index_type(0,0),  // SW
					     index_type(1,0),  // SE
					     index_type(1,1),  // NE
					     index_type(0,1) };// NW

/*  corner_offset

                         _
    ^ NW                 /| NE
    |                   / 
    +------------------+
    |                  |
    |                  |
    |                  |
    |                  |  
    |                  |
    | SW               |
    +------------------+  --> SE
              |
              +
*/







index_type RegGrid2D::direction_[4]  = { index_type(1,0), // S
					 index_type(0,1), // E
					 index_type(1,0), // N
					 index_type(0,1)};// W

/*  direction of sides:
   
  +----- > ----------+
  |                  |
  |                  |
  |                  |
  ^                  ^
  |                  |
  |                  |
  +----- > ----------+

*/



index_type  RegGrid2D::side_vertex_1_[4] = { index_type(0,0),  // S 
					     index_type(1,0),  // E 
					     index_type(0,1),  // N 
					     index_type(0,0) };// W 

index_type  RegGrid2D::side_vertex_2_[4] = { index_type(1,0),  // S 
					     index_type(1,1),  // E 
					     index_type(1,1),  // N 
					     index_type(0,1) };// W 

/*  coordinates of start (VX1) and end (VX2) vertices of sides
   
      VN1                VN2
  VW2  +------------------+  VE2
       |                  |
       |                  |
       |                  |
       |                  |
       |                  |
       |                  |
  VW1  +------------------+  VE1
      VS1                VS2
*/



/*
int  RegGrid2D::offset_dir[3][3] = 
                        { {Cell::SW,Cell::W,           Cell::NW},
                          {Cell::S, Cell::invalid_side,Cell::N },
			  {Cell::SE,Cell::E,           Cell::NE}};
			  */

::std::string RegGrid2D::side_name_[4] = { "S", "E", "N", "W"};
::std::string RegGrid2D::corner_name_[4] = { "SW", "SE", "NE", "NW"};

int RegGrid2D::invalid_side()   { return (int)Cell::invalid_side;}
int RegGrid2D::invalid_corner() { return (int)Cell::invalid_corner;}

int  RegGrid2D::get_side(const ::std::string& nm) 
{
  if(nm == "S" || nm == "s" || nm == "South" || nm == "south")
    return (int)(Cell::S);
  else if(nm == "E" || nm == "e" || nm == "East" || nm == "east")
    return (int)(Cell::E);
  else if(nm == "N" || nm == "n" || nm == "North" || nm == "north")
    return (int)(Cell::N);
  else if(nm == "W" || nm == "w" || nm == "West" || nm == "west")
    return (int)(Cell::W);
 else
   return (int)(Cell::invalid_side);
}


int  RegGrid2D::get_corner(const ::std::string& nm) 
{
  if(nm == "SE" || nm == "se" || nm == "South-East" || nm == "south-east" || nm == "SouthEast" || nm == "southeast")
    return (int)(Cell::SE);
  else if(nm == "NE" || nm == "ne" || nm == "North-East" || nm == "north-east" || nm == "NorthEast" || nm == "northeast")
    return (int)(Cell::NE);
  else if(nm == "NW" || nm == "nw" || nm == "North-West" || nm == "north-west" || nm == "NorthWest" || nm == "northwest")
    return (int)(Cell::NW);
  else if(nm == "SW" || nm == "sw" || nm == "South-West" || nm == "south-west" || nm == "SouthWest" || nm == "southwest")
    return (int)(Cell::SW);
  else
    return (int)(Cell::invalid_corner);
}

} // namespace cartesian2d

} // namespace GrAL 
