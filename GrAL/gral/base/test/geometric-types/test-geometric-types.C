#include "Gral/Geometries/geometric-types-new.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Geometry/coords.h"
#include "Container/functions.h" // std extensions: identity

#include "Geometry/intersections.h"

#include <iostream>
#include <string>

typedef coordN<2>                                  coord_type;
typedef stdext::identity<coord_type>               mapping_type;
typedef cartesian2d::CartesianGrid2D               grid_type;
typedef cartesian2d::mapped_geometry<mapping_type> geom_type;
typedef grid_types<grid_type>                      gt;

template class polytope<geom_type, gt::Vertex>;
template class polytope<geom_type, gt::Edge>;
template class polytope<geom_type, gt::Cell>;

template<class POLY>
inline std::ostream& print_poly(std::ostream& out, POLY const& P, std::string sep = " -> ")
{
  typename POLY::VertexIterator v = P.FirstVertex();
  if(v != P.EndVertex())
    out << P(*v);
  ++v;
  for(; v != P.EndVertex(); ++v)
    out << sep << P(*v);
  return out;
}

int main() {
  using namespace std;

  {
    grid_type R(5,5);
    geom_type GeomR(R);
    
    
    typedef polytope<geom_type,gt::Vertex> poly0d;
    typedef polytope<geom_type,gt::Edge  > poly1d;
    typedef polytope<geom_type,gt::Cell  > poly2d;
    
    poly0d p0(GeomR, * R.FirstVertex());
    poly1d p1(GeomR, * R.FirstEdge());
    poly2d p2(GeomR, * R.FirstCell());
    
    cout << "Poly 0: "; print_poly(cout, p0); cout  << endl;
    cout << "Poly 1: "; print_poly(cout, p1); cout  << endl;
    cout << "Poly 2: "; print_poly(cout, p2); cout  << endl;
    
    cout << "directions 0: " << endl;
    polytope_directions<poly0d> dir0(p0);
    cout << "directions 1: " << endl;
    polytope_directions<poly1d> dir1(p1);
    cout << "directions 2: " << endl;
    polytope_directions<poly2d> dir2(p2);
    

    poly2d p2B(GeomR, R.cell(3,3));
    
    typedef intersection_by_separating_axes<poly0d, poly2d> i02;
    typedef intersection_by_separating_axes<poly1d, poly2d> i12;
    typedef intersection_by_separating_axes<poly2d, poly2d> i22;
    typedef intersection_by_separating_axes<poly0d, poly1d> i01;
    cout << "p0 and p2 do " << (i02::intersects(p0,p2) ? "" : "not") << " intersect" << endl;
    cout << "p1 and p2 do " << (i12::intersects(p1,p2) ? "" : "not") << " intersect" << endl;
    cout << "p2 and p2 do " << (i22::intersects(p2,p2) ? "" : "not") << " intersect" << endl;
    cout << "p0 and p1 do " << (i01::intersects(p0,p1) ? "" : "not") << " intersect" << endl;
    
    cout << "p0 and p2B do " << (i02::intersects(p0,p2B) ? "" : "not") << " intersect" << endl;
    cout << "p1 and p2B do " << (i12::intersects(p1,p2B) ? "" : "not") << " intersect" << endl;
    cout << "p2 and p2B do " << (i22::intersects(p2,p2B) ? "" : "not") << " intersect" << endl;
  }
  {
    typedef coordN<3> coord_type;
    typedef stdext::identity<coord_type>           mapping_type;
    typedef cartesian3d::CartesianGrid3D           grid_type;
    typedef cartesian3d::mapped_geometry<mapping_type> geom_type;
    typedef grid_types<grid_type>                      gt;
    
    grid_type R(5,5,5);
    geom_type GeomR(R);
 
    typedef polytope<geom_type,gt::Vertex> poly0d;
    typedef polytope<geom_type,gt::Edge  > poly1d;
    typedef polytope<geom_type,gt::Face  > poly2d;
    typedef polytope<geom_type,gt::Cell  > poly3d;
    
    poly0d p0(GeomR, * R.FirstVertex());
    poly1d p1(GeomR, * R.FirstEdge());
    poly2d p2(GeomR, * R.FirstFace());
    poly3d p3(GeomR, * R.FirstCell());
    
    cout << "Poly 0: "; print_poly(cout, p0); cout  << endl;
    cout << "Poly 1: "; print_poly(cout, p1); cout  << endl;
    cout << "Poly 2: "; print_poly(cout, p2); cout  << endl;
    cout << "Poly 3: "; print_poly(cout, p3); cout  << endl;
    
    poly3d p3B(GeomR, gt::Cell(R,gt::index_type(1,1,1)));
    //    cout << "p0 and p0 do " << (intersects(p0,p0) ? "" : "not") << " intersect" << endl;
    cout << "p0 and p1 do " << (intersects(p0,p1) ? "" : "not") << " intersect" << endl;
    cout << "p0 and p2 do " << (intersects(p0,p2) ? "" : "not") << " intersect" << endl;
    cout << "p0 and p3 do " << (intersects(p0,p3) ? "" : "not") << " intersect" << endl;

    cout << "p1 and p1 do " << (intersects(p1,p1) ? "" : "not") << " intersect" << endl;
    cout << "p1 and p2 do " << (intersects(p1,p2) ? "" : "not") << " intersect" << endl;
    cout << "p1 and p3 do " << (intersects(p1,p3) ? "" : "not") << " intersect" << endl;

    cout << "p2 and p2 do " << (intersects(p2,p2) ? "" : "not") << " intersect" << endl;
    cout << "p2 and p3 do " << (intersects(p2,p3) ? "" : "not") << " intersect" << endl;

    cout << "p3 and p3 do " << (intersects(p3,p3) ? "" : "not") << " intersect" << endl;
    
    cout << "p0 and p3B do " << (intersects(p0,p3B) ? "" : "not") << " intersect" << endl;
    cout << "p1 and p3B do " << (intersects(p1,p3B) ? "" : "not") << " intersect" << endl;
    cout << "p2 and p3B do " << (intersects(p2,p3B) ? "" : "not") << " intersect" << endl;
    cout << "p3 and p3B do " << (intersects(p3,p3B) ? "" : "not") << " intersect" << endl;

  }
}
