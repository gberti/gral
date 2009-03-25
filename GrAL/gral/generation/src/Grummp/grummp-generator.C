
/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Generation/Grummp/grummp-generator.h"

#include "GR_AdaptPred.h"
#include "GR_config.h"
#include "GR_AdaptPred.h"
#include "GR_Bdry2D.h"
#include "GR_Quality.h"
#include "GR_misc.h"

#define FILE_NAME_LEN 1024

namespace GrAL {

namespace grummp_generator {

void 
TriGenerator::triangulate(std::string const &strBaseFileName, 
                          std::string const &options, 
                          Mesh2DAdapter &output) {
  std::string strQualFileName;

  const char *strCanonicalName="tri";
  
  iMessageStdoutLevel = -1;
  bool qAdaptPred_cache = qAdaptPred;
  qAdaptPred = false;

  vGRUMMPInit(strCanonicalName);
  
  /* setup qAdaptPred
   *       qAllowBdryChanges
           qCircleEncroaching
           dGrading
           qAnisoCoarsening
           iQualMeasure
           dScale
   */
  // File name manipulation
  strQualFileName = strQualFileName + ".qual",

  // Open input and log files
  vOpenMessageFile(const_cast<char*>(strBaseFileName.c_str()));
  vMessage(1, "Mesh quality output file: %s\n", strQualFileName.c_str());

  // Reading a bdry file and creating a mesh from that
  Bdry2D B2D(strBaseFileName.c_str(), false);
  pM2D = new Mesh2D(B2D);
  pM2D->vInitLengthScale(1.0/dScale, 1.0/dGrading);
    
  vMessage(2, "Number of verts in initial triangulation:  %d\n",
	     pM2D->iNumVerts());
  vMessage(1, "Number of triangles in initial triangulation:  %d\n",
	     pM2D->iNumCells());

  //-----------------------
  assert(pM2D->qValid());

  Quality Qual(pM2D, iQualMeasure);
  Qual.vEvaluate();

  if (qAllowBdryChanges)
    pM2D->vAllowBdryChanges();
  else
    pM2D->vDisallowBdryChanges();    
      
  int iNumPasses = 0;
  pM2D->vCheckForSmallAngles();
      
  pM2D->vSetCircleEncroaching(qCircleEncroaching);
  pM2D->vCheckForLargeAngles();
  pM2D->vInitLengthScale(1.0/dScale, 1.0/dGrading);
  pM2D->vSetLSChecking(true);
      
  pM2D->qRuppert(0);
      
  Qual.vEvaluate();
      
  pM2D->vSetSmoothingThreshold(40);
  iNumPasses = 1;
  vMessage(1,"Number of triangles after refinement:  %d\n",
	     pM2D->iNumCells());

  if (iNumPasses > 0) {
    // Do some smoothing if requested
    vMessage(1, "Smoothing vertices...\n");
    for (int ii = iPreviousPasses; ii < iNumPasses; ii++) {
      vMessage(2, "Pass %d of %d...\n", ii+1, iNumPasses);
      pM2D->iSmooth(1);
      if ((ii+1)%3 == 0) {
	      // After a few smoothing passes, try swapping to see if that
	      // helps any.
	      pM2D->iSwap();
      }
    }
    Qual.vEvaluate();
  }
  
  // Quality output
  Qual.vWriteToFile(const_cast<char*>(strQualFileName.c_str()));
  
  //vCheckEdgeLengths(pM2D);

  if (pM2D != NULL) {
    output.steal(pM2D);
  }
  qAdaptPred = qAdaptPred_cache;
}; 

void 
TriGenerator::triangulate(Boundary2D const& boundary, 
                          std::string const &options, 
                          Mesh2DAdapter &output) {
  std::string strQualFileName;
  std::cerr << "IN triangulate\n";
  const char *strCanonicalName="tri";
  
  iMessageStdoutLevel = -1;
  bool qAdaptPred_cache = qAdaptPred;
  qAdaptPred = false;

  vGRUMMPInit(strCanonicalName);
  
  std::cerr << "Chekckpoint1\n";
  /* setup qAdaptPred
   *       qAllowBdryChanges
           qCircleEncroaching
           dGrading
           qAnisoCoarsening
           iQualMeasure
           dScale
   */
  // File name manipulation
  strQualFileName = strQualFileName + ".qual",

  // Open input and log files
  vOpenMessageFile("tri_messages");
  vMessage(1, "Mesh quality output file: %s\n", strQualFileName.c_str());

  // Reading a bdry file and creating a mesh from that
  Bdry2D B2D;
  boundary.SetupBdry2D(B2D); 

  pM2D = new Mesh2D(B2D);
  pM2D->vInitLengthScale(1.0/dScale, 1.0/dGrading);
    
  vMessage(2, "Number of verts in initial triangulation:  %d\n",
	     pM2D->iNumVerts());
  vMessage(1, "Number of triangles in initial triangulation:  %d\n",
	     pM2D->iNumCells());

  //-----------------------
  assert(pM2D->qValid());

  Quality Qual(pM2D, iQualMeasure);
  Qual.vEvaluate();

  if (qAllowBdryChanges)
    pM2D->vAllowBdryChanges();
  else
    pM2D->vDisallowBdryChanges();    
      
  int iNumPasses = 0;
  pM2D->vCheckForSmallAngles();
      
  pM2D->vSetCircleEncroaching(qCircleEncroaching);
  pM2D->vCheckForLargeAngles();
  pM2D->vInitLengthScale(1.0/dScale, 1.0/dGrading);
  pM2D->vSetLSChecking(true);
      
  pM2D->qRuppert(0);
      
  Qual.vEvaluate();
      
  pM2D->vSetSmoothingThreshold(40);
  iNumPasses = 1;
  vMessage(1,"Number of triangles after refinement:  %d\n",
	     pM2D->iNumCells());

  if (iNumPasses > 0) {
    // Do some smoothing if requested
    vMessage(1, "Smoothing vertices...\n");
    for (int ii = iPreviousPasses; ii < iNumPasses; ii++) {
      vMessage(2, "Pass %d of %d...\n", ii+1, iNumPasses);
      pM2D->iSmooth(1);
      if ((ii+1)%3 == 0) {
	      // After a few smoothing passes, try swapping to see if that
	      // helps any.
	      pM2D->iSwap();
      }
    }
    Qual.vEvaluate();
  }
  
  // Quality output
  Qual.vWriteToFile(const_cast<char*>(strQualFileName.c_str()));
  
  //vCheckEdgeLengths(pM2D);

  if (pM2D != NULL) {
    output.steal(pM2D);
  }
  qAdaptPred = qAdaptPred_cache;
}; 




} // namespace grummp_generator

} // namespace GrAL

