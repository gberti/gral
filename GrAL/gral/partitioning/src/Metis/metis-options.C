
// $LICENSE

/*! \file
 */

#include "Gral/Partitioning/internal/metis-options.h"

namespace GrAL {

metis_options::metis_options() {
    opt[0]=0; //defaults are used
    opt[1]=3; //default
    opt[2]=1; //default
    opt[3]=3; //default
    opt[4]=0; //default
    algorithm_flag = 2; 
};

// determines the matching type
void metis_options::set_rm() {
    opt[0]=1;
    opt[1]=1; //random matching
}; 

void metis_options::set_hem() {
    opt[0]=1;
    opt[1]=2; //heavy-edge matching
}; 
 
void metis_options::set_shem() {
    opt[0]=1;
    opt[1]=3; //shorted heavy-edge matching
};
   
  // determines the algorithm used for refinement
void metis_options::set_rbr() {
    opt[0]=1;
    opt[3]=1; //random boundary refinement
};

void metis_options::set_gbr() {
    opt[0]=1;
    opt[3]=1; //greedy boundary refinement
};

  //determines the algorithm used for partitioning
void metis_options::use_pmetis() {
    algorithm_flag = 0; //uses METIS_PartGraphRecursive
};
 
void metis_options::use_kmetis() {
    algorithm_flag = 1; //uses METIS_PartGraphKway
}; 
 
metis_options::~metis_options() { };

} // namespace GrAL 
