#ifndef NMWR_GB_GRAPHICSDEVICE_ATTRIBUTE_H
#define NMWR_GB_GRAPHICSDEVICE_ATTRIBUTE_H

// $LICENSE

#include <string>
#include "GraphicsDevice/value-table.h"

#include "Utility/reference-count.h"

namespace GrAL {

typedef ::std::string          attr_key;
typedef ValueTable<attr_key> attribute;



/*! \brief Class for storing and retrieving graphics attributes.
   
 */
class Attribute {
private:
 copy_on_write_ptr<attribute>  attr;
public:
 Attribute() : attr(new attribute()) {}
 attribute      & AttrTable()        { return (*attr);}
 attribute const& AttrTable()  const { return (*attr);}
 bool IsEmpty() const {return (attr->IsEmpty());}


 static Attribute 
   black(),
   white(),
   red(),
   green(),
   blue(),
   yellow(),
   pink(),
   magenta(),
   cyan(),
   orange(), 
   new_color(double,double,double);
};


class attr_base {
public:
  virtual attr_key key() const = 0;
};


template<class Attr>
inline bool GetValue(const Attribute& A, Attr& a)
{ return GetVal(A.AttrTable(),a);}

template<class Attr>
inline void SetValue(Attribute& A, const Attr& a)
{ AddVal(A.AttrTable(),a);}

} // namespace GrAL 

#endif
