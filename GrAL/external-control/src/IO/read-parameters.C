#include <list> // STL
#include <iostream.h>

#include "IO/skip-comments.h"
#include "IO/read-parameters.h"


class string_list : public std::list<std::string> {
public:
  string_list() {}
};

MutableVars::MutableVars()  
  : table(new string_table_1), 
    unrecognized(new string_list)  {}

unsigned MutableVars::size() const { return table->size();}
bool     MutableVars::defined(std::string const& nm) const { 
  return (table->find(nm) != table->end());
}

Mutator* MutableVars::getMutator(std::string const& nm)
{
  return (table->find(nm) != table->end() ? 
	  (*(table->find(nm))).second : 0);
}

//MutableVars::~MutableVars() { delete(table); delete(unrecognized);}
MutableVars::~MutableVars() {}

void MutableVars::AddVariable(std::string const& name, Mutator* m)
  { (*table)[name]=m; }

void MutableVars::AddVariable(char const*  name, Mutator* m)
  { (*table)[std::string(name)]=m; }


// read values as long as there are any, 
// unknown names are recorded in "unrecognized".

// should be virtual members, shouldn't they?
static inline char comment_start() { return '#';}
static inline char comment_end()   { return '\n';}

void MutableVars::ReadVariable(std::istream& is)
{
  is >> ws;
  //  skip_comments_istream in(is,comment_start(),comment_end());
  if(is) {
    std::string s; 
    is >> s;

     std::map<std::string, Mutator*, std::less<std::string> >::iterator it;
    if( (it = table->find(s)) != table->end()) {
      //  cerr << "found: " << s << endl;
      // (*table)[s]->read(is);//.the_istream()); // hier nur istream-Funktionalitaet!
      (*it).second->read(is);//.the_istream()); // hier nur istream-Funktionalitaet!
    }
    else if (s != "\n")
      unrecognized->push_back(s);
  }
}

void MutableVars::ReadValues(std::istream& is)
{ 
  while (is) { 
    skip_comment(is);
    ReadVariable(is); 
  }
}

void MutableVars::PrintValues(std::ostream     & out, 
			      std::string const& pre, 
			      std::string const& sep) const
{
   table_type::const_iterator item(table->begin());
  for(; item != table->end(); item++) {
    //    out << pre << (*item).first << sep;
    (*item).second->print(out, pre + (*item).first + sep);
    if( (*item).second->description() != "")
      out << "  " << comment_start() << " " 
	  << (*item).second->description() << " " 
	  << comment_end();
    out << "\n";
  }
}

bool MutableVars::HasUnrecognized() const { return (unrecognized->size() != 0);} 

void MutableVars::PrintUnrecognized(std::ostream& out) const
{
  string_list::const_iterator item(unrecognized->begin());
  for(; item != unrecognized->end(); ++item)
    out << *item << '\n';
}
