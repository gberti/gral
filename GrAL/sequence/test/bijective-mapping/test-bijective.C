
#include <iostream.h>
#include <fstream.h>
#include <string>

#include "Utility/pre-post-conditions.h"
#include "Container/bijective-mapping.h"


int main(int argc, char* argv[]) {

  typedef  bijective_mapping<int,int>::domain_type domain_type;
  typedef  bijective_mapping<int,int>::range_type  range_type;

  typedef  domain_type::const_iterator domain_iter;
  typedef   range_type::const_iterator  range_iter;

  std::string mapfilename1  = "map.dat";
  std::string mapfilename2 = "map2.dat";

  int N = 10;
  bijective_mapping<int,int> m1;

  ifstream mapfile1(mapfilename1.c_str());
  bijective_mapping<int,int> mf1;
  read_bm(mf1,mapfile1);
  mapfile1.close();

  ifstream mapfile2(mapfilename2.c_str());
  bijective_mapping<int,int> mf2;
  read_bm(mf2,mapfile2);
  mapfile2.close();
  
  for(int i = 1; i <= N; ++i)
    m1[i] = -i;

  bijective_mapping<int,int> i1 = m1.inverse();
  bijective_mapping<int,int> m2 = m1;
  bijective_mapping<int,int> i2 = m2.inverse();

  cerr << "mf1:\n"; write_bm(mf1,cerr); cerr << '\n'; 
  cerr << "mf2:\n"; write_bm(mf2,cerr); cerr << '\n'; 

  cerr << "m1:\n"; write_bm(m1,cerr); cerr << '\n'; 
  cerr << "i1:\n"; write_bm(i1,cerr); cerr << '\n'; 
  cerr << "m2:\n"; write_bm(m2,cerr); cerr << '\n'; 
  cerr << "i2:\n"; write_bm(i2,cerr); cerr << '\n'; 

  cerr << "domain(m1): ";
  for(domain_iter d1 = m1.domain().begin(); d1 != m1.domain().end(); ++d1)
    cerr << *d1 << ' ';
  cerr << '\n';
  cerr << "range(m1): ";
  for(range_iter r1 = m1.range().begin(); r1 != m1.range().end(); ++r1)
    cerr << *r1 << ' ';
  cerr << '\n';

  REQUIRE( (m1.domain().size() == m1.range().size()), "|domain| != |range|\n",1);
  REQUIRE( (i1.domain().size() == i1.range().size()), "|domain| != |range|\n",1);

  for(domain_iter di = m1.domain().begin(); di != m1.domain().end(); ++di) {
    REQUIRE( (*di == i1(m1(*di))), "inverse is not ok!\n",1);    
    REQUIRE( (m1(*di) == m2(*di)), "copy is not ok!\n",1);    
    REQUIRE( (i1(m1(*di)) == i2(m2(*di))), "inverse copies not ok!\n",1); 
  }

  for(range_iter ri = i1.range().begin(); ri != i1.range().end(); ++ri) {
    REQUIRE( m1.defined(*ri), "m1 not defined for di in range(i1)!\n",1);
  }
}


