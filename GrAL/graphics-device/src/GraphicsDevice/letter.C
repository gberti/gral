

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <list>

#include "GraphicsDevice/letter.h"
#include "GraphicsDevice/segment.h"
#include "ctype.h"

namespace GrAL { 

typedef std::list<geom_segment> segment_list;


static void make_alist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.5,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0,0)+offset,coord_type(0,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0,0)+offset,coord_type(0.8,0.2,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.9,0.1,0)+offset));
}

static void make_blist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_clist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_dlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_elist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.5,0)+offset,coord_type(0.8,0.25,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.9,0.1,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.25,0)+offset,coord_type(0.8,0.25,0)+offset));
}

static void make_flist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.5,0,0)+offset,coord_type(0.5,0.8,0)+offset));
  l.push_back(geom_segment(coord_type(0.5,0.8,0)+offset,coord_type(0.7,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.35,0.6,0)+offset,coord_type(0.66,0.6,0)+offset));
}

static void make_glist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.7,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.7,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.7,-0.4,0)+offset,coord_type(0.7,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.7,-0.4,0)+offset,coord_type(0.5,-0.3,0)+offset));
  l.push_back(geom_segment(coord_type(0.9,0.1,0)+offset,coord_type(0.5,-0.3,0)+offset));
}

static void make_hlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.5,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_ilist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.38,0.6,0)+offset,coord_type(0.42,0.62,0)+offset));
}

static void make_jlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0.5,0)+offset,coord_type(0.4,-0.2,0)+offset));
  l.push_back(geom_segment(coord_type(0.38,0.6,0)+offset,coord_type(0.42,0.62,0)+offset));
  l.push_back(geom_segment(coord_type(0.3,0.5,0)+offset,coord_type(0.4,0.5,0)+offset));
}

static void make_klist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.3,0)+offset,coord_type(0.8,0.7,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.3,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_llist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.6,0,0)+offset));
}

static void make_mlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.4,0)+offset,coord_type(0.4,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.4,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_nlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.2,0,0)+offset,coord_type(0.2,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.2,0.4,0)+offset,coord_type(0.6,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0,0)+offset,coord_type(0.6,0.5,0)+offset));
}

static void make_olist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_plist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,-0.4,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_qlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,-0.4,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_rlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.5,0)+offset,coord_type(0.6,0.5,0)+offset));
}

static void make_slist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.25,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.25,0)+offset,coord_type(0.8,0.25,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.25,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0,0,0)+offset));
}

static void make_tlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0.2,0)+offset,coord_type(0.4,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.35,0.8,0)+offset,coord_type(0.65,0.8,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.2,0)+offset,coord_type(0.6,0,0)+offset));
}

static void make_ulist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.5,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_vlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_wlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.2,0,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.2,0,0)+offset,coord_type(0.4,0.4,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.4,0)+offset,coord_type(0.6,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_xlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_ylist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.4,0.25,0)+offset));
}

static void make_zzlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Alist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_Blist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,0.85,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.85,0)+offset,coord_type(0.8,0.65,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.65,0)+offset,coord_type(0,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.35,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.15,0)+offset,coord_type(0.8,0.35,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0.15,0)+offset));
}

static void make_Clist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Dlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.6,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,1,0)+offset,coord_type(0.8,0.8,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.8,0)+offset,coord_type(0.8,0.2,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0,0)+offset,coord_type(0.8,0.2,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.6,0,0)+offset));
}

static void make_Elist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_Flist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_Glist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_Hlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Ilist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,1,0)+offset));
}

static void make_Jlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0.3,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Klist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Llist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Mlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.4,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.5,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_Nlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Olist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_Plist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_Qlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.95,-0.15,0)+offset,coord_type(0.65,0.15,0)+offset));
}

static void make_Rlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_Slist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0,0,0)+offset));
}

static void make_Tlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_Ulist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,1,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_Vlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_Wlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.2,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.2,0,0)+offset,coord_type(0.4,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.5,0)+offset,coord_type(0.6,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.6,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_Xlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_Ylist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.4,0.5,0)+offset));
}

static void make_Zzlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_zerolist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.3,0,0)+offset,coord_type(0.5,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
}
static void make_onelist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.7,0,0)+offset,coord_type(0.8,1,0)+offset));
}
static void make_twolist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.5,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0.5,0)+offset,coord_type(0,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
}

static void make_trilist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
}
static void make_forlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.7,0)+offset));
}

static void make_fivlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_sixlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,0.5,0)+offset));
}

static void make_sevlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0.7,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_eitlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0,0)+offset,coord_type(0.8,0,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_ninlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,1,0)+offset,coord_type(0.8,1,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.5,0)+offset,coord_type(0.8,0.5,0)+offset));
  l.push_back(geom_segment(coord_type(0.8,0,0)+offset,coord_type(0.8,1,0)+offset));
}

static void make_s1list(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0,0.4,0)+offset,coord_type(0.8,0.4,0)+offset));
  l.push_back(geom_segment(coord_type(0,0.6,0)+offset,coord_type(0.8,0.6,0)+offset));
}

static void make_s0list(segment_list& ,RenderableGeom::coord_type const&) { }

static void make_dotlist(segment_list& l,RenderableGeom::coord_type const& offset)
{ 
  typedef RenderableGeom::coord_type coord_type;
  l.erase(l.begin(),l.end());
  l.push_back(geom_segment(coord_type(0.4,0,0)+offset,coord_type(0.4,0.05,0)+offset));
  l.push_back(geom_segment(coord_type(0.4,0.05,0)+offset,coord_type(0.45,0.05,0)+offset));
  l.push_back(geom_segment(coord_type(0.45,0.05,0)+offset,coord_type(0.45,0.,0)+offset));
  l.push_back(geom_segment(coord_type(0.45,0,0)+offset,coord_type(0.4,0,0)+offset));
}

static 
segment_list alist,blist,clist,d_list,elist,flist,glist,hlist,ilist,
                    jlist,klist,llist,mlist,nlist,olist,plist,qlist,rlist,
                    slist,tlist,ulist,vlist,wlist,xlist,ylist,zzlist,
                    Alist,Blist,Clist,D_list,Elist,Flist,Glist,Hlist,Ilist,
                    Jlist,Klist,Llist,Mlist,Nlist,Olist,Plist,Qlist,Rlist,
                    Slist,Tlist,Ulist,Vlist,Wlist,Xlist,Ylist,Zzlist,zerolist,
                    onelist,twolist,trilist,forlist,fivlist,sixlist,sevlist,
                    eitlist,ninlist,s1list,s0list,dotlist;
//alist.push_back(geom_segment(coord_type(0,0,0),coord_type(0,1,0)));
//alist.push_back(geomsegment());

//const RenderableGeom LetterA(new geom_segment_list(alist));
RenderableGeom Lettera(RenderableGeom::coord_type const& offset)
{
  make_alist(alist,offset);
  return RenderableGeom(new geom_segment_list(alist));
}

RenderableGeom Letterb(RenderableGeom::coord_type const& offset)
{
  make_blist(blist,offset);
  return RenderableGeom(new geom_segment_list(blist));
}

RenderableGeom Letterc(RenderableGeom::coord_type const& offset)
{
  make_clist(clist,offset);
  return RenderableGeom(new geom_segment_list(clist));
}

RenderableGeom Letterd(RenderableGeom::coord_type const& offset)
{
  make_dlist(d_list,offset);
  return RenderableGeom(new geom_segment_list(d_list));
}

RenderableGeom Lettere(RenderableGeom::coord_type const& offset)
{
  make_elist(elist,offset);
  return RenderableGeom(new geom_segment_list(elist));
}

RenderableGeom Letterf(RenderableGeom::coord_type const& offset)
{
  make_flist(flist,offset);
  return RenderableGeom(new geom_segment_list(flist));
}

RenderableGeom Letterg(RenderableGeom::coord_type const& offset)
{
  make_glist(glist,offset);
  return RenderableGeom(new geom_segment_list(glist));
}

RenderableGeom Letterh(RenderableGeom::coord_type const& offset)
{
  make_hlist(hlist,offset);
  return RenderableGeom(new geom_segment_list(hlist));
}

RenderableGeom Letteri(RenderableGeom::coord_type const& offset)
{
  make_ilist(ilist,offset);
  return RenderableGeom(new geom_segment_list(ilist));
}

RenderableGeom Letterj(RenderableGeom::coord_type const& offset)
{
  make_jlist(jlist,offset);
  return RenderableGeom(new geom_segment_list(jlist));
}

RenderableGeom Letterk(RenderableGeom::coord_type const& offset)
{
  make_klist(klist,offset);
  return RenderableGeom(new geom_segment_list(klist));
}

RenderableGeom Letterl(RenderableGeom::coord_type const& offset)
{
  make_llist(llist,offset);
  return RenderableGeom(new geom_segment_list(llist));
}

RenderableGeom Letterm(RenderableGeom::coord_type const& offset)
{
  make_mlist(mlist,offset);
  return RenderableGeom(new geom_segment_list(mlist));
}

RenderableGeom Lettern(RenderableGeom::coord_type const& offset)
{
  make_nlist(nlist,offset);
  return RenderableGeom(new geom_segment_list(nlist));
}

RenderableGeom Lettero(RenderableGeom::coord_type const& offset)
{
  make_olist(olist,offset);
  return RenderableGeom(new geom_segment_list(olist));
}

RenderableGeom Letterp(RenderableGeom::coord_type const& offset)
{
  make_plist(plist,offset);
  return RenderableGeom(new geom_segment_list(plist));
}

RenderableGeom Letterq(RenderableGeom::coord_type const& offset)
{
  make_qlist(qlist,offset);
  return RenderableGeom(new geom_segment_list(qlist));
}

RenderableGeom Letterr(RenderableGeom::coord_type const& offset)
{
  make_rlist(rlist,offset);
  return RenderableGeom(new geom_segment_list(rlist));
}

RenderableGeom Letters(RenderableGeom::coord_type const& offset)
{
  make_slist(slist,offset);
  return RenderableGeom(new geom_segment_list(slist));
}

RenderableGeom Lettert(RenderableGeom::coord_type const& offset)
{
  make_tlist(tlist,offset);
  return RenderableGeom(new geom_segment_list(tlist));
}

RenderableGeom Letteru(RenderableGeom::coord_type const& offset)
{
  make_ulist(ulist,offset);
  return RenderableGeom(new geom_segment_list(ulist));
}

RenderableGeom Letterv(RenderableGeom::coord_type const& offset)
{
  make_vlist(vlist,offset);
  return RenderableGeom(new geom_segment_list(vlist));
}

RenderableGeom Letterw(RenderableGeom::coord_type const& offset)
{
  make_wlist(wlist,offset);
  return RenderableGeom(new geom_segment_list(wlist));
}

RenderableGeom Letterx(RenderableGeom::coord_type const& offset)
{
  make_xlist(xlist,offset);
  return RenderableGeom(new geom_segment_list(xlist));
}

RenderableGeom Lettery(RenderableGeom::coord_type const& offset)
{
  make_ylist(ylist,offset);
  return RenderableGeom(new geom_segment_list(ylist));
}

RenderableGeom Letterzz(RenderableGeom::coord_type const& offset)
{
  make_zzlist(zzlist,offset);
  return RenderableGeom(new geom_segment_list(zzlist));
}


RenderableGeom LetterA(RenderableGeom::coord_type const& offset)
{
  make_Alist(Alist,offset);
  return RenderableGeom(new geom_segment_list(Alist));
}

RenderableGeom LetterB(RenderableGeom::coord_type const& offset)
{
  make_Blist(Blist,offset);
  return RenderableGeom(new geom_segment_list(Blist));
}

RenderableGeom LetterC(RenderableGeom::coord_type const& offset)
{
  make_Clist(Clist,offset);
  return RenderableGeom(new geom_segment_list(Clist));
}

RenderableGeom LetterD(RenderableGeom::coord_type const& offset)
{
  make_Dlist(D_list,offset);
  return RenderableGeom(new geom_segment_list(D_list));
}

RenderableGeom LetterE(RenderableGeom::coord_type const& offset)
{
  make_Elist(Elist,offset);
  return RenderableGeom(new geom_segment_list(Elist));
}

RenderableGeom LetterF(RenderableGeom::coord_type const& offset)
{
  make_Flist(Flist,offset);
  return RenderableGeom(new geom_segment_list(Flist));
}

RenderableGeom LetterG(RenderableGeom::coord_type const& offset)
{
  make_Glist(Glist,offset);
  return RenderableGeom(new geom_segment_list(Glist));
}

RenderableGeom LetterH(RenderableGeom::coord_type const& offset)
{
  make_Hlist(Hlist,offset);
  return RenderableGeom(new geom_segment_list(Hlist));
}

RenderableGeom LetterI(RenderableGeom::coord_type const& offset)
{
  make_Ilist(Ilist,offset);
  return RenderableGeom(new geom_segment_list(Ilist));
}

RenderableGeom LetterJ(RenderableGeom::coord_type const& offset)
{
  make_Jlist(Jlist,offset);
  return RenderableGeom(new geom_segment_list(Jlist));
}

RenderableGeom LetterK(RenderableGeom::coord_type const& offset)
{
  make_Klist(Klist,offset);
  return RenderableGeom(new geom_segment_list(Klist));
}

RenderableGeom LetterL(RenderableGeom::coord_type const& offset)
{
  make_Llist(Llist,offset);
  return RenderableGeom(new geom_segment_list(Llist));
}

RenderableGeom LetterM(RenderableGeom::coord_type const& offset)
{
  make_Mlist(Mlist,offset);
  return RenderableGeom(new geom_segment_list(Mlist));
}

RenderableGeom LetterN(RenderableGeom::coord_type const& offset)
{
  make_Nlist(Nlist,offset);
  return RenderableGeom(new geom_segment_list(Nlist));
}

RenderableGeom LetterO(RenderableGeom::coord_type const& offset)
{
  make_Olist(Olist,offset);
  return RenderableGeom(new geom_segment_list(Olist));
}

RenderableGeom LetterP(RenderableGeom::coord_type const& offset)
{
  make_Plist(Plist,offset);
  return RenderableGeom(new geom_segment_list(Plist));
}

RenderableGeom LetterQ(RenderableGeom::coord_type const& offset)
{
  make_Qlist(Qlist,offset);
  return RenderableGeom(new geom_segment_list(Qlist));
}

RenderableGeom LetterR(RenderableGeom::coord_type const& offset)
{
  make_Rlist(Rlist,offset);
  return RenderableGeom(new geom_segment_list(Rlist));
}

RenderableGeom LetterS(RenderableGeom::coord_type const& offset)
{
  make_Slist(Slist,offset);
  return RenderableGeom(new geom_segment_list(Slist));
}

RenderableGeom LetterT(RenderableGeom::coord_type const& offset)
{
  make_Tlist(Tlist,offset);
  return RenderableGeom(new geom_segment_list(Tlist));
}

RenderableGeom LetterU(RenderableGeom::coord_type const& offset)
{
  make_Ulist(Ulist,offset);
  return RenderableGeom(new geom_segment_list(Ulist));
}

RenderableGeom LetterV(RenderableGeom::coord_type const& offset)
{
  make_Vlist(Vlist,offset);
  return RenderableGeom(new geom_segment_list(Vlist));
}

RenderableGeom LetterW(RenderableGeom::coord_type const& offset)
{
  make_Wlist(Wlist,offset);
  return RenderableGeom(new geom_segment_list(Wlist));
}

RenderableGeom LetterX(RenderableGeom::coord_type const& offset)
{
  make_Xlist(Xlist,offset);
  return RenderableGeom(new geom_segment_list(Xlist));
}

RenderableGeom LetterY(RenderableGeom::coord_type const& offset)
{
  make_Ylist(Ylist,offset);
  return RenderableGeom(new geom_segment_list(Ylist));
}

RenderableGeom LetterZZ(RenderableGeom::coord_type const& offset)
{
  make_Zzlist(Zzlist,offset);
  return RenderableGeom(new geom_segment_list(Zzlist));
}

RenderableGeom Letterzero(RenderableGeom::coord_type const& offset)
{
  make_zerolist(zerolist,offset);
  return RenderableGeom(new geom_segment_list(zerolist));
}

RenderableGeom Letterone(RenderableGeom::coord_type const& offset)
{
  make_onelist(onelist,offset);
  return RenderableGeom(new geom_segment_list(onelist));
}

RenderableGeom Lettertwo(RenderableGeom::coord_type const& offset)
{
  make_twolist(twolist,offset);
  return RenderableGeom(new geom_segment_list(twolist));
}

RenderableGeom Lettertri(RenderableGeom::coord_type const& offset)
{
  make_trilist(trilist,offset);
  return RenderableGeom(new geom_segment_list(trilist));
}

RenderableGeom Letterfor(RenderableGeom::coord_type const& offset)
{
  make_forlist(forlist,offset);
  return RenderableGeom(new geom_segment_list(forlist));
}

RenderableGeom Letterfiv(RenderableGeom::coord_type const& offset)
{
  make_fivlist(fivlist,offset);
  return RenderableGeom(new geom_segment_list(fivlist));
}

RenderableGeom Lettersix(RenderableGeom::coord_type const& offset)
{
  make_sixlist(sixlist,offset);
  return RenderableGeom(new geom_segment_list(sixlist));
}

RenderableGeom Lettersev(RenderableGeom::coord_type const& offset)
{
  make_sevlist(sevlist,offset);
  return RenderableGeom(new geom_segment_list(sevlist));
}

RenderableGeom Lettereit(RenderableGeom::coord_type const& offset)
{
  make_eitlist(eitlist,offset);
  return RenderableGeom(new geom_segment_list(eitlist));
}

RenderableGeom Letternin(RenderableGeom::coord_type const& offset)
{
  make_ninlist(ninlist,offset);
  return RenderableGeom(new geom_segment_list(ninlist));
}

RenderableGeom SLetter1(RenderableGeom::coord_type const& offset)
{
  make_s1list(s1list,offset);
  return RenderableGeom(new geom_segment_list(s1list));
}

RenderableGeom SLetter0(RenderableGeom::coord_type const& offset)
{
  make_s0list(s0list,offset);
  return RenderableGeom(new geom_segment_list(s0list));
}

RenderableGeom LetterDot(RenderableGeom::coord_type const& offset)
{
  make_dotlist(dotlist,offset);
  return RenderableGeom(new geom_segment_list(dotlist));
}

RenderableGeom RLetter(char ch, RenderableGeom::coord_type const& offset)
{
  switch (ch)//toupper
  {
    case 'a' : return Lettera(offset);
    case 'b' : return Letterb(offset);
    case 'c' : return Letterc(offset);
    case 'd' : return Letterd(offset);
    case 'e' : return Lettere(offset);
    case 'f' : return Letterf(offset);
    case 'g' : return Letterg(offset);
    case 'h' : return Letterh(offset);
    case 'i' : return Letteri(offset);
    case 'j' : return Letterj(offset);
    case 'k' : return Letterk(offset);
    case 'l' : return Letterl(offset);
    case 'm' : return Letterm(offset);
    case 'n' : return Lettern(offset);
    case 'o' : return Lettero(offset);
    case 'p' : return Letterp(offset);
    case 'q' : return Letterq(offset);
    case 'r' : return Letterr(offset);
    case 's' : return Letters(offset);
    case 't' : return Lettert(offset);
    case 'u' : return Letteru(offset);
    case 'v' : return Letterv(offset);
    case 'w' : return Letterw(offset);
    case 'x' : return Letterx(offset);
    case 'y' : return Lettery(offset);
    case 'z' : return Letterzz(offset);
    case 'A' : return LetterA(offset);
    case 'B' : return LetterB(offset);
    case 'C' : return LetterC(offset);
    case 'D' : return LetterD(offset);
    case 'E' : return LetterE(offset);
    case 'F' : return LetterF(offset);
    case 'G' : return LetterG(offset);
    case 'H' : return LetterH(offset);
    case 'I' : return LetterI(offset);
    case 'J' : return LetterJ(offset);
    case 'K' : return LetterK(offset);
    case 'L' : return LetterL(offset);
    case 'M' : return LetterM(offset);
    case 'N' : return LetterN(offset);
    case 'O' : return LetterO(offset);
    case 'P' : return LetterP(offset);
    case 'Q' : return LetterQ(offset);
    case 'R' : return LetterR(offset);
    case 'S' : return LetterS(offset);
    case 'T' : return LetterT(offset);
    case 'U' : return LetterU(offset);
    case 'V' : return LetterV(offset);
    case 'W' : return LetterW(offset);
    case 'X' : return LetterX(offset);
    case 'Y' : return LetterY(offset);
    case 'Z' : return LetterZZ(offset);
    case '0' : return Letterzero(offset);
    case '1' : return Letterone(offset);
    case '2' : return Lettertwo(offset);
    case '3' : return Lettertri(offset);
    case '4' : return Letterfor(offset);
    case '5' : return Letterfiv(offset);
    case '6' : return Lettersix(offset);
    case '7' : return Lettersev(offset);
    case '8' : return Lettereit(offset);
    case '9' : return Letternin(offset);
    case '=' : return SLetter1(offset);
    case '.' : return LetterDot(offset);
    default  : return SLetter0(offset);
  }
  //  return SLetter0(offset); // never reached, but keeps xlC quiet.
}

} // namespace GrAL 
