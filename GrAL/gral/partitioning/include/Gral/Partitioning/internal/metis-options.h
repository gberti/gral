#ifndef NMWR_GB_PARTITIONING_METIS_OPTIONS_H
#define NMWR_GB_PARTITIONING_METIS_OPTIONS_H

// $LICENSE 

/*! \file
 */


/*! \brief  Data structure for using Metis partitioning routines with 
    different options.
    \ingroup metispartitioninginternal

    \see The $Metis homepage.
    \author Holger Kache, Guntram Berti
*/ 

class metis_options  {

public: 
//------- DATA --------------
  //! Dependent on Metis algorithm selected
  int opt[5];
  //! 0: use recursive multilevel, 1: use kway multilevel, 2: select 
  int algorithm_flag;

//------- CONSTRUCTION ------
   metis_options();

//------- FUNCTIONS --------- 
  //! choose matching type: Random matching
  void set_rm();
  //! choose matching type: Heavy edge matching
  void set_hem();
  //! choose matching type: Sorted heavy edge matching
  void set_shem();

  void set_rbr();
  void set_gbr();

  void use_pmetis();
  void use_kmetis();

//------- DESTRUCTION -------
  ~metis_options();

};

#endif
