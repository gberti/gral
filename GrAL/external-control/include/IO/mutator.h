#ifndef NMWR_GB_MUTATOR_H
#define NMWR_GB_MUTATOR_H

/*! \file
 */

// $LICENSE


#include <iostream>
#include <stdlib.h>

#include "Config/compiler-config.h"
#include "IO/mutator-base.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/type-name-traits.h"


//----------------------------------------------------------------
//
//  Some implementations of the Mutator-interface
//
//----------------------------------------------------------------

/*! \brief  The simplest concrete Mutator


    Reads value v from an istream / writes v to an ostream
    This mutator is ok for reading parameter files or command lines,
    or even GUI input if it is converted to a string.

    \see mutatorcreators
 */
template<class T>
class TypedMutator : public Mutator {
  //protected:
public:
  T& v;
public:
  TypedMutator(T& vv) : v(vv) {}
  //  T value() {return v;}
  virtual void read(std::istream& in)   { 
    REQUIRE_ALWAYS((in.good()), "Stream not good! v = " << v << '\n',1);
    in >> v;
    ENSURE_ALWAYS((in.eof() || in.good()), "Input failed! v = " << v << '\n',1);
  }
  virtual void print(std::ostream& out) const 
    { out << v;}
  virtual void print(std::ostream& out, const std::string& prefix = "") const 
    { out << prefix << v;}

  virtual std::string vartypename() const { return std::string(type_name_traits<T>::name());}
};

 
//! Notify controlee, if variable changes.
template<class T>
class NotifyOnChangeMutator : public TypedMutator<T> {
public:
  typedef  TypedMutator<T> base;
  NotifyOnChangeMutator(T& t, controlable& c)
    : base(t), controlee(c) {}
  virtual void read(std::istream& in) {
    T old(base::v);
    base::read(in);
    if( old != base::v) controlee.notify();
  }
private:
  controlable& controlee;
};


//! Sets value to true, if name is read
/*! examples: 
 \code
  Ctrl.add("show", new SetTrueOnReadMutator(show));
 \endcode
  will set show to true if read,
*/
class SetTrueOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  SetTrueOnReadMutator(bool& flag) : tm(flag) {}
  virtual void read(std::istream&) { v = true;}
  virtual void print(std::ostream& out) const { out << v;}
  virtual void print(std::ostream& out, const std::string& name) const 
    { if(v) out << name;} 
};

//! Sets value to false, if name is read
class SetFalseOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  SetFalseOnReadMutator(bool& flag) : tm(flag) {}
  virtual void read(std::istream&) { v = false;}
  virtual void print(std::ostream& out) const { out << !v;}
  virtual void print(std::ostream& out, const std::string& name) const 
    { if(!v) out << name;} 
};


/*! \brief  Mutator to toggle the boolean value of v

  Example: 
 \code
  Ctrl.add("toggle-binary", new FlipOnReadMutator(bin_flag));
 \endcode
  will flip the value of bin_flag if "toggle-binary" is read.
*/
class FlipOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  FlipOnReadMutator(bool& flag) : tm(flag) {}
  virtual void read(std::istream&) { v = !v;}
  virtual void print(std::ostream& out) const { out << v;}
  virtual void print(std::ostream& out, const std::string& name) const 
    {  out << name;} 
};


/*! \brief Mutator which sets v2 to deflt, if read occurs.
  example: 
  \code
  SetOnReadMutator<double,bool>(x,xread,true)
  \endcode
  will set xread to true if x is read.
*/
template<class T, class Tsec>
class SetOnReadMutator : public TypedMutator<T> {
  typedef TypedMutator<T> tm;
protected:
  Tsec& v2;
  Tsec  deflt;
public:
  SetOnReadMutator(T& t, Tsec& t2, Tsec def) : tm(t), v2(t2), deflt(def) {}
  virtual void read(std::istream& in) { tm::read(in); v2 = deflt;} 
};



/*! \brief Mutator printing an additional comment when written
 */
template<class T>
class CommentedMutator : public TypedMutator<T> {
  typedef TypedMutator<T> tm;
  std::string comment;
public:
  CommentedMutator(T& t, const std::string& c) 
    //: tm(t), comment(c) {}
    : TypedMutator<T>(t), comment(c) {}
  //  virtual void print(ostream& out) const { tm::print(out); out  << "  " << comment;} 
  virtual std::string description() const { return comment;}
};

/*! \brief Mutator which prints a message to an ostream if read.
 
  This is useful for adding help messages to command-line control devices, e.g. :
  \code
  string help = "Usage: foobar -f <file> -n <number> \n";
  Ctrl.add("-?", new MessageOnReadMutator(cerr,help));
 \endcode
*/

class MessageOnReadMutator : public Mutator {
private:
  std::ostream* out;
  std::string   text;
public:
  MessageOnReadMutator(std::ostream & ou, std::string const& txt) 
    : out(&ou), text(txt) {}
  virtual void read (std::istream& in) { (*out) << text; exit(0); }
  virtual void print(std::ostream&   ) const {}
  virtual void print(std::ostream& , std::string const& ) const {}

  virtual std::string vartypename() const { return "";}

}; 

////////////////////////////////
// Mutator-generating Functions
////////////////////////////////

/*! \defgroup mutatorcreators Creator functions for mutators

*/

/*! \brief Creator function for TypedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline TypedMutator<T>* GetMutator(T& t) { return new TypedMutator<T>(t);}

//!
/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline CommentedMutator<T>* GetMutator(T& t, std::string const& comment) 
{ return new CommentedMutator<T>(t,comment);}

/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline CommentedMutator<T>* GetMutator(T& t, const char* comment) 
{ return new CommentedMutator<T>(t,comment);}
 
/*! \brief Creator function for NotifyOnChangeMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline NotifyOnChangeMutator<T>* GetNotifyingMutator(T& t, controlable& observ) 
{ return new NotifyOnChangeMutator<T>(t,observ);}

/*! \brief Creator function for SetTrueOnReadMutator
    
    \ingroup mutatorcreators 
 */
inline SetTrueOnReadMutator* GetTrueOnReadMutator(bool& t)
{ return new SetTrueOnReadMutator(t);}

/*! \brief Creator function for SetFalseOnReadMutator
    
    \ingroup mutatorcreators 
 */
inline SetFalseOnReadMutator* GetFalseOnReadMutator(bool& t)
{ return new SetFalseOnReadMutator(t);}

/*! \brief Creator function for FlipOnReadMutator
    
    \ingroup mutatorcreators 
 */
inline FlipOnReadMutator* GetFlipOnReadMutator(bool& t)
{ return new FlipOnReadMutator(t);}

/*! \brief Creator function for SetOnReadMutator
    
    \ingroup mutatorcreators 
 */
template<class T, class TObs>
inline SetOnReadMutator<T,TObs>* GetSetOnReadMutator(T& t, TObs& obs, TObs deflt)
{ return new SetOnReadMutator<T,TObs>(t,obs,deflt); }

/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline CommentedMutator<T>* GetCommentedMutator(T& t, std::string const& comment)
{ return  new  CommentedMutator<T>(t,comment); }

/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline CommentedMutator<T>* GetCommentedMutator(T& t, const char* comment)
{ return  new  CommentedMutator<T>(t,std::string(comment)); }

#endif
