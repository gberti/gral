#ifndef NMWR_GB_READ_PARAMETERS_H
#define NMWR_GB_READ_PARAMETERS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
//
//     read-parameters.h -- a simple class for reading some
//     parameter values from a stream
//
//     (C) Guntram Berti, date: 1.09.96
//
//     purpose: say you have a number of numerical parameters
//     declared in your program, i.e. an instance of a class for which you want 
//     sometimes to read in a different value than the predefined one. 
//     Then you could set up a huge parameter file which is scanned
//     at startup time and where you give a value for each variable
//     you want to be able to change sometimes. This is very errorprone,
//     because you have to remember the ordering of the variables,
//     the parameter file is not well readable (you cannot associate 
//     values with variables) and is larger then it has to be.
//
//     A better solution is the following: You give an unique name ( a string) 
//     to every variable you declare and want to be changeable. The parameterfile
//     then contains only the names and values of the variables that are really
//     subject to changes, the ordering does not matter, and you may easily
//     read your parameter files. The only thing you have to do is to define
//     operator >> for each class type you want to read in.
//     The class that achieves this is MutableVars: you add the names and
//     references to a variable you want to be <<mutable>>, and then you
//     may read in parameter files. How does this look like in a piece of code?
//
//     MutableVars MV; // define one instance of the bookkeeping class
//     ...
//     double x;
//     myclass A; // a variable of some class readable with >> A
//     AddVar(MV, "x",x);  // add x to database
//     AddVar(MV, "A",A);  // add A to database
//     ...
//     MV.ReadValues(param_file); // read a parameter file
//     ...
//
//     In the parameter file, you may (but do not have to!!) write the
//     following:
//
//      x   0.123456
//     ...
//      A   <data of A> 
//
//     where <data of A> is what you would use when you want to read A
//     directly:  cin >> A;
//
///////////////////////////////////////////////////////////////////

#include "forward/string.h"
#include "forward/iostream.h"

#include "IO/mutator.h"
#include "Utility/reference-count.h"

#include "map.h"  // STL

// this is basically a dictionary that maps strings
// to placeholders for a variable reference.
// pointers to the abstract base class are used in order to profit
// from polymorphism (derived classes are automatically
// generated via templates in class TypedMutator).
class string_table_1: public map<string, Mutator*, less<string> > {
public:
  string_table_1() {}
};

//class string_table_1;
class string_list;
class Mutator;

class MutableVars {
private:
  //  typedef map<string, Mutator*, less<string> > table_type;
  //table_type table;
  typedef string_table_1                      table_type;
  typedef table_type::const_iterator const_iterator;

  // RCIPtr<table_type>  table;
  // RCIPtr<string_list> unrecognized;
  table_type* table;
  string_list* unrecognized;
  // FORBIDDEN
  MutableVars(MutableVars const&);
  MutableVars& operator=(MutableVars const&);
public:
  MutableVars();
  ~MutableVars();

  void AddVariable(const string& name, Mutator* m);
  void AddVariable(const char*   name, Mutator* m);
  void ReadVariable(istream& is);
  void ReadValues(istream& in);
  void PrintValues(ostream& out,
		   const string& prefix = "",
		   const string& sep = " ") const;
  bool HasUnrecognized() const;
  void PrintUnrecognized(ostream& out) const;
 
  unsigned size() const;
  bool     defined   (string const& nm) const;
  Mutator* getMutator(string const& nm);
  
  const_iterator begin() const { return table->begin();}
  const_iterator end()   const { return table->end();}
};




// this function is necessary, because MutableVars::AddVariable
// (as a member function) is not (yet) allowed to be a template function

template<class T> 
inline void AddVar(MutableVars& MV, const string& name, T& v)
{ MV.AddVariable(name,GetMutator(v));}


template<class T> 
inline void AddVar(MutableVars& MV, const char* name, T& v)
{ AddVar(MV,string(name),v);}


#endif
