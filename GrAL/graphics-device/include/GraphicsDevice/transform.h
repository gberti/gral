#include <math.h>

Transformation Transform(const point& b_l, const point& b_r, const point& t_l)
{

Transformation TF1,TF2,TF3,TF4,TF5;

  //3 Rotationen (-90 "uber 0 bis 90 grad)
  TF2=Rotation3D(point(1,0,0),atan2((b_l[3]-t_l[3]),-(b_l[2]-t_l[2])));
  TF3=Rotation3D(point(0,1,0),atan((b_l[3]-b_r[3])*1.0/(b_l[1]-b_r[1])));
  TF4=Rotation3D(point(0,0,1),atan2((b_r[2]-b_l[2]),(b_r[1]-b_l[1])));
  TF5=Identity3D;

  //3 Rotationen (90 "uber 180 bis 0 grad)
  if (b_l[2]-t_l[2]>0)
    TF2=Rotation3D(point(1,0,0),atan2((b_l[3]-t_l[3]),-(b_l[2]-t_l[2])));

  if (b_l[1]-b_r[1]>0){
    TF3=Rotation3D(point(0,1,0),atan2((b_l[3]-b_r[3]),-(b_l[1]-b_r[1])));
  }

  if (b_r[1]-b_l[1]<0)
    TF4=Rotation3D(point(0,0,1),atan2((b_r[2]-b_l[2]),-(b_r[1]-b_l[1])));
  //3-mal nicht definierte Rotation bereinigen

  //Check difference between 90 grad rotations
  if ((b_l[1]-b_r[1]==0) && (b_l[1]-t_l[1]!=0)) 
    {TF3=Rotation3D(point(0,1,0),atan2((b_r[3]-b_l[3]),(b_r[1]-b_l[1])));
    if ((t_l[1]-b_l[1]>0) && (b_r[2]==t_l[2]))
      {TF5=Rotation3D(point(0,0,1),M_PI/2);cerr << "DONE1\n";}
    if ((t_l[1]-b_l[1]<0) && (b_r[2]==t_l[2]))
      {TF5=Rotation3D(point(0,0,1),-M_PI/2);cerr << "DONE1\n";}
    }
  if ((b_l[1]-b_r[1]==0) && (b_l[1]-t_l[1]==0))
    if (b_l[2]!=b_r[2]) TF3=Identity3D;

  TF1=Translate(point(b_l[1],b_l[2],b_l[3]));



return TF1(TF5(TF4(TF3(TF2))));
}

