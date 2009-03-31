#ifndef NMWR_GB_MUTATOR_H
#define NMWR_GB_MUTATOR_H

/*! \file
 */


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <iostream>

#include <boost/shared_ptr.hpp>
#include "Utility/ref-ptr.h" // for null_deleter
 

#include "Config/compiler-config.h"
#include "IO/mutator-base.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/type-name-traits.h"

namespace GrAL {

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
class TypedMutator : public mutator_impl  {
public:
  T& v;
public:
  TypedMutator(T& vv) : v(vv) {}
  //  T value() {return v;}
protected:
  virtual bool read_var(std::istream& in)   { 
    REQUIRE_ALWAYS((in.good()), "Stream not good! v = " << v << '\n',1);
    in >> v;
    ENSURE_ALWAYS((in.eof() || in.good()), "Input failed! v = " << v << '\n',1);
    return true;
  }
public:
  virtual void print(std::ostream& out) const 
    { out << v;}
  virtual void print(std::ostream& out, const std::string& prefix) const 
    { out << prefix << v;}

  virtual std::string vartypename() const { return std::string(type_name_traits<T>::name());}
};

  /*! \brief Mutator for container types
     
  On read, this mutator appends to the controled container.

  \templateparams
   - T is a partial model of STL Back Insertion Sequence
     - <code> const_iterator begin() / end() </code>
     - <code> push_back(value_type) </code>

   \b Example:
   \code
   std::vector<int> indices;
   RegisterAt(Ctrl, "+i", GetPushbackMutator(indices));
   // on the commandline, use '+i' repeatedly:
   // pgrm  +i 1 +i 88 +i 7
   \endcode

  */
  template<class T>
  class PushbackMutator : public mutator_impl  { 
    T& v;
  public:
    PushbackMutator(T& v_) : v(v_) {}
    virtual void print(std::ostream& out) const { 
      for(typename T::const_iterator i = v.begin(); i != v.end(); ++i)
	out << *i << ' ';
    }
    virtual void print(std::ostream& out, const std::string& prefix) const 
    { out << prefix; print(out); }

    virtual std::string vartypename() const { return std::string(type_name_traits<T>::name());}

  protected:
    virtual bool read_var(std::istream& in) {
      REQUIRE_ALWAYS((in.good()), "Stream not good! ",1);
      typename T::value_type vv;
      in >> vv;
      v.push_back(vv);
      ENSURE_ALWAYS((in.eof() || in.good()), "Input failed in PushBackMutator!", 1);
      return true;
    }
  };
  /*! \brief Creator function for the PushbackMutator
    
      \ingroup mutatorcreators 

  */
  template<class T>
  inline Mutator  GetPushbackMutator(T& t) { return Mutator(new PushbackMutator<T>(t));}

  


//! Notify controlee, if variable changes.
template<class T>
class NotifyOnChangeMutator : public TypedMutator<T> {
public:
  typedef  TypedMutator<T> base;
  NotifyOnChangeMutator(T& t, controlable& c)
    : base(t), controlee(c) {}
protected:
  virtual bool read_var(std::istream& in) {
    T old(base::v);
    bool res = base::read_var(in);
    if( old != base::v) controlee.notify();
    return res;
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
  virtual void print(std::ostream& out) const { out << v;}
  virtual void print(std::ostream& out, const std::string& name) const 
    { if(v) out << name;} 
protected:
  virtual bool read_var(std::istream&) { v = true; return true;}

};

//! Sets value to false, if name is read
class SetFalseOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  SetFalseOnReadMutator(bool& flag) : tm(flag) {}
  virtual void print(std::ostream& out) const { out << !v;}
  virtual void print(std::ostream& out, const std::string& name) const 
    { if(!v) out << name;} 
protected:
  virtual bool read_var(std::istream&) { v = false; return true;}
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
  virtual void print(std::ostream& out) const { out << v;}
  virtual void print(std::ostream& out, const std::string& name) const 
    {  out << name;} 
protected:
  virtual bool read_var(std::istream&) { v = !v; return true; } 

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
protected:
  virtual bool read_var(std::istream& in) { tm::read_var(in); v2 = deflt; return true;} 
};



/*! \brief Mutator printing an additional comment when written
 */
template<class T>
class CommentedMutator : public TypedMutator<T> {
  typedef TypedMutator<T> tm;
  std::string comment;
public:
  CommentedMutator(T& t, const std::string& c) 
    : TypedMutator<T>(t), comment(c) {}
  virtual std::string description() const { return comment;}
};

/*! \brief Mutator which prints a message to an ostream if read.
 
  This is useful for adding help messages to command-line control devices, e.g. :
  \code
  string help = "Usage: foobar -f <file> -n <number> \n";
  Ctrl.add("-?", GetMessageOnReadMutator(cerr,help));
 \endcode
*/

class MessageOnReadMutator : public mutator_impl {
private:
  boost::shared_ptr<std::ostream> out;
  std::string                     text;
  bool                            do_ex;
public:
  MessageOnReadMutator(std::ostream & ou, std::string const& txt, bool do_exit = true) 
    : out(&ou, null_deleter()), text(txt), do_ex(do_exit) {}
  virtual void print(std::ostream&   ) const {}
  virtual void print(std::ostream& , std::string const& ) const {}

  virtual std::string vartypename() const { return "";}
protected:
  virtual bool read_var(std::istream& in) { (*out) << text; if(do_ex) exit(0); return true; }
}; 



////////////////////////////////
// Mutator-generating Functions
////////////////////////////////

/*! \defgroup mutatorcreators Creator functions for mutators

*/

/*! \brief Creator function for the basic TypedMutator
    
    \ingroup mutatorcreators 

    This function is rarely used directly, 
    because it is implicitely called by \ref RegisterAt().
 */
template<class T>
inline Mutator  GetMutator(T& t) { return Mutator(new TypedMutator<T>(t));}

//!
/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline Mutator GetMutator(T& t, std::string const& comment) 
{ return  Mutator(new CommentedMutator<T>(t,comment));}

/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline Mutator GetMutator(T& t, const char* comment) 
{ return  Mutator(new CommentedMutator<T>(t,comment));}
 

/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline Mutator GetCommentedMutator(T& t, std::string const& comment)
{ return  Mutator(new  CommentedMutator<T>(t,comment)); }

/*! \brief Creator function for CommentedMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline Mutator GetCommentedMutator(T& t, const char* comment)
{ return  Mutator(new  CommentedMutator<T>(t,std::string(comment))); }


/*! \brief Creator function for NotifyOnChangeMutator
    
    \ingroup mutatorcreators 
 */
template<class T>
inline Mutator GetNotifyingMutator(T& t, controlable& observ) 
{ return Mutator(new NotifyOnChangeMutator<T>(t,observ));}

/*! \brief Creator function for SetTrueOnReadMutator
    
    \ingroup mutatorcreators 
 */
inline Mutator GetTrueOnReadMutator(bool& t)
{ return Mutator(new SetTrueOnReadMutator(t));}

/*! \brief Creator function for SetFalseOnReadMutator
    
    \ingroup mutatorcreators 
 */
inline Mutator GetFalseOnReadMutator(bool& t)
{ return Mutator(new SetFalseOnReadMutator(t));}

/*! \brief Creator function for FlipOnReadMutator
    
    \ingroup mutatorcreators 
 */
inline Mutator GetFlipOnReadMutator(bool& t)
{ return Mutator(new FlipOnReadMutator(t));}

/*! \brief Creator function for SetOnReadMutator
    
    \ingroup mutatorcreators 
 */
template<class T, class TObs>
inline Mutator GetSetOnReadMutator(T& t, TObs& obs, TObs deflt)
{ return Mutator(new SetOnReadMutator<T,TObs>(t,obs,deflt)); }



/*! \brief Creator function for MessageOnReadMutator
    
    \ingroup mutatorcreators 
    
    Returns a Mutator which prints a message when read.
 */
inline Mutator GetMessageOnReadMutator(std::ostream & out, std::string const& txt)
{ return Mutator(new MessageOnReadMutator(out, txt, false));}


/*! \brief Creator function for MessageOnReadMutator
    
    \ingroup mutatorcreators 
    
    Returns a Mutator which prints a message when read.
    This is very handy when implementing usage or help messages.

   \par Example:
   \code
   ControlDevice Ctrl = ...
   
   string h;
   h += "myprog:  Do something very useful.\n";
   h += "Usage: myprog <option>\n";
  
   int n;
   RegisterAt(Ctrl, "-n", n);
   h += "  -n <int>  (Number of norks)\n";

   int k;
   RegisterAt(Ctrl, "-k", k);
   h += "  -n <int>  (Number of knorks)\n";

   Mutator help = GetMessageAndExitOnReadMutator(cout, h);
   Ctrl.add("-h",     help);
   Ctrl.add("-?",     help);
   Ctrl.add("--help", help);

   Ctrl.update();
   \endcode

 */
inline Mutator GetMessageAndExitOnReadMutator(std::ostream & out, std::string const& txt)
{ return Mutator(new MessageOnReadMutator(out, txt, true));}


} // namespace GrAL 

#endif
