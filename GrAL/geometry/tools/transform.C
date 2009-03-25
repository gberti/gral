/*! \file */

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <Container/tuple-point-traits.h>
#include <Geometry/affine-mapping.h>
#include <Geometry/matrix.h>

#include <IO/control-device.h>

#include <iostream>


int main(int argc, char* argv[]) {
   using namespace std;
  
   typedef GrAL::tuple<double,3>                               coord_type;
   typedef GrAL::point_traits<coord_type>                      pt;
   typedef GrAL::algebraic_primitives<coord_type>              ap;
   typedef GrAL::matrix<3,3,0>                                 matrix_type;
   typedef GrAL::affine_mapping<matrix_type, coord_type>       mapping_type;
   

   GrAL::ControlDevice Ctrl = GrAL::GetCommandlineAndFileControlDevice(argc,argv,"","main");
   string h;
   h += "transform: Generate 3D affine transformation matrix (including translation part).\n";
   h += "Coefficients are read from stdin  and written to stdout.\n";
   h += "Typical usage: transform -id | transform <options> | transform <options> \n";
   h += "Options:\n";

   
   bool id = false;
   Ctrl.add("-id", GrAL::GetTrueOnReadMutator(id));
   h += "    -id (produce 3D identity)\n";

   double rot_x = 0.0;
   GrAL::RegisterAt(Ctrl, "-rx", rot_x);
   h += "    -rx <rot degrees> (rotation about x-axis, in degrees)\n";
   double rot_y = 0.0;
   GrAL::RegisterAt(Ctrl, "-ry", rot_y);
   h += "    -ry <rot degrees> (rotation about y-axis, in degrees)\n";
   double rot_z = 0.0;
   GrAL::RegisterAt(Ctrl, "-rz", rot_z);
   h += "    -rz <rot degrees> (rotation about z-axis, in degrees)\n";

   coord_type rot_axis(1,0,0);
   double     rot_angle(0.0);
   GrAL::RegisterAt(Ctrl, "-raxis", rot_axis);
   GrAL::RegisterAt(Ctrl, "-rangle", rot_angle);
   h += "    -raxis <x y z> -rangle <rot degrees> (rotation axis and angle)\n";

   coord_type rot_from(1,0,0);
   coord_type rot_to  (1,0,0);
   GrAL::RegisterAt(Ctrl, "-rot_from", rot_from);
   GrAL::RegisterAt(Ctrl, "-rot_to",   rot_to);
   h += "    -rot_from <direction from> -rot_to<direction to> \n";
   h += "           (rotate direction 'from' to 'to', args are normalised).\n"; 

   double sd(1.0);
   
   GrAL::RegisterAt(Ctrl, "-s",sd);
   h += "    -s <scaling=1.0>  (scaling factor)\n";

   coord_type s(1.0);
   GrAL::RegisterAt(Ctrl, "-S",  s);
   GrAL::RegisterAt(Ctrl, "-sx", pt::x(s));
   GrAL::RegisterAt(Ctrl, "-sy", pt::y(s));
   GrAL::RegisterAt(Ctrl, "-sz", pt::z(s));


   coord_type t(0.0);
   GrAL::RegisterAt(Ctrl, "-t", t);
   GrAL::RegisterAt(Ctrl, "-tx", pt::x(t));
   GrAL::RegisterAt(Ctrl, "-ty", pt::y(t));
   GrAL::RegisterAt(Ctrl, "-tz", pt::z(t));
   h += "    -t <tx ty tz> | -tx <tx> -ty <ty> -tz <tz> (translation by (tx ty tz))\n";

   GrAL::AddHelp(Ctrl,h);
   Ctrl.update();

   s *= sd;

   mapping_type M_in = mapping_type::identity();
   /*
   if(id) {
     cout << M_in;
     return 0;
   }
   */

   if(!id) 
     if(! (cin >> M_in)) {
       std::cerr << "Failed reading transformation from stdin!\n";
       return 2;
     }
  
   typedef mapping_type mt;
   mt Rx = mt::rotation3d(coord_type(1,0,0), rot_x*M_PI/180.0);
   mt Ry = mt::rotation3d(coord_type(0,1,0), rot_y*M_PI/180.0);
   mt Rz = mt::rotation3d(coord_type(0,0,1), rot_z*M_PI/180.0);
   mt Ra = mt::rotation3d(rot_axis, rot_angle);
   mt R_from_to = mt::rotate_to(rot_from, rot_to);
   mt R  = R_from_to(Ra(Rz(Ry(Rx))));
   
   mt S  = mt::scaling(coord_type(s));
   mt T  = mt::translation(t);
   mt M_out = T(S(R(M_in)));

   cout.precision(16);
   cout << M_out;

   return 0;
}
