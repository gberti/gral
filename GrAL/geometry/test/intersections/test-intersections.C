#include "Geometry/intersections.h"
#include "Geometry/coords.h"

#include <iostream>

typedef coordN<2> ct2;
typedef coordN<3> ct3;

template class segment<ct2>;
template class line_projector<ct2>;
template class interval<double>;
//template class intersection_by_separating_axes<segment<ct2>, segment<ct2> >;


template<class SCALAR>
inline std::ostream&  operator<<(std::ostream& out, interval<SCALAR> I)
{ return (out << I.l() << ' ' << I.r());}

template<class P>
inline std::ostream&  operator<<(std::ostream& out, space_point<P> S)
{ return (out << S[0]);}

template<class P>
inline std::ostream&  operator<<(std::ostream& out, segment<P> S)
{ return (out << S[0] << "," << S[1]);}

template<class P>
inline std::ostream&  operator<<(std::ostream& out, triangle<P> T)
{ return (out << T[0] << "," << T[1] << "," << T[2]);}

template<class P>
inline std::ostream&  operator<<(std::ostream& out, tetrahedron<P> T)
{ return (out << T[0] << "," << T[1] << "," << T[2] << "," << T[3]);}


int main() {

  using namespace std;

  {
    cout << "2D" << endl;

    typedef coordN<2> ct;
    typedef segment<ct>  segment_type;
    typedef triangle<ct> triangle_type;
    typedef line_projector<ct> projector_type; 
    typedef intersection_by_separating_axes<segment_type,  segment_type>  is_segment;
    typedef intersection_by_separating_axes<segment_type,  triangle_type> is_seg_tri;
    typedef intersection_by_separating_axes<triangle_type, triangle_type> is_triangle;

    vector<segment_type> S;
    S.push_back(segment_type(ct( 0,0), ct(0,2)));
    S.push_back(segment_type(ct( 1,0), ct(1,2)));
    S.push_back(segment_type(ct( 0,3), ct(0,4)));
    S.push_back(segment_type(ct(-1,1), ct(1,1)));
    S.push_back(segment_type(ct(-1,3), ct(1,3)));
    S.push_back(segment_type(ct( 0,0), ct(1,2)));
  
    vector<triangle_type> T;
    T.push_back(triangle_type(ct(0,0), ct(1,0), ct(0,1)));
    T.push_back(triangle_type(ct(1,1), ct(1,0), ct(0,1)));
    T.push_back(triangle_type(ct(0.6,0.6), ct(0.6,0.7), ct(0.7,0.6))); // \subset T[1]
    T.push_back(triangle_type(ct(-2,-2), ct(-2,0.5), ct(0.5,-2)));	    

    vector<projector_type> P;
    P.push_back(projector_type(ct(1,0)));
    P.push_back(projector_type(ct(0,1)));
    P.push_back(projector_type(ct(2,0)));
    P.push_back(projector_type(ct(0,2)));

    cout << "Segment/segment" << endl;

    for(unsigned s = 0; s < S.size(); ++s)
      for(unsigned p = 0; p < P.size(); ++p)
	cout << P[p](S[s]) << endl;


    for(unsigned s1 = 0; s1 < S.size(); ++s1)
      for(unsigned s2 = s1; s2 < S.size(); ++s2)
	cout << "Segments " 
	     << "[" << S[s1] << "]" << "  "
	     << "[" << S[s2] << "]" << "  "
	     << " do "
	     << (is_segment::intersects(S[s1], S[s2]) ? "" : "not ")
	     << "intersect." << endl; 

    cout << "Triangle/triangle" << endl;
    for(unsigned t = 0; t < T.size(); ++t)
      for(unsigned p = 0; p < P.size(); ++p)
	cout << P[p](T[t]) << endl;


    for(unsigned s1 = 0; s1 < T.size(); ++s1)
      for(unsigned s2 = s1; s2 < T.size(); ++s2)
	cout << "Triangles " 
	     << "[" << T[s1] << "]" << "  "
	     << "[" << T[s2] << "]" << "  "
	     << " do "
	     << (is_triangle::intersects(T[s1], T[s2]) ? "" : "not ")
	     << "intersect." << endl; 

    for(unsigned s = 0; s < S.size(); ++s)
      for(unsigned t = 0; t < T.size(); ++t)
	cout << "Segment/Triangle " 
	     << "[" << S[s] << "]" << "  "
	     << "[" << T[t] << "]" << "  "
	     << " do "
	     << (is_seg_tri::intersects(S[s], T[t]) ? "" : "not ")
	     << "intersect." << endl; 

  }  




  {
    cout << "3D" << endl;
    typedef coordN<3>    ct;
    typedef segment<ct>  segment_type;
    typedef triangle<ct> triangle_type;
    typedef tetrahedron<ct> tetra_type;
    typedef line_projector<ct> projector_type; 
    typedef intersection_by_separating_axes<segment_type,  segment_type>  is_segment;
    typedef intersection_by_separating_axes<segment_type,  triangle_type> is_seg_tri;
    typedef intersection_by_separating_axes<triangle_type, triangle_type> is_triangle;
    typedef intersection_by_separating_axes<tetra_type,    tetra_type   > is_tetra;
    typedef intersection_by_separating_axes<space_point<ct>, tetra_type   > is_point_tet;

    vector<space_point<ct> > C;
    C.push_back(space_point<ct>(ct(0.0,0.0,0.0)));
    C.push_back(space_point<ct>(ct(0.5,0.5,0.5)));
    C.push_back(space_point<ct>(ct(1.0,1.0,1.0)));

    vector<segment_type> S;
    S.push_back(segment_type(ct( 0,0, 0), ct(0,2,0)));
    S.push_back(segment_type(ct( 0,0, 0), ct(2,0,0)));
    S.push_back(segment_type(ct(0.1,0.1,0.1), ct(1,1,1)));
    S.push_back(segment_type(ct(-0.1,-0.1,-0.1), ct(1,1,1)));
    S.push_back(segment_type(ct(-1,-1,-1), ct(1,1,1)));
  
    vector<triangle_type> T;
    T.push_back(triangle_type(ct(0,0,0), ct(1,0,0), ct(0,1,0)));
    T.push_back(triangle_type(ct(0,0,0), ct(1,0,0), ct(0,0,1)));
    T.push_back(triangle_type(ct(0,0,0), ct(0,1,0), ct(0,0,1)));
    T.push_back(triangle_type(ct(1,0,0), ct(0,1,0), ct(0,0,1)));

    vector<tetra_type>   Tets;
    Tets.push_back(tetra_type(ct(0,0,0), ct(1,0,0), ct(0,1,0), ct(0,0,1)));
    Tets.push_back(tetra_type(ct(1,1,1), ct(1,0,0), ct(0,1,0), ct(0,0,1)));
    Tets.push_back(tetra_type(ct(0,0,0), ct(-1,0,0), ct(0,-1,0), ct(0,0,-1)));
    Tets.push_back(tetra_type(ct(-1,-1,-1), ct(-1,0,0), ct(0,-1,0), ct(0,0,-1)));

    vector<projector_type> P;
    P.push_back(projector_type(ct(1,0,0)));
    P.push_back(projector_type(ct(0,1,0)));
    P.push_back(projector_type(ct(0,0,1)));
    P.push_back(projector_type(ct(1,1,1)));

    cout << "Segment projections" << endl;

    for(unsigned s = 0; s < S.size(); ++s) {
      cout << "Segment " << S[s] << endl;
      for(unsigned p = 0; p < P.size(); ++p) {
	cout << "dir " << P[p].dir() << ": "
	     << P[p](S[s]) << endl;
      }
    }


    cout << "Segment/segment" << endl;

    for(unsigned s1 = 0; s1 < S.size(); ++s1)
      for(unsigned s2 = s1; s2 < S.size(); ++s2)
	cout << "Segments " 
	     << "[" << S[s1] << "]" << "  "
	     << "[" << S[s2] << "]" << "  "
	     << " do "
	     << (is_segment::intersects(S[s1], S[s2]) ? "" : "not ")
	     << "intersect." << endl; 


    cout << "Triangle projections" << endl;
    for(unsigned t = 0; t < T.size(); ++t) {
      cout << "Triangle " << T[t] << endl;
      for(unsigned p = 0; p < P.size(); ++p)
	cout << "dir " << P[p].dir() << ": "
	     <<  P[p](T[t]) << endl;
    }

    cout << "Triangle/triangle" << endl;

    for(unsigned s1 = 0; s1 < T.size(); ++s1)
      for(unsigned s2 = s1; s2 < T.size(); ++s2)
	cout << "Triangles " 
	     << "[" << T[s1] << "]" << "  "
	     << "[" << T[s2] << "]" << "  "
	     << " do "
	     << (is_triangle::intersects(T[s1], T[s2]) ? "" : "not ")
	     << "intersect." << endl; 
 
   cout << "Segment/triangle" << endl;

    for(unsigned s = 0; s < S.size(); ++s)
      for(unsigned t = 0; t < T.size(); ++t)
	cout << "Segment/Triangle " 
	     << "[" << S[s] << "]" << "  "
	     << "[" << T[t] << "]" << "  "
	     << " do "
	     << (is_seg_tri::intersects(S[s], T[t]) ? "" : "not ")
	     << "intersect." << endl; 

    cout << "Tet/Tet" << endl; 
    for(unsigned s1 = 0; s1 < Tets.size(); ++s1)
      for(unsigned s2 = s1; s2 < Tets.size(); ++s2)
	cout << "Tets " 
	     << "[" << Tets[s1] << "]" << "  "
	     << "[" << Tets[s2] << "]" << "  "
	     << " do "
	     << (is_tetra::intersects(Tets[s1], Tets[s2]) ? "" : "not ")
	     << "intersect." << endl; 
  
  cout << "Point/Tet" << endl; 
    for(unsigned s1 = 0; s1 < C.size(); ++s1)
      for(unsigned s2 = 0; s2 < Tets.size(); ++s2)
	cout << "Point " 
	     <<  C[s1]  << "  "
	     << "& Tet " << " [" << Tets[s2] << "]" << "  "
	     << " do "
	     << (is_point_tet::intersects(C[s1], Tets[s2]) ? "" : "not ")
	     << "intersect." << endl; 
  }  
}
