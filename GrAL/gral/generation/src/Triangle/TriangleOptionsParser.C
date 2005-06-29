#include "Gral/Generation/Triangle/TriangleOptionsParser.h"
#include "Utility/pre-post-conditions.h"
#include <stdlib.h> /* for atof and atoi */
#include <stdio.h>

void 
TriangleOptionsParser::invalid_input(const char *t) {
  std::ostringstream os;
  os << "Invalid character \'" << t << "\' at index " << at_char
     << " in "<< options_text;
  throw TriangleOptionsException(os.str());
}


void
TriangleOptionsParser::plg() {
  if (_action == REPORT) {
     report +="Triangulates a Planar Straight Line Graph .poly"
                      "file.\n";
  } else {
    flags["plg"].second = true;
  }  
}

void
TriangleOptionsParser::refine() {
  if (_action == REPORT) {
    report += "Refines a previously generated mesh\n";
  } else {
    flags["refine"].second = true;
  }
}

void
TriangleOptionsParser::quality() {
  if (_action == REPORT) {
    report += "Quality mesh generation.  A minimum angle may be specified.\n";
  } else {
    flags["quality"].second = true;
  }
}

void
TriangleOptionsParser::quality(const char *text) {
  if (_action == REPORT) {
    report += "Quality mesh generation.  A minimum angle may be specified.\n";
  } else {
    flags["quality"].second = true;
    _minangle = atof(text+1);
  }
}

void
TriangleOptionsParser::maxarea() {
  if (_action == REPORT) {
    report += "Applies a maximum triangle area constraint\n";
  } else {
    flags["maxarea"].second = true;
  }
}

void
TriangleOptionsParser::maxarea(const char* text) {
  if (_action == REPORT) {
    report += "Applies a maximum triangle area constraint\n";
  } else {
    flags["globalmaxarea"].second = true;
    _maxarea = atof(text+1); 
  }
}

void
TriangleOptionsParser::attributes() {
  if (_action == REPORT) {
    report += "Applies a maximum triangle area constraint.\n";
  } else {
    flags["attributes"].second = true;
  }
}

void
TriangleOptionsParser::convexhull() {
  if (_action == REPORT) {
    report += "Encloses the convex hull with segments.\n";
  } else {
    flags["convexhull"].second = true;
  }
}

void
TriangleOptionsParser::weighted() {
  if (_action == REPORT) {
    report += "Weighted Delaunay triangulation\n";
  } else {
    flags["weighted"].second = true;
  }
}

void
TriangleOptionsParser::regular() {
  if (_action == REPORT) {
     report += "Regular triangulation (lower hull of a  height field)\n";
  } else {   
    flags["regular"].second = true;
  }
}

void
TriangleOptionsParser::jettison() {
  if (_action == REPORT) {
     report += "Jettison unused vertices from output node file.\n";
  } else {
    flags["jettison"].second = true;
  }
}

void
TriangleOptionsParser::edgelist() {
  if (_action == REPORT) {
     report += "Generates an edge list\n";
  }
}

void
TriangleOptionsParser::voronoi() {
  if (_action == REPORT) {
    report += "Generates a Voronoi diagram\n";
  }
}

void
TriangleOptionsParser::neighbours() {
  if (_action == REPORT) {
    report += "Generates a list of triangle neighbors\n";
  }
}

void
TriangleOptionsParser::geomview() {
  if (_action == REPORT) {
     report += "Generates an .off file for Geomview.\n";
  }
}

void
TriangleOptionsParser::noboundinfo() {
  if (_action == REPORT) {
    report += "Suppresses output of boundary information\n";
  }
}

void
TriangleOptionsParser::nopolyfile() {
  if (_action == REPORT) {
    report += "Suppresses output of .poly file.\n";
  }
}

void
TriangleOptionsParser::nonodefile() {
  if (_action == REPORT) {
    report += "Suppresses output of .node file.\n";
  }
}

void
TriangleOptionsParser::noelefile() {
  if (_action == REPORT) {
     report += "Suppresses output of .ele file.\n";
  }
}

void
TriangleOptionsParser::noiterinfo() {
  if (_action == REPORT) {
     report += "Suppresses mesh iteration numbers\n";
  }
}

void
TriangleOptionsParser::ignoreholes() {
  if (_action == REPORT) {
   report += "Ignores holes in .poly file.\n";
  } else {
    flags["noholes"].second = true;
  }
}

void
TriangleOptionsParser::noexactarithm() {
  if (_action == REPORT) {
    report += "Suppresses use of exact arithmetic\n";
  } else {
    flags["noexact"].second = true;
  }
}

void
TriangleOptionsParser::zerobasednum() {
  if (_action == REPORT) {
    report += "Numbers all items starting from zero  (rather than one)\n";
  }
}

void
TriangleOptionsParser::secondorder() {
  if (_action == REPORT) {
    report += "2 Generates second-order subparametric elements\n";
  } else {
    flags["secondorder"].second = true;
  }
}

void
TriangleOptionsParser::noboundsplit() {
  if (_action == REPORT) {
   report += "Suppresses boundary segment splitting\n";
  } else {
    flags["noboundsplit"].second = true;
  }
}

void
TriangleOptionsParser::steiner() {
  if (_action == REPORT) {
    report += "Specifies maximum number of added Steiner points\n";
  } else {
    flags["steiner"].second = true;
  }
}

void
TriangleOptionsParser::steiner(const char *text) {
  if (_action == REPORT) {
    report += "Specifies maximum number of added Steiner points\n";
  } else {
    flags["steiner"].second = true;
    _nsteiner = atoi(text+1);
  }
}



void
TriangleOptionsParser::equatorialcircles() {
  if (_action == REPORT) {
     report += "Uses equatorial circles, not equatorial lenses\n";
  } else {
    flags["nolense"].second = true;
  }
}

void
TriangleOptionsParser::incremental() {
  if (_action == REPORT) {
    report += "Uses incremental method, rather than divide-and-conquer\n";
  } else {
    flags["incremental"].second = true;
  }
}

void
TriangleOptionsParser::sweepline() {
  if (_action == REPORT) {
   report += "Uses Fortune's sweepline algorithm, rather than  d-and-c\n";
  } else {
    flags["sweepline"].second = true;
  }
}

void
TriangleOptionsParser::verticalcuts() {
  if (_action == REPORT) {
    report += "Uses vertical cuts only, rather than  alternating cuts\n";
  } else {
    flags["verticalcuts"].second = true;
  }
}

void
TriangleOptionsParser::forcebysplit() {
  if (_action == REPORT) {
   report += "Force segments into mesh by splitting (instead of using CDT)\n";
  } else {
    flags["forcebysplit"].second = true;
  }
}

void
TriangleOptionsParser::checkconsistency() {
  if (_action == REPORT) {
   report += "Check consistency of final mesh\n";
  } else {
    flags["check"].second = true;
  }
}

void
TriangleOptionsParser::userconstraint() {
  if (_action == REPORT) {
   report += "Applies a user-defined triangle constraint.\n";
  } else {
    flags["userconstraint"].second = true;
  }
}

void
TriangleOptionsParser::inellipse() {
  if (_action == REPORT) {
   report += "Applies a user-defined distance metric in incircle test.\n";
  } else {
    flags["inellipse"].second = true;
  }
}


void
TriangleOptionsParser::parseminus() {
   
}

void
TriangleOptionsParser::quiet() {
  if (_action == REPORT) {
     report += "Quiet:  No terminal output except errors.\n";
  } else {
    flags["quiet"].second = true;
  }
}

void
TriangleOptionsParser::verbose() {
  if (_action == REPORT) {
     report += "Verbose:  Detailed information on what I'm doing.\n";
  } else {
    flags["verbose"].second = true;
  }
}

const char* TriangleOptionsParser::GetOptions() {
  char* o=s;
  char* t;
  char tmp[16];
  tmp[15]='\0';
  for (flags_container::iterator i = flags.begin(); i != flags.end(); ++i) {
      switch(i->second.first) {
         case 'q':
           if (i->second.second) {
             *o++ = 'q';  
             snprintf(tmp, 15, "%-15.6f", _minangle);
             t = tmp;
             while (*t != ' ' && *t != '\0') {
                *o++ = *t++;
             }
           }
           break;
         case 'S':
           if (i->second.second) {
             *o++ = 'S';  
             snprintf(tmp, 15, "%-15d", _nsteiner);
             t = tmp;
             while (*t != ' ' && *t != '\0') {
                *o++ = *t++;
             }
           }
           break;
         case 'G':
           if (i->second.second) {
             *o++ = 'a';  
             snprintf(tmp, 15, "%-15.6f", _maxarea);
             t = tmp;
             while (*t != ' ' && *t != '\0') {
                *o++ = *t++;
             }
           }
           break;
         case 'o':
           if (i->second.second) {
           *o++ = 'o';
           *o++ = '2';
           }
           break;
         default:  
           if (i->second.second) {
             *o++ = i->second.first;
           }
      }
  }
  *o = '\0';
  return s;
}

std::string TriangleOptionsParser::Info(const char *flags) {
  _action = REPORT;
  report = "";
  Parse(flags);
  return report;
}

void TriangleOptionsParser::init() {
  typedef std::pair<const char, bool> fvp;
  typedef std::pair<const std::string, fvp> fv; 
  flags.insert(fv("plg",  fvp('p', false)));
  flags.insert(fv("refine",  fvp('r', false)));
  flags.insert(fv("quality",  fvp('q', true)));
  flags.insert(fv("maxarea", fvp('a', false)));
  flags.insert(fv("globalmaxarea", fvp('G', false)));
  flags.insert(fv("attributes", fvp('A', false)));
  flags.insert(fv("convexhull", fvp('c', false)));
  flags.insert(fv("weighted", fvp('w', false)));
  flags.insert(fv("regular", fvp('W', false)));
  flags.insert(fv("jettison", fvp('j',false)));
  flags.insert(fv("noholes", fvp('O', false)));
  flags.insert(fv("noexact", fvp('X', false)));
  flags.insert(fv("secondorder", fvp('o', false)));
  flags.insert(fv("nolenses", fvp('L', false)));
  flags.insert(fv("inclremental", fvp('i',false)));
  flags.insert(fv("sweepline", fvp('F', false)));
  flags.insert(fv("verticalcuts", fvp('l',false)));
  flags.insert(fv("forcebysplit", fvp('s',false)));
  flags.insert(fv("noboundsplit", fvp('Y',false)));
  flags.insert(fv("check", fvp('C', false)));
  flags.insert(fv("quiet", fvp('Q', true)));
  flags.insert(fv("verbose", fvp('V', false)));
  flags.insert(fv("userconstraint", fvp('u', false)));
  flags.insert(fv("inellipse", fvp('T', false))); 
}
