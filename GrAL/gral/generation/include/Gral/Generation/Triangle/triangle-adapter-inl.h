#ifndef GR_TRIANGLE_ADAPTER_INL_H
#define GR_TRIANGLE_ADAPTER_INL_H

//--------------------------------------------------------------
//-- BEGIN TriangleAdapter 

inline
TriangleAdapter::TriangleAdapter() : _a(0), owned(false) {
  // empty
}

inline 
TriangleAdapter::TriangleAdapter(TriangleAdapter::Adaptee *a) 
                                         : _a(a), owned(false) {
  // empty
}

inline 
TriangleAdapter::~TriangleAdapter() {
  clear();
  // empty
}

inline void
TriangleAdapter::clear() {
  if (owned) {
    free_output_triangulateio(_a); 
    delete _a;
  } 
  _a = NULL;
  owned = false;
}

inline void
TriangleAdapter::init(TriangleAdapter::Adaptee *a) {
  clear();
  _a = a;
  _coordlist = _a->pointlist;
}

inline void
TriangleAdapter::steal(TriangleAdapter::Adaptee *a) {
  init(a);
  owned = true;
}

inline bool 
TriangleAdapter::IsOwner() const {
  return owned;
}

inline unsigned 
TriangleAdapter::dimension() const { 
  return dim;
}



inline int 
TriangleAdapter::NumOfVertices() const  { 
  REQUIRE(_a != NULL, "not bound", 1); 
  return _a->numberofpoints;
}

inline int 
TriangleAdapter::NumOfCells() const {
  REQUIRE(_a != NULL, "not bound", 1);
  return _a->numberoftriangles;  
}  

inline bool 
TriangleAdapter::IsBound() const {
  return _a != NULL;
}

inline const TriangleAdapter::Adaptee* 
TriangleAdapter::TheAdaptee() const {
  return _a;
}

inline double const* 
TriangleAdapter::getCoordsList() const { 
  REQUIRE(_a != NULL, "not bound", 1);
  return  _a->pointlist;
}

inline int const* 
TriangleAdapter::getCellsList() const {
  REQUIRE(_a != NULL, "not bound", 1); 
  return _a->trianglelist;
}

inline bool 
TriangleAdapter::valid(TriangleAdapter::vertex_handle v) const {
  return (0 <= v  && v < NumOfVertices()); 
}
inline bool 
TriangleAdapter::valid(TriangleAdapter::cell_handle c) const {
  return  (0 <= c && c < NumOfCells());
}

inline TriangleAdapter::CellIterator
TriangleAdapter::FirstCell() const { 
  return CellIterator(*this);
}

inline TriangleAdapter::VertexIterator
TriangleAdapter::FirstVertex() const { 
  return VertexIterator(*this);
}

inline unsigned 
TriangleAdapter::space_dimension() const { 
  return 2;
}

inline TriangleAdapter::grid_type const& 
TriangleAdapter::TheGrid() const {
  return *this;
}

inline TriangleAdapter::coord_type  
TriangleAdapter::coord(Vertex const& v) const { 
    const double *cs = _coordlist + 2*v.handle();
    return coord_type(*cs, *(cs+1));
}    
inline std::ostream&
operator<<(std::ostream& out, TriangleAdapter::coord_type const& p)
{ 
  typedef TriangleAdapter::pt pt;
  return (out << pt::x(p) << ' ' << pt::y(p));
}

//-- END TriangleAdapter 

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
  init_cellslist();
}

inline 
Cell::Cell(Cell::grid_type const& gg, Cell::cell_handle cc) : g(&gg), c(cc) {
  init_cellslist();
}

inline
Cell::~Cell() {
  // empty
}  

inline void 
Cell::init_cellslist() {
  cellslist = g->getCellsList();
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
  return grid_type::vertex_handle(cellslist[3*c+lv]);
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
VertexOnCellIterator::VertexOnCellIterator() : vc(-1) {
  // empty
}
inline 
VertexOnCellIterator::VertexOnCellIterator(Cell const& cc, int vvc): c(cc), vc(vvc) {
  // empyt
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
  return c.cellslist[3*c.handle()+vc];
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

#endif /* ifndef  GRAL_MG_TRIANGLE_ADAPTER_INL_H */
