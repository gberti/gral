#ifndef NMWR_GB_GRAPHICS_MANIP_H
#define NMWR_GB_GRAPHICS_MANIP_H

///////////////////////////////////////////////////
//
//  Manipulators for Graphics Devices.
//  Modeled after iostream-manipulators.
//
///////////////////////////////////////////////////


typedef void (*DevManip0)(rendering_language&);


template<class arg>
class DevManip1{
    arg x;
    void (*f)(const arg&,rendering_language&);
public:
    DevManip1(const arg& y, void (*g)(const arg&, rendering_language&)){x=y;f=g;}
    void apply(rendering_language& L) const { (*f)(x,L);}
};


// DevManip0
inline void BeginBlock(rendering_language& L) { L.begin_block();}
inline void EndBlock(rendering_language& L) { L.end_block();}
inline void EndGroup(rendering_language& L) { L.end_group();}
inline void EndAttribute(rendering_language& L) { L.end_attribute();}
inline void EndTransformation(rendering_language& L) { L.end_transformation();}


// DevManip1

inline void begin_group(const string& name, rendering_language& L)
 { L.begin_group(name);}

inline DevManip1<string>
BeginGroup(const string& name)
{ return (DevManip1<string>(name, &begin_group)); }

inline void begin_attribute(const Attribute& A, rendering_language& L)
 { L.begin_attribute(A);}
inline DevManip1<Attribute> 
BeginAttribute(const Attribute& A)
{ return (DevManip1<Attribute>(A, begin_attribute)); }


inline void begin_transformation(const Transformation& T, rendering_language& L)
 { L.begin_transformation(T);}
inline DevManip1<Transformation> 
BeginTransformation(const Transformation& T)
{ return (DevManip1<Transformation>(T, &begin_transformation)); }


inline void pause_for(const double& sec, rendering_language& L)
{ L.pause(sec);}
inline DevManip1<double> 
Pause(const double& sec)
{  return (DevManip1<double>(sec,&pause_for));}

inline void geom_from_file(const string& file, rendering_language& L)
 { L.read_from_file(file);}
inline DevManip1<string> GeomFromFile(const string& file)
{ return (DevManip1<string>(file, &geom_from_file)); }

/*
inline void begin_group(const string& group_name, rendering_language& L)
{ L.begin_group(group_name);}
inline DevManip1<string> BeginGroup(const string& group_name)
{ return DevManip1<string>(group_name,begin_group);}
*/

#endif
