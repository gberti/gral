#ifndef NMWR_GB_MUTATOR_H
#define NMWR_GB_MUTATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include <iostream.h>
#include <stdlib.h>

#include "compiler-config.h"
#include "mystring.h"
#include "IO/mutator-base.h"
#include "Utility/pre-post-conditions.h"
#include "Container/type-name-traits.h"



///////////////////////////////////////////////////////////////
//
//  Some implementations of the Mutator-interface
//
///////////////////////////////////////////////////////////////

// simplest concrete Mutator
template<class T>
class TypedMutator : public Mutator {
  //protected:
public:
  T& v;
public:
  TypedMutator(T& vv) : v(vv) {}
  //  T value() {return v;}
  virtual void read(istream& in)   { 
    REQUIRE_ALWAYS((in.good()), "Stream not good! v = " << v << '\n',1);
    in >> v;
    ENSURE_ALWAYS((in.eof() || in.good()), "Input failed! v = " << v << '\n',1);
  }
  virtual void print(ostream& out) const 
    { out << v;}
  virtual void print(ostream& out, const string& prefix = "") const 
    { out << prefix << v;}

  virtual string vartypename() const { return string(type_name_traits<T>::name());}
};

 
// notify controlee, if variable changes.
template<class T>
class NotifyOnChangeMutator : public TypedMutator<T> {
public:
  typedef  TypedMutator<T> base;
  NotifyOnChangeMutator(T& t, controlable& c)
    : base(t), controlee(c) {}
  virtual void read(istream& in) {
    T old(value());
    base::read(in);
    if( old != value()) controlee.notify();
  }
private:
  controlable& controlee;
};


/*
template<class T>
class DebugTypedMutator : public TypedMutator<T> {
  typedef TypedMutator<T> tm;
public: 
  DebugTypedMutator(T& t) : tm(t) {}
  virtual void read(istream& in) {
    cerr << "Mutator: old value: " << value();
    tm::read(in);
    cerr << "  new value: " << value();
   }
  virtual void print(ostream& out)
    { tm::print(out); out << " (@ " << (void*)&v << ")";}
};
*/

class SetTrueOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  SetTrueOnReadMutator(bool& flag) : tm(flag) {}
  virtual void read(istream&) { v = true;}
  virtual void print(ostream& out) const { out << v;}
  virtual void print(ostream& out, const string& name) const 
    { if(v) out << name;} 
};


class SetFalseOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  SetFalseOnReadMutator(bool& flag) : tm(flag) {}
  virtual void read(istream&) { v = false;}
  virtual void print(ostream& out) const { out << !v;}
  virtual void print(ostream& out, const string& name) const 
    { if(!v) out << name;} 
};



class FlipOnReadMutator : public TypedMutator<bool> {
  typedef TypedMutator<bool> tm;
 public:
  FlipOnReadMutator(bool& flag) : tm(flag) {}
  virtual void read(istream&) { v = !v;}
  virtual void print(ostream& out) const { out << v;}
  virtual void print(ostream& out, const string& name) const 
    {  out << name;} 
};

// examples: 
// Ctrl.add("show", new SetTrueOnRead(show));
//  will set show to true if read,
// Ctrl.add("toggle-binary", new SetTrueOnRead(bin_flag));
// will flip the value of bin_flag if "toggle-binary" is read.



template<class T, class Tsec>
class SetOnReadMutator : public TypedMutator<T> {
  typedef TypedMutator<T> tm;
protected:
  Tsec& v2;
  Tsec  deflt;
public:
  SetOnReadMutator(T& t, Tsec& t2, Tsec def) : tm(t), v2(t2), deflt(def) {}
  virtual void read(istream& in) { tm::read(in); v2 = deflt;} 
};

// example: SetOnReadMutator<double,bool>(x,xread,true)
// will set xread to true if x is read.

template<class T>
class CommentedMutator : public TypedMutator<T> {
  typedef TypedMutator<T> tm;
  string comment;
public:
  CommentedMutator(T& t, const string& c) 
    //: tm(t), comment(c) {}
    : TypedMutator<T>(t), comment(c) {}
  //  virtual void print(ostream& out) const { tm::print(out); out  << "  " << comment;} 
  virtual string description() const { return comment;}
};

// this prints a message to an ostream if read.
// useful for adding help messages to command-line control devices,
// e.g. :
//  string help = "Usage: foobar -f <file> -n <number> \n";
//  Ctrl.add("-?", new MessageOnReadMutator(cerr,help));
//
class MessageOnReadMutator : public Mutator {
private:
  ostream* out;
  string   text;
public:
  MessageOnReadMutator(ostream & ou, string const& txt) 
    : out(&ou), text(txt) {}
  virtual void read (istream& in) { (*out) << text; exit(0); }
  virtual void print(ostream&   ) const {}
  virtual void print(ostream& , const string& ) const {}

  virtual string vartypename() const { return "";}

}; 

////////////////////////////////
// Mutator-generating Functions
////////////////////////////////

// simplest: base to all other Mutators below
template<class T>
inline TypedMutator<T>* GetMutator(T& t) { return new TypedMutator<T>(t);}

template<class T>
inline CommentedMutator<T>* GetMutator(T& t, const string& comment) 
{ return new CommentedMutator<T>(t,comment);}
 
template<class T>
inline CommentedMutator<T>* GetMutator(T& t, const char* comment) 
{ return new CommentedMutator<T>(t,comment);}
 
// notify observ if t is read & changed
template<class T>
inline NotifyOnChangeMutator<T>* GetNotifyingMutator(T& t, controlable& observ) 
{ return new NotifyOnChangeMutator<T>(t,observ);}

// t = true if read
inline SetTrueOnReadMutator* GetTrueOnReadMutator(bool& t)
{ return new SetTrueOnReadMutator(t);}

// t = false if read
inline SetFalseOnReadMutator* GetFalseOnReadMutator(bool& t)
{ return new SetFalseOnReadMutator(t);}

// t = !t if read
inline FlipOnReadMutator* GetFlipOnReadMutator(bool& t)
{ return new FlipOnReadMutator(t);}

// obs = deflt if t is read
template<class T, class TObs>
inline SetOnReadMutator<T,TObs>* GetSetOnReadMutator(T& t, TObs& obs, TObs deflt)
{ return new SetOnReadMutator<T,TObs>(t,obs,deflt); }

// write a comment if printed
template<class T>
inline CommentedMutator<T>* GetCommentedMutator(T& t, const string& comment)
{ return  new  CommentedMutator<T>(t,comment); }

template<class T>
inline CommentedMutator<T>* GetCommentedMutator(T& t, const char* comment)
{ return  new  CommentedMutator<T>(t,string(comment)); }

#endif
