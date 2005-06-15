#ifndef TRIANGLE_OPTIONS_PARSER_CLASSDECL_H
#define TRIANGLE_OPTIONS_PARSER_CLASSDECL_H
#include <map>
class TriangleOptionsParser : public yyFlexLexer {
  public:
  TriangleOptionsParser() : _minangle(20),
                            report(""), _action(DEFAULT), at_char(0) {init();}  
  ~TriangleOptionsParser() {}
  int yylex();
  inline void parse();
  inline void parse(const char*);

  const char* options(); 
  std::string info(const char *flags);
  
  bool getOption(const std::string name) { return flags[name].second; }
  void setOption(const std::string name, bool v) { flags[name].second = v;}
  
  double getMinAngle() const { return _minangle;}
  void setMinAngle(const double angle) { _minangle = angle;}
  
  double getMaxArea() const {return _maxarea;}
  void setMaxArea(const double area) { _maxarea = area;}
  
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
