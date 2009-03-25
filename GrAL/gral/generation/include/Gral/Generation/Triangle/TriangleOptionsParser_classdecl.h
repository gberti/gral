#ifndef TRIANGLE_OPTIONS_PARSER_CLASSDECL_H
#define TRIANGLE_OPTIONS_PARSER_CLASSDECL_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <map>

class TriangleOptionsParser : public yyFlexLexer {
  public:
  TriangleOptionsParser() : _minangle(20),
                            report(""), _action(DEFAULT), at_char(0) {init();}  
  ~TriangleOptionsParser() {}
  int yylex();
  inline void Parse();
  inline void Parse(const char*);

  const char* GetOptions(); 
  std::string Info(const char *flags);
  
  bool GetOption(const std::string name) { return flags[name].second; }
  void SetOption(const std::string name, bool v) { flags[name].second = v;}
  
  double GetMinAngle() const { return _minangle;}
  void SetMinAngle(const double angle) { _minangle = angle;}
  
  double GetMaxArea() const {return _maxarea;}
  void SetMaxArea(const double area) { _maxarea = area;}
  
  private:       
  typedef enum { REPORT, DEFAULT } action_type;
  void plg();  
  void refine();  
  void quality();  
  void quality(const char* text);  
  void maxarea();  
  void maxarea(const char* text);  
  void userconstraint();  
  void attributes();  
  void convexhull();  
  void weighted();  
  void regular();  
  void jettison();  
  void edgelist();  
  void voronoi();  
  void neighbours();  
  void geomview();  
  void noboundinfo();  
  void nopolyfile();  
  void nonodefile();  
  void noelefile();  
  void noiterinfo();  
  void ignoreholes();  
  void noexactarithm();  
  void zerobasednum();  
  void secondorder();  
  void noboundsplit();  
  void steiner();  
  void steiner(const char *text);  
  void equatorialcircles();  
  void incremental();  
  void sweepline();  
  void verticalcuts();  
  void forcebysplit();  
  void checkconsistency();  
  void quiet();
  void verbose();
  void inellipse();
  void usertest();
  void parseminus();
  void invalid_input(const char *);

  void init(); 

  typedef std::map<const std::string, std::pair<const char,bool> > flags_container;
  std::map<const std::string, std::pair<const char,bool> > flags;
  double _minangle;
  int _nsteiner;
  double _maxarea;
  char s[100];
  std::string report;
  action_type _action;
  std::string options_text; // options to be parsed
  int at_char;   // indicates position of invalid input
};

#endif
