#ifndef GR_GRUMMP_ADAPTER_INL_H
#define GR_GRUMMP_ADAPTER_INL_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

//--------------------------------------------------------------
//-- BEGIN Mesh2DAdapter 

inline
Mesh2DAdapter::Mesh2DAdapter() : _a(0), owned(false) {
  // empty
}

inline 
Mesh2DAdapter::Mesh2DAdapter(Mesh2DAdapter::Adaptee *a) 
                                         : _a(a), owned(false) {
  // empty
}

inline 
Mesh2DAdapter::~Mesh2DAdapter() {
  clear();
  // empty
}

inline void
Mesh2DAdapter::clear() {
  if (owned) {
    delete _a;
  } 
  _a = NULL;
  owned = false;
}

inline void
Mesh2DAdapter::init(Mesh2DAdapter::Adaptee *a) {
  clear();
  _a = a;
}

inline void
Mesh2DAdapter::steal(Mesh2DAdapter::Adaptee *a) {
  clear();
  _a = a;
  owned = true;
}

inline bool 
Mesh2DAdapter::IsOwner() const {
  return owned;
}

inline unsigned 
Mesh2DAdapter::dimension() const { 
  return dim;
}

inline int 
Mesh2DAdapter::NumOfVertices() const  { 
  REQUIRE(_a != NULL, "not bound", 1); 
  return _a->iNumVerts();
}

inline int 
Mesh2DAdapter::NumOfCells() const {
  REQUIRE(_a != NULL, "not bound", 1);
  return _a->iNumCells();
}  

inline bool 
Mesh2DAdapter::IsBound() const {
  return _a != NULL;
}

inline const Mesh2DAdapter::Adaptee* 
Mesh2DAdapter::TheAdaptee() const {
  return _a;
}

inline bool 
Mesh2DAdapter::valid(Mesh2DAdapter::vertex_handle v) const {
  return (0 <= v  && v < NumOfVertices()); 
}
inline bool 
Mesh2DAdapter::valid(Mesh2DAdapter::cell_handle c) const {
  return  (0 <= c && c < NumOfCells());
}

inline Mesh2DAdapter::CellIterator
Mesh2DAdapter::FirstCell() const { 
  return CellIterator(*this);
}

inline Mesh2DAdapter::VertexIterator
Mesh2DAdapter::FirstVertex() const { 
  return VertexIterator(*this);
}

inline unsigned 
Mesh2DAdapter::space_dimension() const { 
  return 2;
}

inline Mesh2DAdapter::grid_type const& 
Mesh2DAdapter::TheGrid() const {
  return *this;
}

inline Mesh2DAdapter::coord_type  
Mesh2DAdapter::coord(Vertex const& v) const { 
    const double *cs = _a->pVVert(v.handle())->adCoords();
    return coord_type(*cs, *(cs+1));
}    

inline std::ostream&
operator<<(std::ostream& out, Mesh2DAdapter::coord_type const& p)
{ 
  typedef Mesh2DAdapter::pt pt;
  return (out << pt::x(p) << ' ' << pt::y(p));
}

//-- END Mesh2DAdapter 

//--------------------------------------------------------------
//-- BEGIN Vertex

inline 
Vertex::Vertex() : g(0), v(-1) {
  // empty
}

inline  
Vertex::Vertex(Vertex::grid_type const& gg): g(&gg), v(0) {
  // empty
}

inline 
Vertex::Vertex(Vertex::grid_type const& gg, int vv) : g(&gg), v(vv) {
  // empty
}
  
inline 
Vertex::~Vertex() {
  // empty
}  


inline  bool 
Vertex::IsDone() const { 
  cb(); 
  return (v == g->NumOfVertices()); 
}

inline Vertex const& 
Vertex::operator*() const { 
  cv(); 
  return *this;
}

inline Vertex & 
Vertex::operator++() { 
  cv(); 
  ++v; 
  return *this;
}

inline   Vertex::grid_type const& 
Vertex::TheGrid() const { 
  cb(); 
  return *g;
}
inline Vertex::vertex_handle   
Vertex::handle()  const { 
  cv(); 
  return v;
}

// checking functions
inline bool 
Vertex::bound() const { 
  return g != 0;
}

inline bool 
Vertex::valid() const { 
  return (bound() &&  (0 <= v) && (v < TheGrid().NumOfVertices()));
}

inline void 
Vertex::cb() const { 
  REQUIRE (bound(), "",1);
}

inline void 
Vertex::cv() const { 
  REQUIRE (valid(), "",1);
}

inline bool operator==(Vertex const& lhs, Vertex const& rhs) {
    return lhs.v == rhs.v;
}

inline bool operator!=(Vertex const& lhs, Vertex const& rhs) {
    return lhs.v != rhs.v;
}
//-- END Vertex

//-----------------------------------------------------------------
//-- BEGIN Cell
inline   
Cell::Cell() : g(0), c(-1) {
 // empty
}
  
inline 
Cell::Cell(Cell::grid_type const& gg) : g(&gg), c(0) {
}

inline 
Cell::Cell(Cell::grid_type const& gg, Cell::cell_handle cc) : g(&gg), c(cc) {
}

inline
Cell::~Cell() {
  // empty
}  

inline  bool 
Cell::IsDone() const { 
  cb(); 
  return (c == g->NumOfCells()); 
}

inline Cell const& 
Cell::operator*() const { 
  cv(); 
  return *this;
}

inline Cell & 
Cell::operator++() { 
  cv(); 
  ++c; 
  return *this;
}

inline Cell::grid_type const& 
Cell::TheGrid() const { 
  return *g;
}

inline Cell::cell_handle
Cell::handle() const { 
  return c;
}


inline VertexOnCellIterator
Cell::FirstVertex() const {
  return VertexOnCellIterator(*this);
}

inline VertexOnCellIterator
Cell::EndVertex() const {
  return VertexOnCellIterator(*this, NumOfVertices());
}


inline  unsigned 
Cell::NumOfVertices() const { 
  return 3;
}

inline unsigned 
Cell::NumOfEdges() const {
  return 3;
}

inline unsigned 
Cell::NumOfFacets() const {
  return 3;
}

inline Cell::vertex_handle 
Cell::v(int lv) const {
  Vert* v=gcell->pVVert(lv);
  return vertex_handle(g->TheAdaptee()->iVertIndex(v));
}

inline Vertex
Cell::V(int lv) const {
  return Vertex(TheGrid(), v(lv));
}
  
inline Cell::archetype_type const& 
Cell::TheArchetype() const { 
   return TheGrid().ArchetypeOf(*this);
}

// checking functions
inline bool 
Cell::bound() const { 
  return g != 0;
}

inline bool 
Cell::valid() const { 
  return (bound() &&  (0 <= c) && (c < TheGrid().NumOfCells()));
}

inline void 
Cell::cb() const { 
  REQUIRE (bound(), "",1);
}

inline void 
Cell::cv() const { 
  REQUIRE (valid(), "",1);
}

inline bool operator==(Cell const& lhs, Cell const& rhs) {
    return lhs.c == rhs.c;
}

inline bool operator!=(Cell const& lhs, Cell const& rhs) {
    return lhs.c != rhs.c;
} 
//-- END Cell

//--------------------------------------------------------------
//-- BEGIN VertexOnCellIterator

inline 
VertexOnCellIterator::VertexOnCellIterator() : vc(-1), a(0), gcell(0) {
  // empty
  a = NULL;
  gcell=NULL;
}
inline 
VertexOnCellIterator::VertexOnCellIterator(Cell const& cc, int vvc): c(cc), vc(vvc) {
  a = const_cast<Adaptee*>(c.g->TheAdaptee());
  gcell = a->pCCell(c.c); 
}

inline VertexOnCellIterator& 
VertexOnCellIterator::operator++() { 
  cv();  
  ++vc; 
  return *this;
}

inline Vertex 
VertexOnCellIterator::operator*() const { 
  cv(); 
  return Vertex(TheGrid(), handle());
}

inline bool 
VertexOnCellIterator::IsDone() const { 
  cb(); 
  return (vc == 3);
}

inline VertexOnCellIterator::grid_type const& 
VertexOnCellIterator::TheGrid() const { 
  cb(); 
  return c.TheGrid();
}

inline Cell const& 
VertexOnCellIterator::TheCell() const { 
  cb(); 
  return c;
}

inline VertexOnCellIterator::vertex_handle 
VertexOnCellIterator::handle() const { 
  cv(); 
  return a->iVertIndex(gcell->pVVert(vc));
}  

inline int 
VertexOnCellIterator::local_handle() const { 
  cv(); 
  return vc;
}

// checking functions
inline bool 
VertexOnCellIterator::bound() const { 
  return c.valid();
}

inline bool 
VertexOnCellIterator::valid() const { 
  return (c.valid() &&  (0 <= vc) && (vc < 3));
}

inline void VertexOnCellIterator::cb() const { 
  REQUIRE (bound(), "",1);
}

inline void VertexOnCellIterator::cv() const { 
  REQUIRE (valid(), "",1);
}

inline bool 
operator==(VertexOnCellIterator const& lhs, 
                                 VertexOnCellIterator const& rhs) { 
    return lhs.vc == rhs.vc;
}

inline bool 
operator!=(VertexOnCellIterator const& lhs, 
                                 VertexOnCellIterator const& rhs) { 
    return lhs.vc != rhs.vc;
}
//-- END VertexOnCellIterator

#endif
