#ifndef GRAL_VTK_UGRID_ADAPTER_H
#define GRAL_VTK_UGRID_ADAPTER_H

// $LICENSE_CUT_NEC_2007

#include "Gral/Adapters/VTK/vtk-geometry.h"
#include "Geometry/coords.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-handle.h"
#include "Gral/Grids/ComplexND/all.h"
#include "Gral/Grids/Complex2D/all.h"

// #include "Gral/Base/polygon.h"

#include <Utility/pre-post-conditions.h>

#include "Container/bijective-mapping.h"

#include <vector>
#include <string>
#include <iostream>


#include <vtkUnstructuredGrid.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkVertex.h>
#include <vtkIdList.h>



namespace GrAL {
namespace vtk_ugrid {

enum { ANY = -1};

using namespace vtk;

template <class GRID>
class CellVTK;

template <class GRID>
class VertexVTK;

template <class GRID>
class VertexIteratorVTK;

template <class GRID>
class CellIteratorVTK;

template <class GRID>
class VertexOnCellIteratorVTK;

template <int D>
class UGridVTKAdapter;


template<int D> struct get_archetype_type     { typedef complexnd::ComplexND<D-1> type;};
template<>      struct get_archetype_type<3>  { typedef Complex2D                 type;};
template<>      struct get_archetype_type<ANY>{ typedef complexnd::ComplexND<ANY> type;};
    

  class UGridVTKArchetypes_Base {
  public:
    static std::string VTK_type(int t);
  }; 

template <int D>
struct grid_types_UGridVTKAdapter 
  : public grid_types_detail::grid_types_root,
    public UGridVTKArchetypes_Base 
{
  typedef UGridVTKAdapter<D>       grid_type;
  typedef VertexVTK<grid_type>              Vertex;
  typedef CellVTK<grid_type>                Cell;
  typedef CellIteratorVTK<grid_type>        CellIterator;
  typedef VertexIteratorVTK<grid_type>      VertexIterator;

  typedef vertex_handle_int<grid_type>              vertex_handle;
  typedef cell_handle_int  <grid_type>              cell_handle;

  typedef VertexOnCellIteratorVTK<grid_type>  VertexOnCellIterator;

  typedef grid_dim_tag<D>                   dimension_tag;

  // FIXME: does not work for D==ANY
  //  typedef complexnd::ComplexND<D-1>            archetype_type;
  typedef typename get_archetype_type<D>::type archetype_type;

  typedef std::vector<archetype_type>        archetype_sequence;
  typedef archetype_sequence                 archetype_table;
  typedef typename archetype_sequence::const_iterator archetype_iterator;
  typedef int                                archetype_handle;
  typedef grid_types<archetype_type>         archgt;
  typedef vtkUnstructuredGrid                Adaptee;  

  typedef vtk::coord_type coord_type;
  typedef vtk::coord_proxy coord_proxy;
  typedef point_traits<coord_type> pt;
  typedef algebraic_primitives<coord_type> ap;
  typedef double scalar_type; 
};

// Singleton class
template <int D>
class UGridVTKArchetypes 
  : public grid_types_UGridVTKAdapter<D>
 {
  typedef grid_types_UGridVTKAdapter<D> gt;
  public:
  typedef bijective_mapping<int, std::string> archetype_name_map;
  typedef bijective_mapping<int, int> vtk_type_map;
  typedef typename gt::archetype_handle archetype_handle;
  typedef typename gt::archetype_type   archetype_type;
  typedef typename gt::archetype_iterator archetype_iterator;
  typedef typename gt::archetype_table   archetype_table;
  typedef typename gt::Cell Cell;

  //
  static UGridVTKArchetypes& Get() {
    static UGridVTKArchetypes theArchetypes;
    return theArchetypes;
  }
     
  inline int vtk_type(archetype_handle handle) const {
    return vtk_types(handle);
  }

  inline std::string const& name(archetype_handle handle) const {
    return names(handle);   
  }
     
  //  std::string const& cell_name(int nv) const;


  inline archetype_type const & Archetype(archetype_handle a) const {
    return archetypes[a];  
  }

  inline archetype_iterator BeginArchetype() const {
    return archetypes.begin();  
  }

  inline archetype_iterator EndArchetype() const {
    return archetypes.end();  
  }

  archetype_type const& ArchetypeOf (Cell const& c) const; 
  archetype_handle archetype_of(Cell const& c) const; 
     
  inline unsigned NumOfArchetypes() {
    return archetypes.size();
  }
     
  inline archetype_handle handle(archetype_iterator a) {
    return a - archetypes.begin();  
  }

  private: 
  archetype_table    archetypes;
  archetype_name_map names;
  vtk_type_map       vtk_types;

  UGridVTKArchetypes();
  UGridVTKArchetypes(const UGridVTKArchetypes&); // not implemented
  UGridVTKArchetypes& operator=(const UGridVTKArchetypes&); // not implemented 
  ~UGridVTKArchetypes();
};

template<int D>
struct dimension_mixin_grid {
  enum { dim = D };
  //! \brief Grid dimension
  unsigned dimension() const { return D;} 
};


  /*! \brief Gral grid adapter for vtkUnstructuredGrid

    \ingroup vtkugridadaptermodule 
     
     This class is a model of
     -  $GrAL ArchetypedGrid
     -  $GrAL VertexRange, $GrAL CellRange

  */
template <int D>
class UGridVTKAdapter : public grid_types_UGridVTKAdapter<D>, 
                        public dimension_mixin_grid<D> {
  typedef dimension_mixin_grid<D> mixin;
  typedef grid_types_UGridVTKAdapter<D> gt;
  public:
  typedef typename gt::grid_type  grid_type;
  typedef typename gt::grid_type anchor_type;
  typedef typename gt::VertexIterator VertexIterator;
  typedef typename gt::CellIterator   CellIterator;
  typedef typename gt::Cell           Cell;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::cell_handle cell_handle;

  typedef typename gt::archetype_handle archetype_handle;
  typedef typename gt::archetype_type   archetype_type;
  typedef typename gt::archetype_iterator archetype_iterator;
  typedef typename gt::Adaptee Adaptee;

  private:
  Adaptee *adaptee_;

  inline void make_empty() {
    if (adaptee_) {
      adaptee_->UnRegister(NULL);
      adaptee_ = NULL;  
    }
  }
  public:

  /*! \brief Empty grid
   */
  inline UGridVTKAdapter() : adaptee_(0) {
  }

  //! make this an empty grid
  inline void clear() {
    make_empty();
    adaptee_ = vtkUnstructuredGrid::New();
  }

  inline void allocate(int npoints, int ncells) {
     adaptee_->Allocate(ncells);
     vtkPoints *points = vtkPoints::New();
     points->Allocate(npoints);
     adaptee_->SetPoints(points);
  }

  /*! \brief Construct a view to the vtkUnstructuredGrid \c a
   */
  inline explicit UGridVTKAdapter(Adaptee *a) : adaptee_(a) {
    if (adaptee_) {
      adaptee_->Register(NULL); // VTK objects are reference counted
    }
  }

  inline ~UGridVTKAdapter() {
    make_empty();
  }

  inline void SetAdaptee(Adaptee *a) {
    REQUIRE(a != NULL, "Initalization with NULL pointer", 1);
    make_empty();
    adaptee_ = a;
    adaptee_->Register(NULL);
  }

  Adaptee* GetAdaptee() const {
    return adaptee_;
  }

  inline bool IsOwner() const { 
    if (adaptee_) {
      return (adaptee_->GetReferenceCount() <= 1);
    }
    return false;
  }

  inline int NumOfVertices() const {
    REQUIRE(adaptee_ != NULL, "not bound", 1);
    return adaptee_->GetNumberOfPoints();
  } 

  inline int NumOfCells() const {
    REQUIRE(adaptee_ != NULL, "not bound", 1);
    return adaptee_->GetNumberOfCells();
  }
  
  inline bool IsBound() const {
    return adaptee_ != NULL;
  }

  inline const Adaptee* TheAdaptee() const {
    return adaptee_;
  }

  //@{
  /*! \name Sequence iteration
      \todo STL-style EndXXX() 
  */
  inline VertexIterator FirstVertex() const;

  inline VertexIterator EndVertex() const;

  inline vertex_handle handle(const Vertex& V)  const;
  inline Vertex vertex(const vertex_handle& v) const;

  inline CellIterator   FirstCell()   const;

  inline CellIterator   EndCell() const;

  inline cell_handle handle(const Cell& C)  const;

  inline Cell cell(const cell_handle& c) const;
    
  //@}

  bool valid(vertex_handle v) const {
    return (0 <= v && v < NumOfVertices());  
  }
  
  bool valid(cell_handle c) const {
    return (0 <= c && c < NumOfCells());
  }
//@}

  /*@{*/
  /*! \name Archetype handling
   
     \see $GrAL ArchetypedGrid concept
  */

  static archetype_type const& Archetype(archetype_handle a) {
    return UGridVTKArchetypes<D>::Get().Archetype(a);
  }

  static  archetype_type const& ArchetypeOf (Cell const& c) { 
    return UGridVTKArchetypes<D>::Get().ArchetypeOf(c);
  }

  inline  archetype_type const& ArchetypeOf (cell_handle handle) const;

  inline archetype_handle  archetype_of(cell_handle handle) const;

  static archetype_handle archetype_of(Cell const& c) { 
    return UGridVTKArchetypes<D>::Get().archetype_of(c);
  }

  static archetype_iterator BeginArchetype() { 
    return UGridVTKArchetypes<D>::Get().BeginArchetype();
  }

  static archetype_iterator EndArchetype() { 
    return UGridVTKArchetypes<D>::Get().EndArchetype();
  }

  static unsigned NumOfArchetypes() { 
    return UGridVTKArchetypes<D>::Get().NumOfArchetypes();
  }

  static archetype_handle handle(archetype_iterator a) {
    return UGridVTKArchetypes<D>::Get().handle(a);
  }

  // Vertex Geometry part
  
  inline unsigned space_dimension() const {
    return 3;
  }
    
  inline grid_type const& TheGrid() const {
    return *this;
  }

  void set_grid(grid_type const& gg) { REQUIRE((this == &gg),
             "UGridVTKAdapter must be its own gemetry",1);}
  void rebind(grid_type const& gg) { REQUIRE((this == &gg),
             "UGridVTKAdapter must be its own gemetry",1);}
  bool bound() const {return true;}

  /*! \brief Coordinate of Vertex (read/write access) */
  inline coord_proxy  coord(Vertex const& v);
  inline coord_proxy  coord(vertex_handle const h);

  /*! \brief Coordinate of Vertex (read access) */
  inline coord_type coord(Vertex const& v) const; 
  inline coord_type coord(vertex_handle const h) const;


  public:
/*
  template<class G_SRC, class VCORR, class CCORR>
  friend
  void ConstructGrid0(vtk_ugrid::UGridVTKAdapter & G_dest,
		      G_SRC    const& G_src,
		      VCORR         & v_corr,
		      CCORR         & c_corr);
*/
  // set dependent information
  void calculate_dependent();

  // modifying functions (used in ConstructGrid())
  vertex_handle add_vertex();
  cell_handle   add_cell(archetype_handle a);

}; //-- END UGridVTKAdapter

template <class GRID>
class VertexVTK : public grid_types_UGridVTKAdapter<GRID::dim> {
  typedef grid_types_UGridVTKAdapter<GRID::dim> gt;
  typedef VertexVTK<GRID> self;
public:
  typedef typename gt::vertex_handle handle_type;  
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::grid_type anchor_type;
  typedef typename gt::grid_type grid_type;
  typedef vertex_type_tag      element_type_tag;
  typedef grid_vertex_category category;
  private:
    handle_type vid_;
    const grid_type *grid_;

  public: 
  /*!
   * Protocol: STL_DefaultConstructible
   */
  inline VertexVTK() : vid_(-1), grid_(0) {
  }; 
  
  /*!
   * Protocol: GridEntity
   */
  inline explicit VertexVTK(grid_type const& grid) : vid_(0), grid_(&grid) { 
    // empty body
  }

  /*!
   * Protocol: GridElement
   */
  inline VertexVTK(grid_type const& grid, handle_type h) : vid_(h), 
                                                           grid_(&grid) {
     // empty body
  }


  /*! 
   * Protocol: STL_EqualityComparable
   */
  bool operator==(const self& rhs) const {
    REQUIRE((grid_ == rhs.grid_), 
	    "cannot compare vertices from different grids !\n",1);
    return (vid_ == rhs.vid_);
  }
  /*! 
   * Protocol: STL_EqualityComparable
   */
  bool operator!=(const self& rhs) const { return !(*this == rhs);}

  /*!
   * Protocol: STL_LessThanComparable
   */
  bool operator< (const self& rhs) const { 
    REQUIRE((grid_ == rhs.grid_), 
	    "cannot compare vertices from different grids !\n",1);
    return (vid_ < rhs.vid_);
  }

  inline grid_type const& TheGrid()  const {
    return *grid_;
  }

  /*!
   * Protocol: GridEntity
   */
  inline anchor_type const& TheAnchor() const {
    return TheGrid();
  }

  /*! 
   * Protocol: GridEntity
   */
  inline handle_type handle()  const {
    return vid_;
  }
};


/************** VertexIteratorVTK *************************/

template <class GRID>
class VertexIteratorVTK  : public grid_types_UGridVTKAdapter<GRID::dim> {
  typedef grid_types_UGridVTKAdapter<GRID::dim> gt;
  typedef VertexIteratorVTK<GRID> self;
  public:
  typedef typename gt::vertex_handle handle_type;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::grid_type anchor_type;
  typedef typename gt::grid_type grid_type;

  typedef typename gt::Vertex         element_type; 
  typedef element_type                value_type;
  typedef grid_cell_iterator_category category;

  private:
  handle_type pos_;
  handle_type endpos_;
  grid_type * grid_;

  public:
  /*!
   * Protocol: STL_DefaultConstructible 
   */
  VertexIteratorVTK() : pos_(0), endpos_(0), grid_(0) {
    // empty body
  }


  /*!
   * Protocol: GridEntity
   */
  explicit inline VertexIteratorVTK(grid_type const& grid) : pos_(0),
                                     grid_(const_cast<grid_type*>(&grid)) { 
    endpos_ = grid_->NumOfVertices(); 
  } 
  /*!
   * Protocol: GridVertexIterator
   */
  VertexIteratorVTK(grid_type const& grid, 
                   const handle_type& h) :
    grid_(const_cast<grid_type*>(&grid)) {
    pos_ = h;
    endpos_ = grid_->NumOfVertices(); 
  }

  /*!
   * Protocol: STL_BasicIteratorOperations
   */
  inline   VertexIteratorVTK& operator++() {
    ++pos_; return (*this);  
  }

  /*!
   * Protocol: GridEntity
   */
  inline grid_type const& TheGrid()  const {
    return (*grid_);
  }

  /*!
   * Protocol: GridEntity
   */
  inline anchor_type const& TheAnchor() const {
    return TheGrid();
  }

  /*!
   * Protocol: GridEntity
   */
  inline handle_type handle()  const {
    return pos_;
  }

  /*! 
   * Protocol: STL_TrivialIterator
   */
  inline   VertexVTK<GRID> operator*() const {
    return (grid_->vertex(pos_)); 
  }

  /*!
   * Protocol: GridSequenceIterator
   */
  inline bool IsDone() const {
    return (pos_ == endpos_);  
  }

  /*!
   * Protocol: GridSequenceIterator
   */
  inline operator bool() const {
    return !IsDone();
  }

  /*!
   * Protocol: STL_EqualityComparable
   */
  bool operator==(const self& rhs) const {
     REQUIRE((grid_ == rhs.grid_), "Grids don't match!\n",1);
     return (pos_ == rhs.pos_); 
  }
  /*!
   * Protocol: STL_EqualityComparable
   */
  bool operator!=(const self& rhs) const { return !(*this == rhs);}

  /*!
   * Protocol: STL_LessThanComparable
   */
  bool operator< (const self& rhs) const {
    REQUIRE((grid_ == rhs.grid_), "Grids don't match!\n",1);
    return (pos_ < rhs.pos_); 

  }
 
};



/************************ CellVTK ***************************/
template<class GRID>
class CellVTK : public grid_types_UGridVTKAdapter<GRID::dim> {
  typedef grid_types_UGridVTKAdapter<GRID::dim> gt;
  typedef CellVTK<GRID> self;
  public:
  typedef typename gt::cell_handle handle_type;
  typedef typename gt::cell_handle cell_handle;
  typedef typename gt::grid_type anchor_type;
  typedef typename gt::grid_type grid_type;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  typedef cell_type_tag      element_type_tag;
  typedef grid_cell_category category;

  friend class vtk_ugrid::VertexOnCellIteratorVTK<GRID>;

  private:

  handle_type cid_;
  const grid_type *grid_;
  vtkCell *cellvtk_;

  void setCell(vtkCell* c) {
    if (cellvtk_) {
       cellvtk_->UnRegister(NULL); 
       cellvtk_ = NULL;
    }
    if(c != NULL) {
     cellvtk_ = c;
     cellvtk_->Register(NULL);
    }
  }

  public:
  /*!
   * Protocol: STL_DefaultConstructible
   */
  CellVTK() : cid_(-1), grid_(NULL), cellvtk_(NULL)  {
    //empyt body
  }

  /*!
   * Protocol: STL_DefaultConstructible
   */

  ~CellVTK() {
    if (cellvtk_) {
     cellvtk_->UnRegister(NULL);     
     cellvtk_ = NULL;
    }
  }
  /*
   * Protocol: STL_Assignable
   */
  CellVTK(const CellVTK& rhs) : cid_(rhs.cid_), grid_(rhs.grid_), 
                                cellvtk_(NULL) {
    setCell(rhs.cellvtk_);
  }
  
  CellVTK& operator=(const self& rhs) {
     if (this != &rhs) {
       cid_ = rhs.cid_;
       grid_ = rhs.grid_;
       setCell(rhs.cellvtk_);
     }
     return *this; 
  }

  /*! 
   * Protocol : GridEntity
   */
  explicit CellVTK(grid_type const& grid) : cid_(0), grid_(&grid), 
                                            cellvtk_(NULL) {
    vtkCell* c = grid.GetAdaptee()->GetCell(cid_);
    setCell(c); 
  }

  /*!
   * Protocol: GridElement
   */
  inline CellVTK(grid_type const& grid, handle_type h) : cid_(h), 
                                                         grid_(&grid),
                                                         cellvtk_(NULL) { 
                                                           
    vtkCell* c = grid.GetAdaptee()->GetCell(cid_);
    setCell(c); 
  }


  /*! 
   * Protocol : GridEntity
   */
  inline grid_type const& TheGrid()  const { 
    cb(); return *grid_;
  }

  /*! 
   * Protocol : GridEntity
   */
  inline anchor_type const& TheAnchor() const {
    return TheGrid();
  }

  /*! 
   * Protocol : GridEntity
   */
  inline handle_type handle()  const { 
    cv(); return cid_; 
  }


  /*!
   * Protocol: VertexRangeCell
   */
  inline int NumOfVertices() const { 
    REQUIRE(cellvtk_ != NULL, "not bound", 1);
    return cellvtk_->GetNumberOfPoints();
  }

  inline VertexOnCellIterator FirstVertex() const;
  inline VertexOnCellIterator EndVertex() const;
  
  // checking functions
  inline bool bound() const {  return grid_ != 0; }
  inline bool valid() const { 
    return (bound() &&  (0 <= cid_) && (cid_ < TheGrid().NumOfCells()));
  }
  inline void cb() const { REQUIRE (bound(), "",1); }
  inline void cv() const { REQUIRE (valid(), "",1); }

  /*!
   * Protocol STL_EqualityComparable
   */
  bool operator==(const self& rhs) { 
    REQUIRE((grid_ == rhs.grid_), 
	    "cannot compare cells from different grids !\n",1);
    return (cid_ == rhs.cid_);
  }  
  
  /*!
   * Protocol STL_EqualityComparable
   */
  bool operator!=(const self& rhs) { return !(*this == rhs);}
};



template <class GRID>
class CellIteratorVTK : public grid_types_UGridVTKAdapter<GRID::dim> {
  typedef grid_types_UGridVTKAdapter<GRID::dim> gt;
  typedef CellIteratorVTK<GRID> self;
  public:
  typedef typename gt::cell_handle handle_type;
  typedef typename gt::cell_handle cell_handle;
  typedef typename gt::grid_type anchor_type;
  typedef typename gt::grid_type grid_type;
  typedef typename gt::Cell Cell;
  typedef Cell                        element_type; 
  typedef element_type                value_type;
  typedef grid_cell_iterator_category category;
  private:
    handle_type pos_;
    handle_type endpos_;
    grid_type const* grid_;

  public:
  /*!
   * Protocol: STL_DefaultConstructible
   */
  CellIteratorVTK() : pos_(0), endpos_(0), grid_(0) {
    // empty body
  }

  explicit inline CellIteratorVTK(grid_type const& grid) : pos_(0), 
                                                           grid_(&grid) {
    endpos_ = grid_->NumOfCells(); 
  };
  /*!
   * Protocol: GridCellIterator
   */
  inline CellIteratorVTK(grid_type const& grid, 
                         const handle_type& h) : pos_(h),
                                                               grid_(&grid) {
    endpos_ = grid_->NumOfCells();
  }


  /*! 
   * Protocol : GridEntity
   */
  inline grid_type const& TheGrid()  const { 
    return *grid_;
  }

  /*! 
   * Protocol : GridEntity
   */
  inline anchor_type const& TheAnchor() const {
    return TheGrid();
  }

  /*! 
   * Protocol : GridEntity
   */
  inline handle_type handle()  const { 
    return pos_; 
  }

  /*!
     Protocol : STL_ForwardIterator
  */
  self& operator++() { ++pos_; return *this;}

  /*!
   * Protocol: STL_TrivialIterator
   */
  inline   Cell operator*() const {
    return (grid_->cell(pos_)); 
  }

  /*!
   * Protocol: GridSequenceIterator
   */
  inline bool IsDone() const {
    return pos_ == endpos_;  
  }

  /*!
   * Protocol: GridSequenceIterator
   */
  inline operator bool() const {
    return !IsDone();  
  }

  /*!
   * Protocol: STL_EqualityComparable
   */
  bool operator==(const self& rhs) const {
     REQUIRE((grid_ == rhs.grid_), "Grids don't match!\n",1);
     return (pos_ == rhs.pos_); 
  }
  /*!
   * Protocol: STL_EqualityComparable
   */
  bool operator!=(const self& rhs) const { return !(*this == rhs);}

  /*!
   * Protocol: STL_LessThanComparable
   */
  bool operator< (const self& rhs) const {
    REQUIRE((grid_ == rhs.grid_), "Grids don't match!\n",1);
    return (pos_ < rhs.pos_); 

  }

};


/******************** VertexOnCellIterator *************************/
template <class GRID>
class VertexOnCellIteratorVTK : public grid_types_UGridVTKAdapter<GRID::dim> {
  typedef grid_types_UGridVTKAdapter<GRID::dim> gt;
  typedef VertexOnCellIteratorVTK<GRID> self;
public:
  typedef typename gt::vertex_handle handle_type;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::grid_type anchor_type;
  typedef typename gt::grid_type grid_type;
  typedef typename gt::Cell Cell;
  typedef typename gt::Vertex Vertex;

  typedef Vertex       element_type; 
  typedef element_type value_type;

  typedef grid_incidence_iterator_category_d<0,GRID::dim> category;

private:
  Cell      c_;
  int       vc_;
  int       vc_end_;
  vtkIdList *nodes_;

public:
  inline VertexOnCellIteratorVTK() : vc_(-1), vc_end_(-1), 
                                     nodes_(NULL) {
    // empty body
  };

  inline explicit VertexOnCellIteratorVTK(Cell const& cc, 
                                       int vvc = 0) : c_(cc), vc_(vvc) {
    vc_end_ = c_.NumOfVertices();
    nodes_ = c_.cellvtk_->GetPointIds();
  }

  inline VertexOnCellIteratorVTK& operator++() {
    cv();
    ++vc_;
    return *this;
  }

  inline Vertex operator*() const {
    cv();
    return Vertex(TheGrid(), handle());
  }

  inline bool IsDone() const {
    cb();
    return (vc_ == vc_end_);
  }

  inline grid_type const& TheGrid() const {
    return c_.TheGrid();
  }

  inline Cell const& TheCell() const {
    cb();
    return *c_;
  }
  inline vertex_handle handle() const {
    cv();
    return nodes_->GetId(vc_);
  }
  inline int local_handle()     const {
    cv();
    return vc_;
  }

  // checking functions
  inline bool bound() const { return c_.valid();}
  inline bool valid() const { return c_.valid() 
                             && (0 <= vc_) && (vc_ < vc_end_); } 
  inline void cb()    const { REQUIRE (bound(), "",1);}
  inline void cv()    const { REQUIRE (valid(), "",1);}

  /*! 
   * Protocol: STL_LessThanComparable
   */
  bool operator==(self const& rhs) const {
    REQUIRE((c_ == rhs.c_), 
	    "cannot compare iterators from different cells!\n",1);
    return (vc_ == rhs.vc_);
  }
  /*! 
   * Protocol: STL_LessThanComparable
   */
  bool operator!=(self const& rhs) const { return !(*this == rhs);}
};



/********************************************************************** 
 *      UGridVTKAdapter implementation
 **********************************************************************/
template <int D>
inline typename UGridVTKAdapter<D>::archetype_type const& 
UGridVTKAdapter<D>::ArchetypeOf (UGridVTKAdapter<D>::cell_handle handle) const {
  return UGridVTKArchetypes<D>::Get().ArchetypeOf(Cell(TheGrid(), handle)); 
} 

template <int D>
inline typename UGridVTKAdapter<D>::archetype_handle  
UGridVTKAdapter<D>::archetype_of(UGridVTKAdapter<D>::cell_handle handle) const {
  return UGridVTKArchetypes<D>::Get().archetype_of(Cell(TheGrid(), handle));  
} 


template <int D>
inline typename UGridVTKAdapter<D>::VertexIterator 
UGridVTKAdapter<D>::FirstVertex() const {
  return VertexIterator(*this, 0);
}

template <int D>
inline typename UGridVTKAdapter<D>::VertexIterator 
UGridVTKAdapter<D>::EndVertex() const {
  return VertexIterator(*this, NumOfVertices());
}

template <int D>
inline typename UGridVTKAdapter<D>::vertex_handle 
UGridVTKAdapter<D>::handle(const UGridVTKAdapter<D>::Vertex& V)  const { 
  REQUIRE(( &(V.TheGrid()) == this),
         "handle(Vertex): V.TheGrid() == " 
         <<  &(V.TheGrid()) << " != this = " << this << "\n",1);
  return V.handle();
}

/*!
 * Protocol: VertexGridRange
 */
template <int D>
inline typename UGridVTKAdapter<D>::Vertex 
UGridVTKAdapter<D>::vertex(const UGridVTKAdapter<D>::vertex_handle& v) const {
  return Vertex(*this,v);
}

template <int D>
inline typename UGridVTKAdapter<D>::CellIterator   
UGridVTKAdapter<D>::FirstCell()   const {
  return CellIterator(*this, 0);
}

template <int D>
inline typename UGridVTKAdapter<D>::CellIterator  
UGridVTKAdapter<D>::EndCell() const {
  return CellIterator(*this, NumOfCells());
}

template <int D>
inline typename UGridVTKAdapter<D>::cell_handle 
UGridVTKAdapter<D>::handle(const UGridVTKAdapter::Cell& C)  const { 
  REQUIRE(( &(C.TheGrid()) == this),
         "handle(Cell): C.TheGrid() == " 
         <<  &(C.TheGrid()) << " != this = " << this << "\n",1);
  return C.handle();
}

template <int D>
inline typename UGridVTKAdapter<D>::Cell 
UGridVTKAdapter<D>::cell(const UGridVTKAdapter::cell_handle& c) const {
  return Cell(*this,c);
}

template <int D>
inline coord_proxy  
UGridVTKAdapter<D>::coord(UGridVTKAdapter<D>::Vertex const& v) { 
  REQUIRE(adaptee_ != NULL, "not bound", 1);
  return coord_proxy(adaptee_->GetPoints(), v.handle());
} 

template <int D>
inline coord_proxy  
UGridVTKAdapter<D>::coord(UGridVTKAdapter<D>::vertex_handle const h) { 
  REQUIRE(adaptee_ != NULL, "not bound", 1);
  return coord_proxy(adaptee_->GetPoints(), h);
} 


template <int D>
inline coord_type  
UGridVTKAdapter<D>::coord(UGridVTKAdapter<D>::Vertex const& v) const { 
  REQUIRE(adaptee_ != NULL, "not bound", 1);
  return coord_type(adaptee_->GetPoint(v.handle()));
} 

template <int D>
inline void
UGridVTKAdapter<D>::calculate_dependent() {
  // empty
}

// modifying functions (used in ConstructGrid())
template <int D>
inline typename UGridVTKAdapter<D>::vertex_handle
UGridVTKAdapter<D>::add_vertex() { 
  return (adaptee_->GetPoints()->InsertNextPoint(0.0,0.0,0.0));
}

template <int D>
inline typename UGridVTKAdapter<D>::cell_handle
UGridVTKAdapter<D>::add_cell(UGridVTKAdapter<D>::archetype_handle a) { 
  cell_handle ret = NumOfCells();
  unsigned nv = Archetype(a).NumOfVertices();
 
  adaptee_->GetCells()->InsertNextCell(nv);
  return ret;
}

template<class GRID>
inline typename CellVTK<GRID>::VertexOnCellIterator
CellVTK<GRID>::FirstVertex() const { return VertexOnCellIterator(*this); }

template<class GRID>
inline typename CellVTK<GRID>::VertexOnCellIterator
CellVTK<GRID>::EndVertex()  const { return VertexOnCellIterator(*this, NumOfVertices()); }


} // namespace vtk_ugrid

template<int D>
struct grid_types<vtk_ugrid::UGridVTKAdapter<D> > 
  : public grid_types_base<vtk_ugrid::grid_types_UGridVTKAdapter<D> > 
{};


#define gt typename grid_types<vtk_ugrid::UGridVTKAdapter<D> >

 
  template<int D> inline gt::VertexIterator   gral_begin(gt::grid_type const& G, gt::VertexIterator) { return G.FirstVertex();}
  template<int D> inline gt::VertexIterator   gral_end  (gt::grid_type const& G, gt::VertexIterator) { return G.EndVertex();}
  template<int D> inline gt::size_type        gral_size (gt::grid_type const& G, gt::VertexIterator) { return G.NumOfVertices();}

  template<int D> inline gt::CellIterator     gral_begin(gt::grid_type const& G, gt::CellIterator)   { return G.FirstCell();}
  template<int D> inline gt::CellIterator     gral_end  (gt::grid_type const& G, gt::CellIterator)   { return G.EndCell();}
  template<int D> inline gt::size_type        gral_size (gt::grid_type const& G, gt::CellIterator)   { return G.NumOfCells();}

  //  template<int D> inline gt::VertexOnCellIterator   gral_begin(gt::Cell   a, gt::VertexOnCellIterator) { return a.FirstVertex();}
  //  template<int D> inline gt::VertexOnCellIterator   gral_end  (gt::Cell   a, gt::VertexOnCellIterator) { return a.EndVertex();}
  //  template<int D> inline gt::size_type              gral_size (gt::Cell   a, gt::VertexOnCellIterator) { return a.NumOfVertices();}

  template<int D> 
  inline vtk_ugrid::VertexOnCellIteratorVTK<vtk_ugrid::UGridVTKAdapter<D> >  
  gral_begin(vtk_ugrid::CellVTK                <vtk_ugrid::UGridVTKAdapter<D> >  a, 
	     vtk_ugrid::VertexOnCellIteratorVTK<vtk_ugrid::UGridVTKAdapter<D> > ) { return a.FirstVertex();}
 
  template<int D> 
  inline vtk_ugrid::VertexOnCellIteratorVTK<vtk_ugrid::UGridVTKAdapter<D> >  
  gral_end  (vtk_ugrid::CellVTK                <vtk_ugrid::UGridVTKAdapter<D> >  a, 
	     vtk_ugrid::VertexOnCellIteratorVTK<vtk_ugrid::UGridVTKAdapter<D> > ) { return a.EndVertex();}

  template<int D> 
  inline gt::size_type  
  gral_size (vtk_ugrid::CellVTK                <vtk_ugrid::UGridVTKAdapter<D> >  a, 
	     vtk_ugrid::VertexOnCellIteratorVTK<vtk_ugrid::UGridVTKAdapter<D> > ) { return a.NumOfVertices();}

#undef gt


template<int D>
struct element_traits<vtk_ugrid::VertexVTK<vtk_ugrid::UGridVTKAdapter<D> > >
  : public element_traits_vertex_base<vtk_ugrid::UGridVTKAdapter<D> >
{
  typedef element_traits_vertex_base<vtk_ugrid::UGridVTKAdapter<D> > base;
  struct hasher_type : public base::hasher_type_elem_base {
    size_t operator()(vtk_ugrid::VertexVTK<vtk_ugrid::UGridVTKAdapter<D> > const& v) const { return v.handle();}
  };

  //vertices are numbered consecutively
  typedef consecutive_integer_tag<0> consecutive_tag;
};

template<int D>
struct element_traits<vtk_ugrid::CellVTK<vtk_ugrid::UGridVTKAdapter<D> > >
  : public element_traits_vertex_base<vtk_ugrid::UGridVTKAdapter<D> >
{
  typedef element_traits_vertex_base<vtk_ugrid::UGridVTKAdapter<D> > base;
  struct hasher_type : public base::hasher_type_elem_base {
    size_t operator()(vtk_ugrid::CellVTK<vtk_ugrid::UGridVTKAdapter<D> > const& c) const { return c.handle();}
  };

  //cells are numbered consecutively
  typedef consecutive_integer_tag<0> consecutive_tag;
};



} // namespace GrAL

#include "Gral/Adapters/VTK/vtk-ugrid-adapter.tt.C"

#endif
