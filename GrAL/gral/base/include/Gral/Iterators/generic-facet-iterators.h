#ifndef GRAL_BASE_GB_GENERIC_FACET_ITERATORS_H
#define GRAL_BASE_GB_GENERIC_FACET_ITERATORS_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Container/my-hash-map.h"

#include <vector>
#include <algorithm> // sort
#include <iostream> 

namespace generic_facet {

  template<class gt> class facet;
  
  template<class gt> class facet_iterator;
  
  template<class gt> class facet_on_cell_iterator;

  template<class gt> class vertex_on_facet_iterator;

  template<class gt> class facet_handle_t;

  template<class gt> class hasher_facet;

  template<class gt> class vtuple;

  template<class gt> class cell_mixin;

  template<class gt>
  struct grid_types_facet : public gt
    {
      // these typedefs should be superflous!
      typedef typename gt::grid_type     grid_type;
      typedef typename gt::Cell          Cell;
      typedef typename gt::Vertex        Vertex;
      typedef typename gt::cell_handle   cell_handle;
      typedef typename gt::vertex_handle vertex_handle;
      typedef typename gt::CellIterator  CellIterator;
      

      typedef grid_types<typename gt::archetype_type> agt;
      typedef typename agt::CellIterator archCellIterator;
      typedef typename agt::VertexOnCellIterator archVertexOnCellIterator;
      typedef typename agt::cell_handle  arch_cell_handle;
      typedef typename agt::Cell         archCell;      

      typedef facet<gt>                    Facet;
      typedef facet_iterator<gt>           FacetIterator;
      typedef facet_on_cell_iterator<gt>   FacetOnCellIterator;
      typedef vertex_on_facet_iterator<gt> VertexOnFacetIterator;
      typedef facet_handle_t<gt>           facet_handle; 
      typedef hasher_facet<gt>             facet_hasher_type;

      typedef vtuple<grid_types_facet<gt> > vtuple_type;


      // typedef cell_mixin<gt>               facet_mixin_for_cell_type;
      typedef cell_mixin<gt>               cell_base_type;
    };




    template<class gt>
      class facet_handle_t : public grid_types_facet<gt> {
	typedef facet_handle_t<gt> self;
      public:
	cell_handle      c;
	arch_cell_handle lh;
	facet_handle_t() {}
	facet_handle_t(cell_handle cc, arch_cell_handle llh)
	  : c(cc), lh(llh) {}

        bool operator==(self const& rhs) const { return (c == rhs.c && lh == rhs.lh);}
	bool operator!=(self const& rhs) const 
	  { return !((*this) == rhs);}
      };
    template<class gt>
    std::ostream& operator<<(std::ostream& out, facet_handle_t<gt> const& h)
      { return (out << h.c << ' ' << h.lh);}



  /*! \brief A generic iterator over the facets of cell.
    \ingroup iterators
    
    This class uses the cell archetype's CellIterator.
    
  */

    template<class gt>
      class facet_on_cell_iterator 
	: public grid_types_facet<gt> {
	
	typedef facet_on_cell_iterator<gt> self;
	friend class vertex_on_facet_iterator<gt>;
	friend class facet<gt>;
      private:
	Cell              c;
	archCellIterator  lf;
      public:
	facet_on_cell_iterator() {}
	explicit 
	facet_on_cell_iterator(Cell const& cc) 
	  : c(cc), lf(c.TheArchetype().FirstCell()) {}
	facet_on_cell_iterator(Cell const& cc, archCellIterator const& llf) 
	  : c(cc), lf(llf) {}
	facet_on_cell_iterator(grid_type const& g, facet_handle const& h)
	  : c(g,h.c), lf(c.TheArchetype(),h.lh) {}
	
	self& operator++() { c_(); ++lf; return *this;}
	bool  IsDone()  const { return lf.IsDone();}
	Facet  operator*() const { c_(); return Facet(*this); }
	
	arch_cell_handle local_handle() const { c_(); return lf.handle();}
	facet_handle  handle()       const 
	 { c_();  return facet_handle(c.handle(), local_handle()); }
	

	archCell         ArchetypeCell() const { c_(); return *lf;} 	
	grid_type const& TheGrid()   const { c_(); return c.TheGrid();}
	Cell      const& TheCell()   const { c_(); return c;}
	Cell      const& TheAnchor() const { c_(); return c;}
	
	
	bool operator==(self const& rhs) const { 
	  cb_(); rhs.cb_();
	  return lf == rhs.lf;
	}
	bool operator!=(self const& rhs) const 
	  { return !((*this) == rhs);}

	bool bound() const { return (c.valid());}
	bool valid() const { return (bound() && lf.valid());}
	void c_()  const { REQUIRE(valid(), "",1);}
	void cb_() const { REQUIRE(bound(), "",1);}
      };


    template<class gt>
    class facet : public grid_types_facet<gt> {
      typedef facet<gt> self;
      friend class vertex_on_facet_iterator<gt>;
    private:
      facet_on_cell_iterator<gt> fc;
    public:
      facet() {}
      facet(grid_type const& g, facet_handle const& h) : fc(g,h) {}
      facet(facet_on_cell_iterator<gt> const& ffc) : fc(ffc) {}
      
      VertexOnFacetIterator FirstVertex() const;
      VertexOnFacetIterator EndVertex()   const;
      unsigned NumOfVertices() const { c_(); return (*fc.lf).NumOfVertices();}

      grid_type  const& TheGrid() const { c_(); return fc.TheGrid();}
      facet_handle handle() const { c_(); return fc.handle();}

      bool operator==(self const& rhs) const {
	c_(); rhs.c_();
	return vtuple_type(*this) == vtuple_type(rhs);
      }
      bool operator!=(self const& rhs) const 
	{ return !((*this) == rhs);}

      bool bound() const { return fc.bound();}
      bool valid() const { return fc.valid();}
      void c_() const { REQUIRE(valid(), "" ,1); }
    };

   



    template<class gt>
      class vtuple {
	typedef vtuple<gt> self;
	typedef typename gt::vertex_handle         vertex_handle;
	typedef typename gt::VertexOnFacetIterator VertexOnFacetIterator;
	typedef typename gt::Facet                 Facet;
      private:
	std::vector<vertex_handle> v;
      public:
	vtuple(Facet const& f) 
	  { 
	    v.reserve(f.NumOfVertices());
	    for(VertexOnFacetIterator vf = f.FirstVertex(); ! vf.IsDone(); ++vf)
	      v.push_back(vf.handle());
	    std::sort(v.begin(), v.end());
	  }

	vertex_handle operator[](int n) const { check_range(n); return v[n];}

	// FIXME: lexicographical_compare_3way is not in std::
	// (only a SGI extension)
        bool operator==(self const& rhs) const { 
	  return (0 == std::lexicographical_compare_3way
		             (v    .begin(),    v.end(),
			      rhs.v.begin(),rhs.v.end()));
	}
	bool operator!=(self const& rhs) const 
	  { return !((*this) == rhs);}

        bool operator< (self const& rhs) const { 
	  return (0  >  std::lexicographical_compare_3way
		              (v    .begin(),    v.end(),
			       rhs.v.begin(),rhs.v.end()));
	}
      private:
	void check_range(unsigned i) const {
	  REQUIRE( ((0 <= i ) && (i < v.size())), 
		   " i = " << i << " must be in [0," << v.size()-1 << "]\n",1);
	  
	}
      };
   

    template<class gt>
      struct hasher_facet : public grid_types_facet<gt> {
	size_t operator()(Facet const& f) const 
	  { 
	    vtuple_type v(f);
            int n = f.NumOfVertices()-1; // v[n] > v[n-1] > v[n-2].
	    // if there are max 24 facets incident to any vertex,
	    // and max 8 facets incident ot any edge,
	    // the following is guaranteed to give a unique number.
	    return 24*v[n] + 8*v[n-1] + v[n-2];
	  }
      };


    template<class gt>
      class vertex_on_facet_iterator : public grid_types_facet<gt> {
         typedef vertex_on_facet_iterator<gt> self;
      private:
	 facet_on_cell_iterator<gt> f;
	 archVertexOnCellIterator  vf;
      public:
	vertex_on_facet_iterator() {}
	explicit
	vertex_on_facet_iterator(facet<gt> const& ff) 
	  : f(ff.fc), vf((*f.lf).FirstVertex()) {}
	vertex_on_facet_iterator(facet<gt> const& ff, 
				 archVertexOnCellIterator const& vff) 
	  : f(ff.fc), vf(vff) 
	  { REQUIRE( IsDone() || (f.ArchetypeCell() == vf.TheCell()),"",1);}
	
        self& operator++() { c_(); ++vf; return (*this);}
	Vertex operator*() const { c_(); return Vertex(TheGrid(), handle());}
	bool IsDone() const { cb_(); return vf.IsDone();}

	grid_type const& TheGrid() const { c_(); return f.TheGrid();}
        vertex_handle handle() const { c_(); return f.TheCell().v(vf.handle());}

	bool operator==(self const& rhs) const { 
	  cb_(); rhs.cb_();
	  return vf == rhs.vf;
	}
	bool operator!=(self const& rhs) const 
	  { return !((*this) == rhs);}


	bool bound() const { return (f.valid());}
	bool valid() const { return (bound() && vf.valid());}
	void c_()       const { REQUIRE(valid(), "", 1);}
	void cb_() const { REQUIRE(bound(), "", 1);}
	
      };



    /*! Facet iterator based on marking visited facets.

     */
    template<class gt>
    class facet_iterator : public grid_types_facet<gt> {
      typedef facet_iterator<gt> self;
    private:
      CellIterator               c;
      facet_on_cell_iterator<gt> e;
      // FIXME: not in std::
      std::hash_map<facet<gt>, bool, 
                    hasher_facet<gt>, equal_to<facet<gt> > > visited;
    public:
      facet_iterator() {}
      explicit 
      facet_iterator(grid_type const& g) 
	: c(g), e(c), visited(TheGrid().NumOfCells()*8)
	{
	  if (! IsDone()) visited[*e] = true;
	}
      
      self& operator++() { c_(); advance_till_valid(); return (*this);}
      Facet operator*() const { c_(); return Facet(e);}
      bool  IsDone()    const { cb_(); return c.IsDone();}

      grid_type const& TheGrid() const { cb_(); return c.TheGrid();}
      facet_handle handle() const { c_(); return e.handle();}

      bool operator==(self const& rhs) const 
	{ cb_(); return ((c == rhs.c) && (e == rhs.e)); }
      bool operator!=(self const& rhs) const 
	{ return !((*this) == rhs);}

      bool valid() const { return c.valid() && e.valid();}
      bool bound() const { return c.bound() && e.bound();}
      void c_()       const { REQUIRE(valid(), "",1); }    
      void cb_() const { REQUIRE(bound(), "",1); }    
    private:
      // one step forward
      void advance() {
	++e;
	if(e.IsDone()) {
	  ++c;
	  if(! c.IsDone()) {
	    e = facet_on_cell_iterator<gt>(c);
	  }
	}
      }
      // step forward until new (not yet visited) facet is found
      void advance_till_valid() {
	do {
	  advance();
	} while (! IsDone() && ! is_new_facet(*e));
	if( ! IsDone()) visited[*e] = true;
	
      }
      bool is_new_facet(facet<gt> const& e) const  
	{ return visited.find(e) == visited.end();}
    };



    //----- inline definitions ----------

    template<class gt>
      inline 
      typename facet<gt>::VertexOnFacetIterator 
      facet<gt>::FirstVertex() const 
      { c_(); return VertexOnFacetIterator(*this);}

    template<class gt>
      inline 
      typename facet<gt>::VertexOnFacetIterator 
      facet<gt>::EndVertex() const 
      { c_(); return VertexOnFacetIterator(*this, fc.ArchetypeCell().EndVertex());}


    // cell type in gt must derive from this type
    template<class gt>
      class cell_mixin 
      : //public grid_types_facet<gt>
          public gt::cell_base_type
      {
	typedef grid_types_facet<gt> gtf;
      public:
	typedef typename gtf::facet_handle      facet_handle;
	typedef typename gtf::Facet             Facet;
	typedef typename gtf::arch_cell_handle  arch_cell_handle;
	typedef typename gtf::archCell          archCell;
	typedef typename gtf::Cell              Cell;

	// map local (archetype) facets to global (grid) facets
	facet_handle_t<gt> f(arch_cell_handle const& h) const {
	  c_(); return facet_handle(handle(),h);
	}	
	facet_handle_t<gt> f(archCell const& af) const { c_(); return f(af.handle());}

	facet<gt> F(arch_cell_handle const& h) const 
	  { c_(); return Facet(TheGrid(),f(h));}
	facet<gt> F(archCell const& af) const { c_(); return F(af.handle());}
        
        unsigned NumOfFacets() const { c_(); return TheArchetype().NumOfCells();}
        facet_on_cell_iterator<gt> FirstFacet() const 
	  { c_(); return facet_on_cell_iterator<gt>(static_cast<Cell const&>(*this));}
        facet_on_cell_iterator<gt> EndFacet() const 
	  { c_(); return facet_on_cell_iterator<gt>(static_cast<Cell const&>(*this),
						    TheArchetype().EndCell());}
      };

} // namespace generic_facet



#endif

