#ifndef GRAL_GB_CARTESIANND_DIRECTION_MAPS_H
#define GRAL_GB_CARTESIANND_DIRECTION_MAPS_H

// $LICENSE_NEC

#include "Container/tuple.h"
#include "Container/index-map-nd.h"

#include <vector>

namespace GrAL {

namespace cartesiannd {

  //------------------------ maps of direction tuples etc. ------------------

  // map index tuples of directions of k-dimensional elements to linear indices and vice versa.
  // E.g. in 3D, facets have direction tuples (0,1), (0,2), and (1,2), which are mapped to 0, 1, 2. 
  template<unsigned DIM> 
  struct delta_map {
    typedef tuple<int, DIM>       index_type;
    typedef  std::vector<unsigned> vector_system;

   
    struct direction_table {
      std::vector<vector_system> num2vec;

      direction_table() {}
      direction_table(unsigned k) { init(k); }

      void init(unsigned k);

      vector_system      & operator[](unsigned m)       { cv(m); return num2vec[m];}
      vector_system const& operator[](unsigned m) const { cv(m); return num2vec[m];}
      vector_system const& operator()(unsigned m) const { cv(m); return num2vec[m];}
      unsigned size() const { return num2vec.size();}

      bool valid(unsigned m) const { return m < num2vec.size();}
      void cv   (unsigned m) const { REQUIRE(valid(m), "m=" << m << " num2vec.size()= " << num2vec.size(), 1);}

      void print( std::ostream& out) const {
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

      void  print( std::ostream& out) const { out << "d=" << dir << " o=" << offset; }
    };



    struct incidences {
      std::vector< std::vector<inc_descriptor> > nb;
      unsigned K_;
      unsigned m_;

      std::vector<inc_descriptor> const& operator()(unsigned dir_k) const { cv(dir_k);  return nb[dir_k];}
      std::vector<inc_descriptor> const& operator[](unsigned dir_k) const { cv(dir_k);  return nb[dir_k];}
      std::vector<inc_descriptor>      & operator[](unsigned dir_k)       { cv(dir_k);  return nb[dir_k];}

      void cv(unsigned dir_k) const { REQUIRE( (0<= dir_k && dir_k < nb.size()), 
					       "nb.size()=" << nb.size() << " dir_k=" << dir_k, 1);}

      void init(unsigned K, unsigned m);
      void init_vertex_on_vertex();
      void init_cell_on_cell(); 
      void init_upward  (unsigned K, unsigned m);
      void init_downward(unsigned K, unsigned m);

      void print( std::ostream& out) const; 
    };



    struct incidence_table {
       std::vector<incidences> inc;
      int k;
      incidence_table() : k(-1) {}
      // K = dimension of anchor
      void init(unsigned K);
      incidences      & operator[](int m)        { cv(m); return inc[m];}
      incidences const& operator[](int m)  const { cv(m); return inc[m];}
      void print( std::ostream& out) const;

      void cv(int m) const { REQUIRE(0 <= m && m < (int) inc.size(), " m=" << m,1);}
    };
    
    // centralized holder for static data
    struct SD {
      SD() {} 
      direction_table dirs[DIM+1];
      incidence_table incs[DIM+1];
    };

    static void init()        { if (!initialized()) init_();}
    static bool initialized() {  return (0 != sd);} 

    static void init_() {
      sd = new SD();
      init_dirs();
      init_incidences();
    }

    static void init_dirs() {
      for(unsigned k = 0; k <= DIM; ++k)
	sd->dirs[k].init(k);
    }
    static void init_incidences() {
      for(unsigned k = 0; k <= DIM; ++k)
	sd->incs[k].init(k);
    }

    static unsigned num_of_directions(unsigned k) { cv_k(k);  return sd->dirs[k].size();}

 
    static void cv_k(unsigned k)             { REQUIRE(0 <= k && k <= DIM, "k=" << k, 1);}
    static void cv_m(unsigned k, unsigned m) { cv_k(k); REQUIRE( m < sd->dirs[k].size(), "", 1);;}

    static vector_system const& num2vec(unsigned k, unsigned m)
    { cv_m(k,m); return sd->dirs[k][m];}

    static unsigned  vec2num    (unsigned k, vector_system const& i)    { return vec2num_rec(k,i,DIM);}
    static unsigned  vec2num_rec(unsigned k, vector_system const& i, unsigned d);

    template<class INDEXTYPE>
    static INDEXTYPE delta2index(vector_system const& i) { 
      INDEXTYPE x(0);
      for(unsigned j = 0; j < i.size(); ++j)
	x[i[j]] = 1;
      return x;
    }
    /*! \brief Offset of direction \c m in dimension \c k
       
        The returned index is the offset of the high vertex of the vertex range
        of an element of dimension \c k with direction \c m.
    */
    template<class INDEXTYPE>
    static INDEXTYPE num2index(unsigned k, unsigned m) { 
      return delta2index<INDEXTYPE>(num2vec(k, m));
    }


    static void print     ( std::ostream& out);
    static void print_maps( std::ostream& out);
    static void print_incs( std::ostream& out);
    static void selfcheck();

    // Data 
    static SD *            sd;
  }; // struct delta_map<DIM> 


  // static data 
  template<unsigned DIM>
  typename delta_map<DIM>::SD *     delta_map<DIM>::sd = 0;



  /*! \internal
      \brief Table to hold index maps for cartesian grids
 
      Provides mappings between nd-indices  and zero-based linear
      numbering for a fixed dimension \c k and each direction \c m.
      The mapping to global linear numbering (sequentializing all directions \c m)
      is done by offse_map.

      This is non-static data  which is specific to each grid.
  */
  template<unsigned DIM> 
  struct map_table {
    typedef index_map_nd<DIM> index_map_type;
    // FIXME: should come from grid / common source
    typedef typename index_map_type::index_type vertex_index_type;

    map_table() {}
    //! Initialize for dimension \c k, and grid dimensions [low,high]
    map_table(unsigned k, vertex_index_type low, vertex_index_type high) { init(k,low,high);}
    void init(unsigned k, vertex_index_type low, vertex_index_type high) 
    {
      maps.resize(binomial_coeff(DIM,k));
      for(unsigned m = 0; m < maps.size(); ++m) {
	// delta gives the sum of unit vectors which span the space 
	// of elements of dimension k and direction m: 
	// In these directions, there is one element less.
	vertex_index_type delta = delta_map<DIM>::template num2index<vertex_index_type>(k, m);
	maps[m] = index_map_type(low, high - delta);
      }
    }

    std::vector<index_map_type>  maps;
    unsigned num_of_maps()       const { return maps.size();}
    unsigned num_of_directions() const { return maps.size();}
    
    //! map for dimension \c m
    index_map_type      & operator[](int m)       { return maps[m];}
    index_map_type const& operator[](int m) const { return maps[m];}
    index_map_type const& operator()(int m) const { return maps[m];}
    
    void print( std::ostream& out) const { 
      for(unsigned m = 0; m < maps.size(); ++m)
	out << "[" << maps[m].max_tuple() << "] ";
    }
  }; // class map_table<DIM>



  /*! \internal
      \brief Offsets for Cartesian grid incidence iteration

      Offsets for the zero-based conversion between nd-indices + directions 
      and linear element handles provided by map_table.

      This is non-static data which is specific to each grid.
   */
  template<unsigned DIM> 
  struct offset_table {
    offset_table() {}

    //! Initialize for dimension \c k
    offset_table(unsigned k) { init(k);}
    void init(unsigned k) { 
      offsets.resize(binomial_coeff(DIM,k) +1);
    }
     std::vector<unsigned>  offsets;
    //! Offset of direction \c m
    unsigned& operator[](unsigned m)       { cv(m); return offsets[m];}
    unsigned  operator[](unsigned m) const { cv(m); return offsets[m];}
    unsigned  operator()(unsigned m) const { cv(m); return offsets[m];}
    unsigned  size()   const { return offsets.size();}
    unsigned  beyond() const { return offsets.back();}
    
    void print( std::ostream& out) const { 
      for(unsigned m = 0; m < offsets.size(); ++m)
	out << offsets[m] << ' ';
    }
    void cv(unsigned m) const { REQUIRE( m < offsets.size(), " m=" << m,1);}
  }; // class offset_table<DIM>



 



 //-------------------------  template function definitions ------------------

  template<unsigned DIM>
  void delta_map<DIM>::direction_table::init(unsigned k) 
  {
    // std::cout << "direction_table::init(" << k << ") ";
    REQUIRE_ALWAYS( (k <= DIM), "k=" << k << " DIM=" << DIM, 1);
    num2vec.resize(binomial_coeff(DIM,k));
    // std::cout << ": " << num2vec.size() << "direction" <<  std::endl;
    vector_system v(k,0);
    for(unsigned j = 0; j < k; ++j)
      v[j] = j;
    num2vec[0] = v;
    for(unsigned m = 1; m < num2vec.size(); ++m)
      num2vec[m] = succ_ordered(num2vec[m-1], DIM-1);
  }
  
  template<unsigned DIM>
  void delta_map<DIM>::incidences::print( std::ostream& out) const
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

  template<unsigned DIM>
  void delta_map<DIM>::incidences::init(unsigned K, unsigned m)
  {
    K_ = K;
    m_ = m;
    if(K > m) init_downward(K,m);
    if(K < m) init_upward  (K,m);
    if(K == m) {
      if(K == 0)   init_vertex_on_vertex();
      if(K == DIM) init_cell_on_cell();
    }
  }



  template<unsigned DIM>
  void delta_map<DIM>::incidences::init_downward(unsigned K, unsigned m)
  {
    nb.resize(sd->dirs[K].size());
    // for all directions of anchor elements of dim. K
    for(unsigned d_k = 0; d_k < sd->dirs[K].size(); ++d_k) {
      vector_system dir_k = num2vec(K, d_k);

      //--- loop over all  directions dir_km \subset dir_k ---
      // will have \choice{K,m} m-subdirections dir_km of dir_k,
      // and for each 2^{K-m} parallel elements of dim m
      unsigned p2 = (unsigned)pow(2.0,(int)(K-m));
      unsigned num_directions = binomial_coeff(K,m);
      nb[d_k].resize(p2*num_directions);
      int nb_cnt = 0;

      // ddir_km traverses all ordered sequences of length m in [0, K-1],
      // to be mapped to a m-subdirection dir_m of dir_k

      // initialize ddir_km
      vector_system ddir_km(m,0);
      for(unsigned d_m = 0; d_m < m; ++d_m)
	ddir_km[d_m] = d_m;

      // begin loop over all  directions dir_m \subset dir_k: 
      // \choice{K,m} of them
      for(unsigned dd_km = 0; dd_km < num_directions; ++dd_km) { 
	// map ddir_km to subsequence dir_km of dir_k
	vector_system dir_km(m); // m out of the k dirs of dir_k
	for(unsigned d_m = 0; d_m < m; ++d_m)
	  dir_km[d_m] = dir_k[ddir_km[d_m]];
	    
	vector_system dir_km_complement;
	set_difference(dir_k.begin(), dir_k.end(), dir_km.begin(), dir_km.end(),
		       back_inserter(dir_km_complement));
	REQUIRE_ALWAYS(dir_km_complement.size() == K-m, "K-m="<< K-m << " size()=" << dir_km_complement.size(), 1);

	// loop over  all 2^{K-m} vectors 'eps'  \in {0,1}^{K-m} 
	vector_system eps(K-m,0);
	for(unsigned n = 0; n < pow(2.0,(int)(K-m)); ++n, ++nb_cnt) {
	  // get offset from dir_m_k and eps 
	  index_type offset(0);
	  for(unsigned i = 0; i < dir_km_complement.size(); ++i)
	    offset[dir_km_complement[i]] = eps[i];
	  nb[d_k][nb_cnt] = inc_descriptor(offset, vec2num(m,dir_km));
	  eps = succ_binary_number(eps);
	}
	    
	ddir_km = succ_ordered(ddir_km, K-1);
      }
    }
  }

  
  template<unsigned DIM>
  void delta_map<DIM>::incidences::init_upward(unsigned K, unsigned m)
  {
    nb.resize(sd->dirs[K].size());
    // for all directions of anchor elements of dim. K
    for(unsigned d_k = 0; d_k < sd->dirs[K].size(); ++d_k) {
      vector_system dir_k = num2vec(K, d_k); 

      //--- loop over all  directions dir_km \superset dir_k ---
      // We have \choice{DIM-K,m-K} m-superdirections dir_km of dir_k,
      // and for each dir_km there are 2^{m-K} parallel elements of dim m
      unsigned p2 = (unsigned)pow(2.0,(int)(m-K));
      unsigned num_directions = binomial_coeff(DIM-K,m-K);
      nb[d_k].resize(p2*num_directions);
      int nb_cnt = 0;
 
      // dir_km_compl traverses all ordered sequences of length m-K in [0, DIM-K-1],
      // to be mapped to a m-superdirection dir_km of dir_k

      // initialize dir_km_compl to {0, ... , m-K-1}
      vector_system dir_km_compl(m-K,0);
      for(unsigned d_m = 0; d_m < m-K; ++d_m)
	dir_km_compl[d_m] = d_m;

      for(unsigned dd_km = 0; dd_km < num_directions; ++dd_km) {
	//--- map dir_km_compl to supersequence dir_km of dir_k
	vector_system dir_km(m); // m dirs, containing the k dirs of dir_k

        // map dir_km_compl from  [0,DIM-K-1] to [0, DIM-1] \ dir_k
	vector_system dir_km_compl_mapped(dir_km_compl);
	int nk = 0;
	for(int i = 0; i < (int)(m-K); ++i) {
	  while(nk < (int)dir_k.size() && dir_k[nk] <= dir_km_compl[i] + nk) {
	    ++nk;
	  }
	  dir_km_compl_mapped[i] = dir_km_compl[i] + nk;
	}
	// combine dir_km_compl and dir_k into dir_km
	std::merge(dir_km_compl_mapped.begin(), dir_km_compl_mapped.end(),
		   dir_k.begin(),               dir_k.end(),
		   dir_km.begin());

	// loop over  all 2^{m-K} vectors 'eps'  \in {0,1}^{m-K} 
	vector_system eps(m-K,0);
	for(unsigned n = 0; n < pow(2.0,(int)(m-K)); ++n, ++nb_cnt) {
	  // get offset from dir_m_k and eps 
	  index_type offset(0);
	  for(unsigned i = 0; i < dir_km_compl_mapped.size(); ++i)
	    offset[dir_km_compl_mapped[i]] = -eps[i];
	  nb[d_k][nb_cnt] = inc_descriptor(offset, vec2num(m,dir_km));
  	  eps = succ_binary_number(eps);
	}
	dir_km_compl = succ_ordered(dir_km_compl, DIM-K-1);
      }
    }
  }
  

  template<unsigned DIM>
  void delta_map<DIM>::incidences::init_cell_on_cell() 
  {
    nb.resize(sd->dirs[DIM].size());
    nb[0].resize(2*DIM);
    for(unsigned dir = 1; dir <= DIM; ++dir) {
      index_type offset(0);
      offset[dir-1] =  1;
      nb[0][2*dir-2] = inc_descriptor(offset,0);
      offset[dir-1] = -1;
      nb[0][2*dir-1] = inc_descriptor(offset,0);
    }
  }

  template<unsigned DIM>
  void delta_map<DIM>::incidences::init_vertex_on_vertex()
  {
    nb.resize(sd->dirs[0].size());
    nb[0].resize(2*DIM);
    for(unsigned dir = 1; dir <= DIM; ++dir) {
      index_type offset(0);
      offset[dir-1] =  1;
      nb[0][2*dir-2] = inc_descriptor(offset,0);
      offset[dir-1] = -1;
      nb[0][2*dir-1] = inc_descriptor(offset,0);
    }
  }



  template<unsigned DIM>
  void delta_map<DIM>::incidence_table::init(unsigned K) 
  {
    k = K; 
    inc.resize(DIM+1);
    for(unsigned m = 0; m < DIM+1; ++m)
      inc[m].init(K,m); // m-on-K-iterator
  }

  template<unsigned DIM>
  void delta_map<DIM>::incidence_table::print( std::ostream& out) const
  {
    out << "dim(anchor) = " << k << ":\n";
    for(unsigned m = 0; m <= DIM; ++m) {
      out << "    ";
      inc[m].print(out);
      out << "\n";
    }
    out << "\n";
  }

  template<unsigned DIM>
  unsigned delta_map<DIM>::vec2num_rec(unsigned k, delta_map<DIM>::vector_system const& i, unsigned d)
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
      return sum  + vec2num_rec(k-1,tail,d-(i[0]+1));
    }
  }


  template<unsigned DIM>
  void delta_map<DIM>::print( std::ostream& out) 
  {
    print_maps(out);
    print_incs(out);
  }

  template<unsigned DIM>
  void delta_map<DIM>::print_maps( std::ostream& out) {
      for(unsigned k = 0; k <= DIM; ++k) {
	out << "dirs[" << k << "]: ";
	sd->dirs[k].print(out);
	out << "\n";
      }
    }

  template<unsigned DIM>
  void delta_map<DIM>::print_incs( std::ostream& out) 
  {
    for(unsigned k = 0; k <= DIM; ++k) {
      out << "incs[" << k << "]: ";
      sd->incs[k].print(out);
      out << "\n";
    }
  }


  template<unsigned DIM>
  void delta_map<DIM>::selfcheck() 
  {
    for(unsigned k = 0; k <= DIM; ++k) 
      for(unsigned m = 0; m < num_of_directions(k); ++m) 
	REQUIRE_ALWAYS( m == vec2num(k, num2vec(k, m)), 
			"m=" << m << " k=" << k << " vec2num(...)=" << vec2num(k, num2vec(k, m)), 1);
  }


} // namespace cartesiannd 

} // namespace GrAL 

#endif
