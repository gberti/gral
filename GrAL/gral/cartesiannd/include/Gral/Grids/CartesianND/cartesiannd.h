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

#include <cmath>



namespace cartesiannd {
  
  //-------------- forward declarations ---------------

  template<class CARTGRID, unsigned K>
  class element_handle_t;
  template<class CARTGRID, unsigned K>
  class sequence_iterator_t;
  template<class CARTGRID, unsigned K, unsigned M>
  class downward_inc_iterator_t;
  template<unsigned DIM>
  class subrange;
  template<unsigned DIM>
  class grid;
  

  template<class GRID,   unsigned D, unsigned CD>  struct element_type 
  { typedef sequence_iterator_t<GRID, D> type; };
  template<unsigned DIM, unsigned D, unsigned CD>  struct element_type<subrange<DIM>, D, CD> 
  { typedef sequence_iterator_t<grid<DIM>, D> type; };

  template<class CARTGRID, unsigned K> struct element_handle_type
  { typedef element_handle_t<CARTGRID, K>  type; };
  template<unsigned DIM, unsigned K> struct element_handle_type<subrange<DIM>, K>
  { typedef element_handle_t<grid<DIM>, K> type;};

  template<class CARTGRID, unsigned K, unsigned M>
  struct incidence_iterator_type 
  { typedef downward_inc_iterator_t<CARTGRID,K,M> type;};

  template<class CARTGRID, unsigned DIM>
  struct grid_types_base
  {
    enum { dimension = DIM};

    //  typedef element_handle_t   <CARTGRID, 0>                vertex_handle;
    typedef typename element_handle_type<CARTGRID, 0>::type vertex_handle;
    typedef sequence_iterator_t<CARTGRID, 0>                VertexIterator;
    typedef typename element_type<CARTGRID, 0, DIM>::type   Vertex;

    //    typedef element_handle_t   <CARTGRID, 1>                edge_handle;
    typedef typename element_handle_type<CARTGRID, 1>::type edge_handle;
    typedef sequence_iterator_t<CARTGRID, 1>                EdgeIterator;
    typedef typename element_type<CARTGRID, 1, DIM-1>::type Edge;

    // typedef element_handle_t   <CARTGRID, 2>                face_handle;
    typedef typename element_handle_type<CARTGRID, 2>::type face_handle;
    typedef sequence_iterator_t<CARTGRID, 2>                FaceIterator;
    typedef typename element_type<CARTGRID, 2, DIM-2>::type Face;

    // typedef element_handle_t   <CARTGRID, DIM-1>            facet_handle;
    typedef typename element_handle_type<CARTGRID, DIM-1>::type facet_handle;
    typedef sequence_iterator_t<CARTGRID, DIM-1>            FacetIterator;
    typedef typename element_type<CARTGRID, DIM-1, 1>::type Facet;

    // typedef element_handle_t   <CARTGRID, DIM>              cell_handle;
    typedef typename element_handle_type<CARTGRID, DIM>::type cell_handle;
    typedef sequence_iterator_t<CARTGRID, DIM>              CellIterator;
    typedef typename element_type<CARTGRID, DIM, 0>::type   Cell;

    typedef typename incidence_iterator_type<CARTGRID, DIM, 0>    ::type VertexOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 1>    ::type EdgeOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 2>    ::type FaceOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM-1>::type FacetOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM>  ::type CellOnCellIterator;

    typedef typename incidence_iterator_type<CARTGRID, 1, 0>      ::type VertexOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, 2>      ::type FaceOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, DIM-1>  ::type FacetOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, DIM>    ::type CellOnEdgeIterator;

    typedef typename incidence_iterator_type<CARTGRID, 2, 0>      ::type VertexOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, 1>      ::type EdgeOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, DIM-1>  ::type FacetOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, DIM>    ::type CellOnFaceIterator;

    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 0>  ::type VertexOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 1>  ::type EdgeOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 2>  ::type FaceOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, DIM>::type CellOnFacetIterator;

    typedef typename incidence_iterator_type<CARTGRID, DIM, 0>    ::type VertexOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 1>    ::type EdgeOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 2>    ::type FaceOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM-1>::type FacetOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM>  ::type CellOnCellIterator;



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
    typedef tuple<int, DIM>       index_type;
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
	  num2vec[m] = succ_ordered(num2vec[m-1], DIM-1);
      }

      vector_system      & operator[](unsigned m)       { cv(m); return num2vec[m];}
      vector_system const& operator[](unsigned m) const { cv(m); return num2vec[m];}
      vector_system const& operator()(unsigned m) const { cv(m); return num2vec[m];}
      unsigned size() const { return num2vec.size();}

      bool valid(unsigned m) const { return m < num2vec.size();}
      void cv   (unsigned m) const { REQUIRE(valid(m), "m=" << m << " num2vec.size()= " << num2vec.size(), 1);}

      void print(std::ostream& out) const {
	for(unsigned m = 0; m < num2vec.size(); ++m)
	  out << "[" << range(num2vec[m].begin(), num2vec[m].end()) << "] ";
      }
    }; // struct direction_table;

    // description of element relative to anchor element
    struct inc_descriptor {
      index_type offset; // offset of the base index of the element w/r to anchor
      unsigned   dir;    // index of the m-dimensional direction of the element
      inc_descriptor() {}
      inc_descriptor(index_type const& idx, unsigned d) : offset(idx), dir(d) {}

      void  print(std::ostream& out) const 
      {
	out << "d=" << dir << " o=" << offset;
      }
    };

    struct incidences {

      std::vector<std::vector<inc_descriptor> > nb;
      unsigned K_;
      unsigned m_;

      std::vector<inc_descriptor> const& operator()(unsigned dir_k) const { cv(dir_k);  return nb[dir_k];}
      std::vector<inc_descriptor> const& operator[](unsigned dir_k) const { cv(dir_k);  return nb[dir_k];}
      std::vector<inc_descriptor>      & operator[](unsigned dir_k)       { cv(dir_k);  return nb[dir_k];}

      void cv(unsigned dir_k) const { REQUIRE( (0<= dir_k && dir_k < nb.size()), 
					       "nb.size()=" << nb.size() << " dir_k=" << dir_k, 1);}

      void init(unsigned K, unsigned m) {
	K_ = K;
	m_ = m;
	if(K > m) init_downward(K,m);
	if(K < m) init_upward  (K,m);
	if(K == m) {
	  if( K == 0) init_vertex_on_vertex();
	  if(K == DIM) init_cell_on_cell();
	}
      }

      void print(std::ostream& out) const 
      {
	out << "m=" << m_ << ": ["; 
        for(unsigned dir_k = 0; dir_k != nb.size(); ++dir_k) {
	  out << "dir_k=" << dir_k << ": ";
	  for(unsigned dir_m = 0; dir_m != nb[dir_k].size(); ++dir_m) {
	    nb[dir_k][dir_m].print(out);
	    out << ";";
	  }
	  out << " | ";
	}
	out << "] ";
      }
      void init_vertex_on_vertex() {}
      void init_cell_on_cell() {}
      void init_upward  (unsigned K, unsigned m) {}

      void init_downward(unsigned K, unsigned m) {
	nb.resize(dirs[K].size());
	// for all directions of anchor elements of dim. K

	// std::cout << "init_downwards(K=" << K << ",m=" << m << ")" << std::endl;

	for(unsigned d_k = 0; d_k < dirs[K].size(); ++d_k) {
	  vector_system dir_k = num2vec(K, d_k);

	  // std::cout << "d_k=" << d_k << " <-> [" << range(dir_k.begin(), dir_k.end()) << "]" << std::endl;

	  //--- loop over all  directions dir_km \subset dir_k ---

	  // will have \choice{K,m} m-subdirections dir_km of dir_k,
	  //  and for each 2^{K-m} parallel elements of dim m
	  unsigned p2 = (unsigned)pow(2,K-m);
	  nb[d_k].resize(p2*binomial_coeff(K,m));

	  // ddir_km traverses all ordered sequences of length m in [0, K-1],
	  // to be mapped to a m-subdirection dir_m of dir_k
	  // initialize ddir_km
	  vector_system ddir_km(m,0);
	  for(unsigned d_m = 0; d_m < m; ++d_m)
	    ddir_km[d_m] = d_m;

	  int cnt = 0;
	  // begin loop over all  directions dir_m \subset dir_k: \choice{K,m} of them
	  for(unsigned dd_km = 0; dd_km < binomial_coeff(K,m); ++dd_km) { // nb[d_k].size(); ++dd_km) {
	    // map ddir_km to subsequence dir_km of dir_k
	    vector_system dir_km(m); // m out of the k dirs of dir_k
	    for(unsigned d_m = 0; d_m < m; ++d_m)
	      dir_km[d_m] = dir_k[ddir_km[d_m]];
	    
	    // std::cout << "  ddir_km= [" << range(ddir_km.begin(), ddir_km.end()) << "] "; 
	    // std::cout << "dir_km= [" << range(dir_km.begin(), dir_km.end()) << "] "; 

	    vector_system dir_km_complement;
	    set_difference(dir_k.begin(), dir_k.end(), dir_km.begin(), dir_km.end(),
			   back_inserter(dir_km_complement));
	    // std::cout << "dir_km_compl= [" << range(dir_km_complement.begin(), dir_km_complement.end()) << "] ";
	    REQUIRE_ALWAYS(dir_km_complement.size() == K-m, "K-m="<< K-m << " size()=" << dir_km_complement.size(), 1);
	    //  std::cout << endl;

	    // loop over  all 2^{K-m} vectors 'eps'  \in {0,1}^{K-m} 
	    vector_system eps(K-m,0);
	    for(unsigned n = 0; n < pow(2,K-m); ++n, ++cnt) {
	      // get offset from dir_m_k and eps 
	      index_type offset(0);
	      //std::cout << "    eps=[" << range(eps.begin(), eps.end()) << "] ";
	      for(unsigned i = 0; i < dir_km_complement.size(); ++i)
		offset[dir_km_complement[i]] = eps[i];
	      //std::cout << "descr: (o=" << offset << " d=" << vec2num(m,dir_km) << ") ";
	      nb[d_k][cnt] = inc_descriptor(offset, vec2num(m,dir_km));
	      eps = succ_binary_number(eps);
	      // std::cout << std::endl;
	    }
	    
	    ddir_km = succ_ordered(ddir_km, K-1);
	    // std::cout << endl;
	  }
	  // std::cout << std::endl;
	}
	// std::cout << std::endl;
      }
    };

    struct incidence_table {
      std::vector<incidences> inc;
      int k;
      incidence_table() : k(-1) {}
      // K = dimension of anchor
      void init(unsigned K) {
	k = K; 
	inc.resize(DIM+1);
	for(unsigned m = 0; m < DIM+1; ++m)
	  inc[m].init(K,m); // m-on-K-iterator
      }
      incidences      & operator[](int m)        { return inc[m];}
      incidences const& operator[](int m)  const { return inc[m];}
      void print(std::ostream& out) const
      {
	out << "dim(anchor) = " << k << ":\n";
	for(unsigned m = 0; m <= DIM; ++m) {
	  out << "    ";
	  inc[m].print(out);
          out << "\n";
	}
	out << "\n";
      }
    };
    
    struct initializer {
      initializer() { 
	std::cout << "initializer" << std::endl;  
	delta_map<DIM>::init();
      }
    };

    static void init() {
      init_dirs();
      init_incidences();
    }

    static void init_dirs() {
      for(unsigned k = 0; k <= DIM; ++k)
	dirs[k].init(k);
    }
    static void init_incidences() {
      for(unsigned k = 0; k <= DIM; ++k)
	incs[k].init(k);
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
      print_maps(out);
      print_incs(out);
    }
    static void print_maps(std::ostream& out) {
      for(unsigned k = 0; k <= DIM; ++k) {
	out << "dirs[" << k << "]: ";
	dirs[k].print(out);
	out << "\n";
      }
    }
    static void print_incs(std::ostream& out) 
    {
     for(unsigned k = 0; k <= DIM; ++k) {
	out << "incs[" << k << "]: ";
	incs[k].print(out);
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
    static incidence_table incs[DIM+1];
    static initializer the_initializer;
  }; // struct delta_map<DIM> 

  // ??? necessary??
  template<unsigned DIM>
  typename delta_map<DIM>::direction_table  delta_map<DIM>::dirs[DIM+1];

  template<unsigned DIM>
  typename delta_map<DIM>::incidence_table  delta_map<DIM>::incs[DIM+1];

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
	maps[m] = index_map_type(low, high - delta);
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
    typedef typename base::index_type        index_type;
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
    vertex_index_type vertex_low()    const { return maps[0][0].min_tuple();}
    vertex_index_type vertex_high()   const { return maps[0][0].max_tuple();}
    vertex_index_type vertex_beyond() const { return maps[0][0].beyond_tuple();}

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


    template<unsigned K>
    element_handle_t<CARTGRID, K> index2handle(index_type idx, unsigned dir) const 
    { return element_handle_t<CARTGRID, K>(maps[K][dir](idx) + offsets[K][dir]); }

    template<unsigned K>
    index_type handle2index(element_handle_t<CARTGRID, K> h, unsigned dir) const
    { return  (maps[K][dir](h - offsets[K][dir])); }


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


  //  template<class CARTGRID, unsigned DIM>
  template<unsigned DIM>
  class subrange : public grid_base<subrange<DIM>, DIM> {
    typedef grid_base<subrange<DIM>, DIM> base;
    typedef subrange<DIM>                 self;
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

    template<unsigned K>
    element_handle_t<base_grid_type, K>  base_handle(element_handle_t<self, K> h, unsigned dir) const 
    { return BaseGrid()->template index2handle<K>(handle2index(h,dir), dir);}


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

    ref_ptr<self const> BaseGrid() const { return ref_ptr<self const>(*this);}

    template<unsigned K>
    element_handle_t<self, K> base_handle(element_handle_t<self, K> h, unsigned dir) const { return h;}

  }; // class grid<unsigned DIM>






  template<class CARTGRID, unsigned K>
  class sequence_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dimension> {
    typedef CARTGRID                                       grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dimension> base;
    typedef sequence_iterator_t<CARTGRID, K>               self;
  public:
    enum { griddim = CARTGRID::dimension};
    typedef typename grid_type::base_grid_type  base_grid_type;
    typedef element_handle_t<base_grid_type,K>  element_handle;
    typedef element_handle_t<CARTGRID, K>       local_element_handle;

    typedef typename base::index_type        index_type;

    //    typedef self                             value_type;
    typedef typename element_type<grid_type, K, griddim - K>::type value_type;
  private:
    ref_ptr<grid_type const> g;
    local_element_handle     h;
    unsigned                 m; // index into the choice of basis vectors, \in [0, \choice{D, K}[

  public:
    sequence_iterator_t() {}
    explicit sequence_iterator_t(grid_type         const& gg) : g(gg), h(0) { init_m();} 
    explicit sequence_iterator_t(ref_ptr<grid_type const> gg) : g(gg), h(0) { init_m();} 
    sequence_iterator_t(grid_type         const& gg, local_element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(ref_ptr<grid_type const> gg, local_element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(grid_type         const& gg, local_element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}
    sequence_iterator_t(ref_ptr<grid_type const> gg, local_element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}


    template<class CARTSUBRANGE>
    sequence_iterator_t(sequence_iterator_t<CARTSUBRANGE, K> const& rhs) 
      : g(rhs.TheGrid()->BaseGrid()), 
	h(g->template index2handle<K>(rhs.index(),rhs.direction())),  m(rhs.direction())
    {}

    // initialization from subrange 
    // this will work only if element_handle and local_element_handle are the same.
    template<class CARTSUBRANGE>
    sequence_iterator_t(ref_ptr<CARTSUBRANGE const> rge, element_handle hh) : g(rge->BaseGrid()), h(hh) { init_m();}


    self&       operator++()       { cv(); ++h; if(! valid_map()) ++m; return *this; }
    bool        IsDone()     const { cb(); return m >= g->maps[K].num_of_maps();}

    // this will use the implicit copy-constructor if value_type == self,
    // and the templated copy constructor otherwise.
    value_type  operator*()  const { cv(); return value_type(*this); } 
    // value_type  operator*()  const { cv(); return value_type(g->BaseGrid(), handle(), m);}

    local_element_handle local_handle() const { cb(); return h;}
    //! \c handle() return  unique element handle with respect to base grid
    element_handle     handle()    const { cb(); return TheGrid()->base_handle(h,m);}
    unsigned           direction() const { cb(); return m;}

    //! for \f$ K \neq 0, D \f$, \c index() is not unique!
    index_type index()  const { cb(); return g->handle2index(h,m);} //  g->maps[K][m](h - g->offsets[K][m]);}

    ref_ptr<grid_type const> TheGrid() const { return g;}

    template<unsigned M>
    typename incidence_iterator_type<CARTGRID,K,M>::type FirstElement() const
    {  return typename incidence_iterator_type<CARTGRID,K,M>::type(*this);}

    typename incidence_iterator_type<CARTGRID,K,0>::type FirstVertex() const { return FirstElement<0>();}
    typename incidence_iterator_type<CARTGRID,K,1>::type FirstEdge  () const { return FirstElement<1>();}
    typename incidence_iterator_type<CARTGRID,K,2>::type FirstFace  () const { return FirstElement<2>();}
    typename incidence_iterator_type<CARTGRID,K,griddim-1>::type FirstFacet() const 
    { return FirstElement<griddim-1>();}
    typename incidence_iterator_type<CARTGRID,K,griddim>::type   FirstCell () const 
    { return FirstElement<griddim>();}



    // Warning: this works only if M < K! Else there might be incident elements outside the grid!
    template<unsigned M>
    unsigned NumOfElements() const { return delta_map<griddim>::incs[K][M][direction()].size();}

    unsigned NumOfVertices() const { return NumOfElements<0>();}    
    unsigned NumOfEdges   () const { return NumOfElements<1>();}    
    unsigned NumOfFaces   () const { return NumOfElements<2>();}    
    unsigned NumOfFacets  () const { return NumOfElements<griddim-1>();}    
    unsigned NumOfCells   () const { return NumOfElements<griddim>  ();}    



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
 { return lhs.local_handle() == rhs.local_handle();} 

#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return !(lhs==rhs);}
#endif


  //------------------------ incidence iterators -------------------------

  template<class CARTGRID, unsigned K, unsigned M>
  class downward_inc_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dimension> {
  private:
    typedef CARTGRID                                       grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dimension> base;
    typedef downward_inc_iterator_t<CARTGRID, K, M>        self;
  public:
    enum { dimension = base::dimension};
    typedef typename base::index_type                      index_type;
    typedef typename grid_type::base_grid_type             base_grid_type;
    typedef element_handle_t<base_grid_type,M>             element_handle;
    typedef typename element_type<base_grid_type, K, dimension-K>::type anchor_type;
    typedef typename element_type<base_grid_type, M, dimension-M>::type element_type;
    typedef element_type                                   value_type;
  private:
    //    ref_ptr<anchor_type const>  a;
    anchor_type a;

    unsigned                    loc;

  public:
    downward_inc_iterator_t() {}
    explicit downward_inc_iterator_t(anchor_type const&         aa, unsigned ll = 0) : a(aa), loc(ll) {}
    //explicit downward_inc_iterator_t(ref_ptr<anchor_type const> aa, unsigned ll = 0) : a(aa), loc(ll) {}


    self& operator++()  { cv(); ++loc; return *this;}
    element_type operator*() const { return element_type(TheGrid(), handle());}
    
    element_handle handle()   const { cv(); return TheGrid()->template index2handle<M>(index(), direction());}
    index_type index()        const { cv(); return a.index() + table()[loc].offset;}
    unsigned   direction()    const { cv(); return table()[loc].dir;}
    unsigned   local_handle() const { cb(); return loc;}

    bool IsDone() const { cb(); return loc >= table().size(); }
    
    ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    ref_ptr<grid_type   const> TheGrid()   const { return a.TheGrid();}

    bool bound() const { return a.valid();}
    bool valid() const { return bound() && ! IsDone();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  private:
    typedef delta_map<dimension>::inc_descriptor incidences;
    std::vector<incidences> table() const { return delta_map<dimension>::incs[K][M][a.direction()];}
  };


 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator==(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator!=(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return !(lhs==rhs);}
#endif



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
