#ifndef GRAL_GB_CARTESIANND_CARTESIANND_H
#define GRAL_GB_CARTESIANND_CARTESIANND_H

// $LICENSE_NEC

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Container/index-map-nd.h"
#include "Container/range.h"
#include "Container/combinatorial.h"

#include "Gral/Base/common-grid-basics.h"

#include <vector>
#include <iostream>

namespace cartesiannd {
  
  //-------------- forward declarations ---------------

  template<class CARTGRID, unsigned K>
  class element_handle_t;
  template<class CARTGRID, unsigned K>
  class sequence_iterator_t;
  template<unsigned DIM>
  class subrange;
  template<unsigned DIM>
  class grid;
  

  template<class GRID,   unsigned D, unsigned CD>  struct element_type 
  { typedef sequence_iterator_t<GRID, D> type; };
  template<unsigned DIM, unsigned D, unsigned CD>  struct element_type<subrange<DIM>, D, CD> 
  { typedef sequence_iterator_t<grid<DIM>, D> type; };


  
  template<class CARTGRID, unsigned DIM>
  struct grid_types_base
  {
    typedef element_handle_t   <CARTGRID, 0>                vertex_handle;
    typedef sequence_iterator_t<CARTGRID, 0>                VertexIterator;
    typedef typename element_type<CARTGRID, 0, DIM>::type   Vertex;

    typedef element_handle_t   <CARTGRID, 1>                edge_handle;
    typedef sequence_iterator_t<CARTGRID, 1>                EdgeIterator;
    typedef typename element_type<CARTGRID, 1, DIM-1>::type Edge;

    typedef element_handle_t   <CARTGRID, 2>                face_handle;
    typedef sequence_iterator_t<CARTGRID, 2>                FaceIterator;
    typedef typename element_type<CARTGRID, 2, DIM-2>::type Face;

    typedef element_handle_t   <CARTGRID, DIM-1>            facet_handle;
    typedef sequence_iterator_t<CARTGRID, DIM-1>            FacetIterator;
    typedef typename element_type<CARTGRID, DIM-1, 1>::type Facet;

    typedef element_handle_t   <CARTGRID, DIM>              cell_handle;
    typedef sequence_iterator_t<CARTGRID, DIM>              CellIterator;
    typedef typename element_type<CARTGRID, DIM, 0>::type   Cell;


    typedef tuple<int, DIM> vertex_index_type;
    typedef tuple<int, DIM> cell_index_type;
    typedef tuple<int, DIM> index_type;
  }; // struct grid_types_base<unsigned DIM>
 


  //--------------------- element handles -------------------

  template<class CARTGRID, unsigned K>
  class element_handle_t {
    typedef element_handle_t<CARTGRID,K> self;
    int h_;
  public:
    element_handle_t() {}
    explicit element_handle_t(int hh) : h_(hh) {}
    int h() const { return h_;}
    operator int&()       { return h_;}
    self& operator++() {++h_; return *this;}
    //operator int () const { return h;}
  };

  template<class CARTGRID, unsigned K>
  inline bool operator==(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() == rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator!=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() != rhs.h();}
  template<class CARTGRID, unsigned K>
  inline std::ostream& operator<<(std::ostream& out, element_handle_t<CARTGRID,K> lhs)
  { return (out << lhs.h());}
  template<class CARTGRID, unsigned K>
  inline std::istream& operator>>(std::istream& in,  element_handle_t<CARTGRID,K> lhs)
  { return (in >> lhs.h());}


  //------------------------ maps of direction tuples etc. ------------------


  // map index tuples of directions of k-dimensional elements to linear indices and vice versa.
  // E.g. in 3D, facets have direction tuples (0,1), (0,2), and (1,2), which are mapped to 0, 1, 2. 
  template<unsigned DIM> 
  struct delta_map {

    typedef std::vector<unsigned> vector_system;

    struct direction_table {
      std::vector<vector_system> num2vec;

      direction_table() {}
      direction_table(unsigned k) { init(k); }

      void init(unsigned k) {
	//std::cout << "direction_table::init(" << k << ") ";
	REQUIRE_ALWAYS( (k <= DIM), "k=" << k << " DIM=" << DIM, 1);
	num2vec.resize(binomial_coeff(DIM,k));
	//std::cout << ": " << num2vec.size() << "direction" << std::endl;
	vector_system v(k,0);
	for(unsigned j = 1; j < k; ++j)
	  v[j] = j;
	num2vec[0] = v;
	for(unsigned m = 1; m < num2vec.size(); ++m)
	  num2vec[m] = succ(num2vec[m-1]);
      }

      vector_system      & operator[](unsigned m)       { cv(m); return num2vec[m];}
      vector_system const& operator[](unsigned m) const { cv(m); return num2vec[m];}
      vector_system const& operator()(unsigned m) const { cv(m); return num2vec[m];}
      unsigned size() const { return num2vec.size();}

      bool valid(unsigned m) const { return m < num2vec.size();}
      void cv   (unsigned m) const { REQUIRE(valid(m), "m=" << m << " num2vec.size()= " << num2vec.size(), 1);}

      // successor of an ordered k-tuple of indices in [0, DIM-1]
      // if there is no successor, v is returned
      // This could go e.g. to Container/combinatorics.h
      static vector_system succ(vector_system v) 
      {
	vector_system res(v);
	unsigned k = v.size();
	if(res[k-1] < DIM-1)
	  res[k-1]++;
	else {
	  for(unsigned j=k-2; j >= 0; --j) {
	    if(res[j] < res[j+1]-1) {
	      res[j]++;
	      for(unsigned jj = j+1; jj < k; ++jj)
		res[jj] = res[jj-1]+1;
	      break;
	    }
	  }
	}
	return res;
      }
      void print(std::ostream& out) const {
	for(unsigned m = 0; m < num2vec.size(); ++m)
	  out << "[" << range(num2vec[m].begin(), num2vec[m].end()) << "] ";
      }
    };
    
    struct initializer {
      initializer() { std::cout << "initializer" << std::endl;  delta_map<DIM>::init_dirs();}
    };

    static void init_dirs() {
      for(unsigned k = 0; k <= DIM; ++k)
	dirs[k].init(k);
    }

    static unsigned num_of_directions(unsigned k) { return dirs[k].size();}

    static vector_system num2vec(unsigned k, unsigned m)
    { REQUIRE( m < dirs[k].size(), "", 1); return dirs[k][m];}

    static unsigned      vec2num(unsigned k, vector_system i)
    { return vec2num_rec(k,i,DIM);}
    static unsigned      vec2num_rec(unsigned k, vector_system i, unsigned d)
    {
      if(i.size() == 0)
	return 0;
      else {
	vector_system tail(i.begin()+1, i.end());
	for(unsigned j = 0; j < tail.size(); ++j)
	  tail[j] -= (i[0]+1);

	unsigned sum = 0;
	for(unsigned j = 0; j < i[0]; ++j)
	  sum += binomial_coeff(d-j-1, k-1);
	/*
	std::cout << "vec2num(" << k << ", [" << range(i.begin(), i.end()) << "], " << d << ")" 
		  << " : sum= " << sum << " tail=" << "[" << range(tail.begin(), tail.end()) << "] " << std::endl;
	*/
	return sum  + vec2num_rec(k-1,tail,d-(i[0]+1));
      }
    }

    template<class INDEXTYPE>
    static INDEXTYPE delta2index(vector_system i) { 
      INDEXTYPE x(0);
      for(unsigned j = 0; j < i.size(); ++j)
	x[i[j]] = 1;
      return x;
    }

    template<class INDEXTYPE>
    static INDEXTYPE num2index(unsigned k, unsigned m) { 
      /*
      std::cout << "delta_map<>::num2index(" << k << ',' << m << ") : ";
      vector_system i = num2vec(k,m);
      std::cout << "i=" << range(i.begin(), i.end());
      INDEXTYPE res = delta2index<INDEXTYPE>(i);
      std::cout << " res=" << res << "\n";
      */
      return delta2index<INDEXTYPE>(num2vec(k, m));
    }


    static void print(std::ostream& out) {
      for(unsigned k = 0; k <= DIM; ++k) {
	out << "dirs[" << k << "]: ";
	dirs[k].print(out);
	out << "\n";
      }
    }
    static void selfcheck() {
      for(unsigned k = 0; k <= DIM; ++k) 
	for(unsigned m = 0; m < num_of_directions(k); ++m) 
	  REQUIRE_ALWAYS( m == vec2num(k, num2vec(k, m)), 
			  "m=" << m << " k=" << k << " vec2num(...)=" << vec2num(k, num2vec(k, m)), 1);
    }

    static direction_table dirs[DIM+1];
    static initializer the_initializer;
  }; // struct delta_map<DIM> 

  template<unsigned DIM>
  typename delta_map<DIM>::direction_table  delta_map<DIM>::dirs[DIM+1];

  template<unsigned DIM>
  typename delta_map<DIM>::initializer      delta_map<DIM>::the_initializer;


  template<unsigned DIM> 
  struct map_table {
    typedef index_map_nd<DIM> index_map_type;
    // FIXME: should come from grid / common source
    typedef typename index_map_type::index_type vertex_index_type;

    map_table() {}
    map_table(unsigned k, vertex_index_type low, vertex_index_type high) { init(k,low,high);}
    void init(unsigned k, vertex_index_type low, vertex_index_type high) 
    {
      maps.resize(binomial_coeff(DIM,k));
      for(unsigned m = 0; m < maps.size(); ++m) {
	vertex_index_type delta = delta_map<DIM>::template num2index<vertex_index_type>(k, m);
	// std::cout << "map_table::init(" << k << "): delta=" << delta << "\n";
	//maps[m] = index_map_type(low, high - delta);
	maps[m] = index_map_type(high - delta);
      }
    }

    std::vector<index_map_type>  maps;
    unsigned num_of_maps() const { return maps.size();}
  
    index_map_type      & operator[](int m)       { return maps[m];}
    index_map_type const& operator[](int m) const { return maps[m];}
    index_map_type const& operator()(int m) const { return maps[m];}
    
    void print(std::ostream& out) const { 
      for(unsigned m = 0; m < maps.size(); ++m)
	out << "[" << maps[m].max_tuple() << "] ";
    }
  };


  template<unsigned DIM> 
  struct offset_table {
    offset_table() {}
    offset_table(unsigned k) { init(k);}
    void init(unsigned k) { 
      offsets.resize(binomial_coeff(DIM,k) +1);
    }
    std::vector<unsigned>  offsets;
    unsigned& operator[](unsigned m)       { return offsets[m];}
    unsigned  operator[](unsigned m) const { return offsets[m];}
    unsigned  operator()(unsigned m) const { return offsets[m];}
    
    void print(std::ostream& out) const { 
      for(unsigned m = 0; m < offsets.size(); ++m)
	out << offsets[m] << ' ';
    }
  };



  //---------------------------- grid types ----------------------------


  template<class CARTGRID, unsigned DIM>
  class grid_base : public grid_types_base<CARTGRID, DIM> {
    typedef grid_types_base<CARTGRID, DIM> base;
  public:
    typedef typename base::vertex_index_type vertex_index_type;
    typedef typename base::VertexIterator    VertexIterator;
    typedef typename base::EdgeIterator      EdgeIterator;
    typedef typename base::FaceIterator      FaceIterator;
    typedef typename base::FacetIterator     FacetIterator;
    typedef typename base::CellIterator      CellIterator;
    enum { dimension = DIM };
    // protected:
  public:
    map_table<DIM>    maps   [DIM+1]; // [0, ..., DIM]
    offset_table<DIM> offsets[DIM+1];

    void init(vertex_index_type low, vertex_index_type beyond);
  public:
    unsigned NumOfVertices() const { return NumOfElements(0);}
    unsigned NumOfEdges()    const { return NumOfElements(1);}
    unsigned NumOfFaces()    const { return NumOfElements(2);}
    unsigned NumOfFacets()   const { return NumOfElements(DIM-1);}
    unsigned NumOfCells()    const { return NumOfElements(DIM);}

    template<unsigned K>
    unsigned NumOfElements()           const { cv(K); return offsets[K][maps[K].num_of_maps()];}
    unsigned NumOfElements(unsigned k) const { cv(k); return offsets[k][maps[k].num_of_maps()];}
    

    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> FirstElement() const;
    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> EndElement()   const;

    VertexIterator FirstVertex()  const { return FirstElement<0>();}
    EdgeIterator   FirstEdge()    const { return FirstElement<1>();}
    FaceIterator   FirstFace()    const { return FirstElement<2>();}
    FacetIterator  FirstFacet()   const { return FirstElement<DIM-1>();}
    CellIterator   FirstCell()    const { return FirstElement<DIM>();}

    VertexIterator EndVertex()    const { return EndElement<0>();}
    EdgeIterator   EndEdge()      const { return EndElement<1>();}
    FaceIterator   EndFace()      const { return EndElement<2>();}
    FacetIterator  EndFacet()     const { return EndElement<DIM-1>();}
    CellIterator   EndCell()      const { return EndElement<DIM>();}

    CARTGRID      & to_derived()       { return static_cast<CARTGRID      &>(*this);}
    CARTGRID const& to_derived() const { return static_cast<CARTGRID const&>(*this);}


    bool valid(unsigned k) const { return k <= DIM;}
    void cv   (unsigned k) const { REQUIRE(valid(k), "k=" << k << " DIM=" << DIM, 1);}

    void print(std::ostream& out) 
    {
      //  out << "directions:\n";
      //  delta_map<DIM>::print(out);
      for(unsigned k = 0; k <= DIM; ++k) {
	out << "maps   [" << k << "]: ";
	maps[k].print(out); 
	out << "\n";
	out << "offsets[" << k << "]: "; 
	offsets[k].print(out); 
	out << "\n";
      }
    } 
  }; // class grid_base<unsigned DIM>


  template<unsigned DIM>
  class subrange : public grid_base<subrange<DIM>, DIM> {
    typedef grid_base<subrange<DIM>, DIM> base;
  public:
    typedef grid<DIM> grid_type;
    typedef grid<DIM> base_grid_type;

    typedef typename base::vertex_index_type vertex_index_type;
  private:
    ref_ptr<grid_type const> g;
  public:
    subrange() {}
    subrange(grid_type const&         gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond); }
    subrange(ref_ptr<grid_type const> gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond); }

    ref_ptr<grid_type const> BaseGrid() const { return g;}

  }; // class subrange<unsigned DIM>


  /*! \brief An N-dimensional Cartesian grid

     \todo Incidence iterators
     \todo Archetypes
     \todo switch operators
  */
  template<unsigned DIM>
  class grid : public grid_base<grid<DIM>, DIM> {
    typedef grid_base<grid<DIM>, DIM> base;
    typedef grid<DIM>                 self;

  public:
    typedef grid<DIM>                        base_grid_type;
    typedef typename base::vertex_index_type vertex_index_type;

    grid(vertex_index_type beyond) 
    { init(vertex_index_type(0), beyond);}
    grid(vertex_index_type low, vertex_index_type high)
    { init(low, high+index_type(1));}

    self const& BaseGrid() const { return *this;}
    
  }; // class grid<unsigned DIM>






  template<class CARTGRID, unsigned K>
  class sequence_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dimension> {
    typedef CARTGRID                                       grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dimension> base;
    typedef sequence_iterator_t<CARTGRID, K>               self;
  public:
    typedef element_handle_t<CARTGRID, K>    element_handle;
    typedef typename base::index_type        index_type;

    //    typedef self                             value_type;
    typedef typename element_type<grid_type, K, grid_type::dimension - K>::type value_type;
  private:
    ref_ptr<grid_type const> g;
    element_handle           h;
    unsigned                 m; // index into the choice of basis vectors, \in [0, \choice{D, K}[

  public:
    sequence_iterator_t() {}
    sequence_iterator_t(grid_type         const& gg) : g(gg), h(0), m(0) {}
    sequence_iterator_t(ref_ptr<grid_type const> gg) : g(gg), h(0), m(0) {}
    sequence_iterator_t(grid_type         const& gg, element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(ref_ptr<grid_type const> gg, element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(grid_type         const& gg, element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}
    sequence_iterator_t(ref_ptr<grid_type const> gg, element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}

    self&       operator++()       { cv(); ++h; if(! valid_map()) ++m; return *this; }
    bool        IsDone()     const { cb(); return m >= g->maps[K].num_of_maps();}

    // FIXME: this does not work for subranges!!
    value_type  operator*()  const { cv(); return value_type(g->BaseGrid(), handle(), m);}

    //! \c handle() return  unique element handle
    element_handle     handle() const { cb(); return h;}

    //! for \f$ K \neq 0, D \f$, \c index() is not unique!
    index_type index()  const { cb(); return g->maps[K][m](h - g->offsets[K][m]);}

    ref_ptr<grid_type const> TheGrid() const { return g;}

    bool bound() const { return g != 0;}
    bool valid() const { return bound() && ! IsDone();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}

    // find appropriate m by looking up the correct interval of h in offsets[K];
    void init_m() { m = 0; while(!valid_map()) ++m;}
    // is h a handle for the current map m?
    bool valid_map() const { return (unsigned)h.h() < g->offsets[K][m+1];}

  }; // class sequence_iterator_t<class CARTGRID, unsigned K>  

  
 template<class CARTGRID, unsigned K>
 inline bool operator==(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return lhs.handle() == rhs.handle();} 

 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return !(lhs==rhs);}
 

  //-----------------------  inline functions  ----------------------------
 

  template<class CARTGRID, unsigned DIM>
  template<unsigned K>
  sequence_iterator_t<CARTGRID, K>
  grid_base<CARTGRID,DIM>::FirstElement() const { return sequence_iterator_t<CARTGRID, K>(to_derived());}

  template<class CARTGRID, unsigned DIM>
  template<unsigned K>
  sequence_iterator_t<CARTGRID, K>
  grid_base<CARTGRID,DIM>::EndElement() const { 
    return sequence_iterator_t<CARTGRID, K>(to_derived(), 
					    element_handle_t<CARTGRID,K>(offsets[k][maps[k].num_of_maps()]), 
					    maps[k].num_of_maps());
  }


  template<class CARTGRID, unsigned DIM>
  void grid_base<CARTGRID,DIM>::init(typename grid_base<CARTGRID,DIM>::vertex_index_type low,
				     typename grid_base<CARTGRID,DIM>::vertex_index_type beyond) 
  {
    std::cout << "grid_base<CARTGRID,DIM>::init(" << "[" << low << "], [" << beyond << "[)\n";
    for(unsigned k = 0; k <= dimension; ++k) {
      maps   [k].init(k, low, beyond);
      offsets[k].init(k);
      offsets[k][0] = 0;
      for(unsigned m = 1; m <= maps[k].num_of_maps(); ++m)
	offsets[k][m] = offsets[k][m-1] + maps[k][m-1].flat_size();
    }
  }

}; // namespace cartesiannd


template<unsigned DIM>
struct grid_types<cartesiannd::grid<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::grid<DIM>, DIM> >
{};


template<unsigned DIM>
struct grid_types<cartesiannd::subrange<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::subrange<DIM>, DIM> >
{};


#endif
