#ifndef NMWR_GB_ATTRIBUTE_H
#define NMWR_GB_ATTRIBUTE_H

#include "mystring.h"
#include "Graphics/value-table.h"
//#include "Graphics/color.h"

typedef string attr_key;
typedef ValueTable<attr_key> attribute;



class Attribute {
private:
 attribute* attr;
public:
 Attribute() : attr(new attribute()) {}
 attribute& AttrTable() { return (*attr);}
 const attribute& AttrTable()  const { return (*attr);}
 bool IsEmpty() const {return (attr->IsEmpty());}

// double Transparent() const {return trans;}

 static Attribute black(),white(),red(),green(),blue(),yellow(),pink(),
   magenta(),cyan(),orange(),new_color(double,double,double);
};

class attr_base {
public:
  virtual attr_key key() const = 0;
};

template<class Attr>
bool GetValue(const Attribute& A, Attr& a)
{ return GetVal(A.AttrTable(),a);}

template<class Attr>
void SetValue(Attribute& A, const Attr& a)
{ AddVal(A.AttrTable(),a);}

/*
extern const Attribute* green;
extern const Attribute* red;
*/
#endif
