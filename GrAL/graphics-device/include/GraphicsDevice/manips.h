#ifndef NMWR_GB_GRAPHICSDEVICE_GRAPHICS_MANIP_H
#define NMWR_GB_GRAPHICSDEVICE_GRAPHICS_MANIP_H

// $LICENSE

#include "GraphicsDevice/rendering-language.h"

/*! \defgroup renderermanips   Manipulators for GraphicsDevice s.
  \ingroup renderers

   Modeled after iostream-manipulators.
   Example:
   \code
   GraphicsDevice Dev;
   typedef RenderableGeom::coord_type coord_t;
   Dev << BeginGroup("Tree") 
       << BeginAttribute(Attribute::green()) << RSphere(1,coord_t(0,0,2)) << EndAttribute
       << BeginAttribute(Attribute::black()) << RCylinder(0.5, 1.2)       << EndAttribute
       << EndGroup;
   \endcode
   
*/

/*! \brief Manipulator without arguments
    \ingroup renderermanips 
    \todo
    Should be formulated with function objects (functor classes)
 */
typedef void (*DevManip0)(rendering_language&);


/*! \brief Manipulator with one argument
    \ingroup renderermanips 
 */
template<class arg>
class DevManip1{
    arg x;
    void (*f)(const arg&,rendering_language&);
public:
    DevManip1(const arg& y, void (*g)(const arg&, rendering_language&)) {x=y; f=g;}
    void apply(rendering_language& L) const { (*f)(x,L);}
};


/*! \defgroup renderermanip0 Renderer Manipulators without argument
 
    \ingroup renderermanips
*/

/*! \ingroup renderermanip0 */
inline void BeginBlock       (rendering_language& L) { L.begin_block();}

/*! \ingroup renderermanip0 
 */
inline void EndBlock         (rendering_language& L) { L.end_block();}

/*! \ingroup renderermanip0 */
inline void EndGroup         (rendering_language& L) { L.end_group();}

/*! \ingroup renderermanip0 */
inline void EndAttribute     (rendering_language& L) { L.end_attribute();}

/*! \ingroup renderermanip0 */
inline void EndTransformation(rendering_language& L) { L.end_transformation();}


/*! \defgroup renderermanip1 Renderer Manipulators with one argument

  \ingroup renderermanips
 */

/*! \ingroup renderermanip1 */
inline void begin_group(const std::string& name, rendering_language& L)
 { L.begin_group(name);}
/*! \ingroup renderermanip1 */
inline DevManip1<std::string>
BeginGroup(const std::string& name)
{ return (DevManip1<std::string>(name, &begin_group)); }

/*! \ingroup renderermanip1 */
inline void begin_attribute(const Attribute& A, rendering_language& L)
 { L.begin_attribute(A);}

/*! \ingroup renderermanip1 */
inline DevManip1<Attribute> 
BeginAttribute(const Attribute& A)
{ return (DevManip1<Attribute>(A, begin_attribute)); }


/*! \ingroup renderermanip1 */
inline void begin_transformation(const Transformation& T, rendering_language& L)
 { L.begin_transformation(T);}

/*! \ingroup renderermanip1 */
inline DevManip1<Transformation> 
BeginTransformation(const Transformation& T)
{ return (DevManip1<Transformation>(T, &begin_transformation)); }


/*! \ingroup renderermanip1 */
inline void pause_for(const double& sec, rendering_language& L)
{ L.pause(sec);}

/*! \ingroup renderermanip1 */
inline DevManip1<double> 
Pause(const double& sec)
{  return (DevManip1<double>(sec,&pause_for));}


/*! \ingroup renderermanip1 */
inline void geom_from_file(const std::string& file, rendering_language& L)
 { L.read_from_file(file);}

/*! \ingroup renderermanip1 */
inline DevManip1<std::string> GeomFromFile(const std::string& file)
{ return (DevManip1<std::string>(file, &geom_from_file)); }


#endif
