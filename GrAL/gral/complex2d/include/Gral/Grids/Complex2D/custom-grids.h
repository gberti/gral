#ifndef GRAL_GB_COMPLEX2D_CUSTOM_GRIDS_H
#define GRAL_GB_COMPLEX2D_CUSTOM_GRIDS_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/construct.h"
//#include "Gral/Grids/Complex2D/stored-geometry.h"
#include "Gral/Geometries/simple-geometry.h"
#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"


namespace GrAL {

  /*! \brief Some ready-made simple grids

     Useful as archetypes for 3D meshes.

  */
  class custom_grids {

    typedef Complex2D                 grid_type;
    typedef tuple<double, 3>          coord_type;
    //    typedef stored_geometry_complex2D geom_type;
    typedef simple_geometry<grid_type, coord_type> geom_type;

    static grid_type simplex3d;
    static grid_type pyramid;
    static grid_type prism;
    static grid_type cube;
    static geom_type simplex3d_geom;
    static geom_type pyramid_geom;
    static geom_type prism_geom;
    static geom_type cube_geom;

    static void init();
    static void init_simplex3d();
    static void init_pyramid();
    static void init_prism();
    static void init_cube();
  public:
    /*! \brief Synomym for Tetrahedron()
     */
    static grid_type const& Simplex3D()       { init();  return simplex3d;}
    static geom_type const& Simplex3DGeom()   { init(); return simplex3d_geom;}

    /*! \brief A combinatorial tetrahedron
     */
    static grid_type const& Tetrahedron()     {  init(); return simplex3d;}
    static geom_type const& TetrahedronGeom() {  init(); return simplex3d_geom;}


    /*! \brief A combinatorial prism

     \code
                    4 .
                   /'\ = .
                 // ' \   = .
               / /  '  \     =
              3-/---'---\-----2
             / /    |    \   / 
            //            \ /  
           0---------------1   


     \endcode
    */
    static grid_type const& Pyramid()         {  init(); return pyramid;}
    static geom_type const& PyramidGeom()     {  init(); return pyramid_geom;}


    /*! \brief A combinatorial prism

     \code
            5
           /'\
          / ' \
         /  '  \
        /   '   \
       3---------4
       |    '    |
       |    2    |
       |   / \   |
       |  /   \  |
       | /     \ |
       |/       \|
       0---------1
    \endcode

    */
    static grid_type const& Prism()           {  init(); return prism;}
    static geom_type const& PrismGeom()       {  init(); return prism_geom;}
  
    /*! \brief A combinatorial cube
          
        \code
              7--------------6
             /'             /|
            / '            / |
            4-------------5  |
            | '           |  |
            | 3-----------|--2
            |/            | /
            |             |/
            0-------------1
           
        \endcode
    */
      static grid_type const& Cube()            {  init(); return cube; }
      static geom_type const& CubeGeom()        {  init(); return cube_geom;}
  };

  
};


#endif
