
// $LICENSE


#include "Utility/safe-file.h"

#include <iostream>
#include <fstream>
#include <string> 
#include <stdlib.h>

namespace GrAL {
 
int file_interactive::open(::std::ifstream& in, ::std::string const& filename)
  {
    if(in.is_open()) {
      in.close();
    }

   const char* name = filename.c_str();
   in.open(name);

   ::std::string my_filename(filename);
     while (! in.is_open()) {
     ::std::cerr << "could not open input file \"" << my_filename << "\" !\n"
	       << "please enter new filename: ";

     ::std::cin >> my_filename;

     const char* my_name = my_filename.c_str();
     in.open(my_name);
   }
   return is_open;
  }

int file_interactive::open_gz(::std::ifstream& in, ::std::string const& filename, int strictness)
{
  
  if(in.is_open())
    in.close();
  
  int res = (int) failed;
  ::std::string my_name(filename);
  const char* name = filename.c_str();
  do { 
    in.open(name);
    if( in.is_open()) 
      res = (int) is_open;
    else { // try ".gz"
      in.open((my_name+".gz").c_str());
      if(in.is_open()) {
	in.close();
	res = (int) is_open_gz;
	system( ("gunzip " + my_name+".gz").c_str());
	in.open(name);
      }
      // ERROR: file could not be opened: prompt user
      else {
	::std::cerr << "could not open neither \"" << my_name << "\" nor \""
	     << my_name + ".gz" << "\" !\n";
	if ((strictness == (int)insist)) {
	  ::std::cerr <<  "please enter new filename: ";
	  ::std::cin >> my_name;
	  name = my_name.c_str();
	}
      }
    }
  }  while (! in.is_open() && (strictness == (int)insist));
    
  return res;
}

void file_interactive::close(::std::ifstream& in, ::std::string const& nm, int gz)
{
  if(in.is_open())
    in.close();
  if(gz == (int) is_open_gz)
    system(("gzip " + nm).c_str());
}

void file_interactive::close(::std::ofstream& out, ::std::string const& nm, int gz)
{
  if(out.is_open())
    out.close();
  if(gz == (int) is_open_gz)
    system(("gzip " + nm).c_str());
}

int file_interactive::open(::std::ofstream& out, ::std::string const& filename) 
  //::std::ios_base::openmode ios_mode)
{
  int res = (int)failed;
  if(out.is_open())
    out.close();
  
  const char* name = filename.c_str();
  //  out.open(name, ios_mode);
  out.open(name);
  
  ::std::string my_name = filename;
  while (!out.is_open()) {
    ::std::cerr << "could not open output file \"" << my_name << "\" !\n"
	      << "please enter new filename: ";
    ::std::cin  >> my_name;
    
     const char* new_name = my_name.c_str();
     out.open(new_name);
  }
  if(out.is_open())
    res = is_open;

  return res;
}

} // namespace GrAL 
