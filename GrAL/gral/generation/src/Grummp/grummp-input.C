/*
 * Bodies of the functions:
 *   SetupPolyline
 *   SetupCircle
 *   SetupArc
 *   SetupBezier
 *   SetupSpline
 *   SetupBdry2D
 * are almost verbatim copied from GRUMMP source (file Bdry2D.cxx).
 *
 * This file is copyrighted as stated in GRUMMP license.
 */
#include <iostream>
#include "Gral/Generation/Grummp/grummp-input.h"
#include "GR_Bdry2D.h"
#include "GR_BdryPatch2D.h"

namespace GrAL {

namespace grummp_generator {

Boundary2D::BdryPatch2D::~BdryPatch2D() {
  // empty  
} 

Boundary2D::Polyline::~Polyline() {
  // empty  
}

void
Boundary2D::SetupBdry2D(::Bdry2D &bdry) const {
  int i=0; 
  int  iBdryPatch = 0;


  std::cerr << "Before Set size\n";
  bdry.vSetSize (NumOfPatches(), NumOfPoints());

  std::cerr << "Set size\n";
  for (PointIterator p=FirstPoint(); 
       p != EndPoint(); ++p, ++i) {
       bdry.vAddPoint((*p).coords());
       bdry.vSetBdryPoint(i, false);
  }

  std::cerr << "Added points size\n";
  for (PolylineIterator p=FirstPolyline(); p !=EndPolyline(); ++p) {
    SetupPolyline(bdry, *p, iBdryPatch);
  }

  std::cerr << "Added polylines size\n";
  for (CircleIterator p=FirstCircle(); p !=EndCircle(); ++p) {
    SetupCircle(bdry, *p, iBdryPatch);
  }

  std::cerr << "Added circles size\n";
  for (ArcIterator p=FirstArc(); p !=EndArc(); ++p) {
    SetupArc(bdry, *p, iBdryPatch);
  }

  std::cerr << "Added arcs size\n";
  for (BezierIterator p=FirstBezier(); p !=EndBezier(); ++p) {
    SetupBezier(bdry, *p, iBdryPatch);
  }
 
  std::cerr << "Added  beziers size\n";
  for (SplineIterator p=FirstSpline(); p !=EndSpline(); ++p) {
    SetupSpline(bdry, *p, iBdryPatch);
  }
 
  std::cerr << "Added splines size\n";
  bdry.vOrderConnectivityInfo();
  // Find small angles
  bdry.vFindSmallAngles();

  std::cerr << "Finished setupe\n";
}

void
Boundary2D::SetupCircle(::Bdry2D &bdry, 
                          Boundary2D::Circle const & circle, 
                          int &iBdryPatch) const {
	  int iBCL, iBCR, iRegL, iRegR;
  
    circle.SetupPatchMarkers(iBCL, iBCR, iRegL, iRegR); 

	  // Initialize the arc...
	  ::BdryArc *pBA = new ::BdryArc (&bdry, iBCL, iBCR, iRegL, iRegR,
				      circle.getRadius(), circle.getCenter());
	  
	  bdry.vAddPatchToList (pBA);
	  
	  // Must still increment..
	  iBdryPatch++;	  			
}

void
Boundary2D::SetupArc(::Bdry2D &bdry, 
                          Boundary2D::Arc const & arc, 
                          int &iBdryPatch) const {
	  int iBCL, iBCR, iRegL, iRegR;
  
    arc.SetupPatchMarkers(iBCL, iBCR, iRegL, iRegR); 

	  double dRadius = arc.getRadius();
	  int    iV0 = arc.getStartPoint(); 
    int    iV1 = arc.getEndPoint();


	  // Initialize the arc...
	  ::BdryArc *pBA = new ::BdryArc (&bdry, iBCL, iBCR, iRegL, iRegR,
				      iV0, iV1, dRadius);
	  
	  bdry.vAddPatchToList (pBA);
	  
	  // Must still increment..
	  iBdryPatch++;	  			

    double adPt[2], adVertPt[2];
	  double adNorm;
	  double dTanX, dTanY;
	  ::structVertConnect sVConnect;
	    		
	  // Might as well build the connectivity list as we go along, right?
	  sVConnect.iPatch = bdry.iNumPatches()-1;
	  sVConnect.iVert  = iV0;
	  bdry.vSetBdryPoint(iV0, true);
	  // Get the tangent vector for iV0
	  pBA->vPointAtArcLengthRatio(0.01, adPt);
	  bdry.vGetPoint(iV0, adVertPt);
	  dTanX = adPt[0] - adVertPt[0];
	  dTanY = adPt[1] - adVertPt[1];
	  adNorm = sqrt(dTanX*dTanX + dTanY*dTanY);
	  dTanX /= adNorm;
	  dTanY /= adNorm;
	  sVConnect.dAngle = atan2(dTanY, dTanX);
	  // Add it to list
	  bdry.vAddToConnectivityList(sVConnect);
			
	  // Do the same for iV1 too
	  sVConnect.iPatch = iBdryPatch-1;
	  sVConnect.iVert  = iV1;
	  bdry.vSetBdryPoint(iV1, true);
	  // Get the tangent vector for iV0
	  pBA->vPointAtArcLengthRatio(0.99, adPt);
	  bdry.vGetPoint(iV1, adVertPt);
	  dTanX = adPt[0] - adVertPt[0];
	  dTanY = adPt[1] - adVertPt[1];
	  adNorm = sqrt(dTanX*dTanX + dTanY*dTanY);
	  dTanX /= adNorm;
	  dTanY /= adNorm;
	  sVConnect.dAngle = atan2(dTanY, dTanX);
	  // Add it to list
	  bdry.vAddToConnectivityList(sVConnect);
}

void
Boundary2D::SetupBezier(::Bdry2D &bdry, 
                          Boundary2D::Bezier const & bezier, 
                          int &iBdryPatch) const {
	  int iBCL, iBCR, iRegL, iRegR;
  
    bezier.SetupPatchMarkers(iBCL, iBCR, iRegL, iRegR); 

	  int    iV0 =bezier.getStartPoint(); 
    int    iV1 =bezier.getEndPoint(); 
    int    iC1 =bezier.getCtrl1(); 
    int    iC2 =bezier.getCtrl2();

    
   	// Initialize the curve...
	  BdryBezier2D *pBB = new BdryBezier2D (&bdry, iBCL, iBCR, iRegL, iRegR,
				      iV0, iV1, iC1, iC2);
	
	  bdry.vAddPatchToList (pBB);
	  	
	  // Must still increment..
	  iBdryPatch++;	  			
	
	  // Make sure connectivity is stored too...
	  double adTan[2];
	  structVertConnect sVConnect;
	    		
	  sVConnect.iPatch = bdry.iNumPatches()-1;
	  sVConnect.iVert  = iV0;
	  bdry.vSetBdryPoint(iV0, true);
	  // Get the tangent vector for iV0
	  pBB->vTangentAtArcLengthRatio(0.0, adTan);
	  sVConnect.dAngle = atan2(adTan[1], adTan[0]);
	  // Add it to list
	  bdry.vAddToConnectivityList(sVConnect);
	  
	  // Do the same for iV1
	  sVConnect.iVert  = iV1;
	  bdry.vSetBdryPoint(iV1, true);
	  // Get the tangent vector for iV1
	  pBB->vTangentAtArcLengthRatio(1.0, adTan);
	  sVConnect.dAngle = atan2(-adTan[1], -adTan[0]);
	  // Add it to list
	  bdry.vAddToConnectivityList(sVConnect);
} 

void
Boundary2D::SetupSpline(::Bdry2D &bdry, 
                          Boundary2D::Spline const & spline, 
                          int &iBdryPatch) const {
	  // Interpolated splines...
	  // format is spline r 1 b 1 iNumPoints iPt0 iPt1 iPt2 ... iPtNumPoints-1
		
	  int		iNP;
	  List<int> LiAllPoints;
	  int iBCL, iBCR, iRegL, iRegR;
  
    spline.SetupPatchMarkers(iBCL, iBCR, iRegL, iRegR); 
    typedef Boundary2D::Polyline::PointHandleIterator handle_iterator;
  
    handle_iterator p = spline.FirstPointHandle();
   
	  iNP = spline.NumOfPoints();

	  // now get all the points
	  for (handle_iterator p = spline.FirstPointHandle(); p != spline.EndPointHandle(); 
         ++p) {
	    LiAllPoints.vAppendItem(*p);
	  }
	  
	  // Initialize the curve...
	  BdryInterpolated2D *pBS = new BdryInterpolated2D (&bdry, iBCL,
							    iBCR, iRegL,
							    iRegR,
							    LiAllPoints); 

	  // Don't add it to the list just yet...
	  //	  vAddPatchToList (pBS);
	  	
	  // Must still increment..
	  iBdryPatch++;	  			
	
	  // Splines are special because it is possible they will be
	  // transformed into several splines. So we have to do this
	  // first... 
	
	  double dTotalChange = pBS->dTotalOrientationChange();		
	  int iNumSplines = (int)(floor(dTotalChange)) + 1;
	  double dChange = dTotalChange/iNumSplines;
	  double dt = 0.0;
	  List<double> LdNewPoints;
	  
	  LdNewPoints.vClear();
	
	  // Find these points..
	  int iS;
	  for (iS = 1; iS < iNumSplines; iS++) {
	    double dEnd = pBS->dArcLengthAtOrientationChange(dChange, dt);
	    dt = dEnd;
	    LdNewPoints.vAppendItem(dt);
	  }
	  
	  // LiAllPoints contains all the control points...
	  // first one is at arclength 0.0, next one at 1.0,
	  // and so on.
	  
	  // This contains a list of the arclengths used for the new
	  // splines i.e. LLdPointsInSplines[0] contains a list of
	  // arclengths used for the new spline 0.
	  // LLdPointsInSplines[1][3] contains the 4th arclength for
	  // spline 1, etc..
	  
	  List < List <double> > LLdPointsInSplines;
	  List < List <double> > LLdCoeffsInSplines;
	  List < List <int>    > LLiPtIdxInSplines;
	  LLdPointsInSplines.vClear();
	  
	  List <double> LdSplinePoints;
	  List <double> LdCoeffs;
	  List <int>    LiIndex;
	  double dLastItem = 0.0;
	  
	  // Fill the list of splines...
	  for (iS = 0; iS < iNumSplines; iS++) {
	    // Clear the list
	    LdSplinePoints.vClear();
	    // Add the first item
	    LdSplinePoints.vAppendItem(dLastItem);
	    double dNextPoint;
	    int j = 0;
	    if (iS < iNumSplines-1){
	      dNextPoint = LdNewPoints[iS];
	    }
	    else {
	      dNextPoint = pBS->dArcLength();
	    }
	    if (floor(dLastItem) < floor(dNextPoint)) {
				// insert all the control points in between..
	      for (j = (int)(floor(dLastItem))+1;
		   j <= (int)(floor(dNextPoint)); j++) { 
		LdSplinePoints.vAppendItem((double)j);
	      }
	    }
	    dLastItem = dNextPoint;
	    if (iFuzzyComp(dNextPoint, (double)(j-1)) != 0) {
				// Add the next point
	      LdSplinePoints.vAppendItem(dNextPoint);
	    }
	    // Add that to the list
	    LLdPointsInSplines.vAppendItem(LdSplinePoints);
	  }
	  
	  // Now build the list of coefficients..
	  for (iS = 0; iS < LLdPointsInSplines.iLength(); iS++) {
	    // Clear the list...
	    LdCoeffs.vClear();
	    for (int iP = 0; iP < LLdPointsInSplines[iS].iLength()-1; iP++) {
	      double dFirst = LLdPointsInSplines[iS][iP];
	      int iFirst = (int)(floor(dFirst));
	      // Now add the coefficients..
	      double d1, d2, d3, d4, d5, d6, d7, d8;
	      pBS->vGetSplineCoefficients(iFirst, &d1, &d2, &d3, &d4,
					  &d5, &d6, &d7, &d8); 
	      // Add them...
	      LdCoeffs.vAppendItem(d1);
	      LdCoeffs.vAppendItem(d2);
	      LdCoeffs.vAppendItem(d3);
	      LdCoeffs.vAppendItem(d4);
	      LdCoeffs.vAppendItem(d5);
	      LdCoeffs.vAppendItem(d6);
	      LdCoeffs.vAppendItem(d7);				
	      LdCoeffs.vAppendItem(d8);				
	    }
	    LLdCoeffsInSplines.vAppendItem(LdCoeffs);
	  }
	  
	  // Now build the list of points indices..
	  for (iS = 0; iS < LLdPointsInSplines.iLength(); iS++) {
	    LiIndex.vClear();			
	    for (int iP = 0; iP < LLdPointsInSplines[iS].iLength(); iP++) {
	      double dAL = LLdPointsInSplines[iS][iP];
	      double dMantissa = dAL - (floor(dAL));
	      if (iFuzzyComp(dMantissa, 0.0) == 0) {
					// That point exists..
					int iAL = (int)(floor(dAL));
					LiIndex.vAppendItem(LiAllPoints[iAL]);
	      }
	      else {
					if (iP != 0) {
		  			// We need to create it.
		  			// Get the coords
		  			double adC[2];
		  			pBS->vPointAtArcLength(dAL, adC);
		  			int iIndex = bdry.iNumPoints();
		  			bdry.vAddPoint(adC);
		  			LiIndex.vAppendItem(iIndex);
					}
					else {
		  			// That point has already been created
		  			int iL = LLiPtIdxInSplines[iS-1].iLength();
		  			LiIndex.vAppendItem(LLiPtIdxInSplines[iS-1][iL-1]);
					}
	      }
	    }
	    LLiPtIdxInSplines.vAppendItem(LiIndex);
	  }		
	  // Check to see what we got..
	  for (int ii = 0; ii < LLdPointsInSplines.iLength(); ii++) {
	    List <double> LdP = LLdPointsInSplines[ii];
	    for (int jj = 0; jj < LdP.iLength(); jj++) {
	      double adPt[2];
	      bdry.vGetPoint(LLiPtIdxInSplines[ii][jj], adPt);
	    }
	  }
	  
	  int iLastSpline = -1;
	  double dLastCut = 0.0;
	  
	  // Now build each spline...
	  for (int iP = 0; iP < LdNewPoints.iLength(); iP++) {
	    // Split it..
	    double d11, d12, d13, d14, d15, d16, d17, d18;			
	    double d21, d22, d23, d24, d25, d26, d27, d28;
	    int iSpline;
	    double dLeft, dNewLastCut;
	    iSpline = (int)(floor(LdNewPoints[iP]));
	    dLeft = LdNewPoints[iP] - (double)iSpline;
	    dNewLastCut = dLeft;
	    if (iLastSpline == iSpline) {
	      dLeft = (dLeft - dLastCut)/(1.0 - dLastCut);
	    }
	    iLastSpline = iSpline;
	    dLastCut = dNewLastCut;
	    // Spline index...
	    int iSi = iP;
	    if (iFuzzyComp(dLeft, 0.0) == 0) {
				dLeft = 1.0;
				iSi--;
	    }
	    // Get the last coefficients from Spline iP..
	    int iBase = LLdCoeffsInSplines[iSi].iLength()/8;
	    iBase--;
	    iBase *= 8;	
	    d11 = LLdCoeffsInSplines[iSi][iBase+0];
	    d12 = LLdCoeffsInSplines[iSi][iBase+1];
	    d13 = LLdCoeffsInSplines[iSi][iBase+2];
	    d14 = LLdCoeffsInSplines[iSi][iBase+3];
	    d15 = LLdCoeffsInSplines[iSi][iBase+4];
	    d16 = LLdCoeffsInSplines[iSi][iBase+5];
	    d17 = LLdCoeffsInSplines[iSi][iBase+6];
	    d18 = LLdCoeffsInSplines[iSi][iBase+7];
	    // Get also the first coefficients from spline iP+1
	    d21 = LLdCoeffsInSplines[iSi+1][0];
	    d22 = LLdCoeffsInSplines[iSi+1][1];
	    d23 = LLdCoeffsInSplines[iSi+1][2];
	    d24 = LLdCoeffsInSplines[iSi+1][3];
	    d25 = LLdCoeffsInSplines[iSi+1][4];
	    d26 = LLdCoeffsInSplines[iSi+1][5];
	    d27 = LLdCoeffsInSplines[iSi+1][6];
	    d28 = LLdCoeffsInSplines[iSi+1][7];
	    // Split..
	    pBS->vSplitSplineAtArcLength(iSpline, dLeft, &d11, &d12,
					 &d13, &d14, &d15, &d16, &d17,
					 &d18, &d21, &d22, &d23, &d24,
					 &d25, &d26, &d27, &d28); 
	    // And put that back in the lists...
	    LLdCoeffsInSplines[iSi][iBase+0] = d11;
	    LLdCoeffsInSplines[iSi][iBase+1] = d12;
	    LLdCoeffsInSplines[iSi][iBase+2] = d13;
	    LLdCoeffsInSplines[iSi][iBase+3] = d14;
	    LLdCoeffsInSplines[iSi][iBase+4] = d15;
	    LLdCoeffsInSplines[iSi][iBase+5] = d16;
	    LLdCoeffsInSplines[iSi][iBase+6] = d17;
	    LLdCoeffsInSplines[iSi][iBase+7] = d18;
	    // And this one too....
	    LLdCoeffsInSplines[iSi+1][0] = d21;
	    LLdCoeffsInSplines[iSi+1][1] = d22;
	    LLdCoeffsInSplines[iSi+1][2] = d23;
	    LLdCoeffsInSplines[iSi+1][3] = d24;
	    LLdCoeffsInSplines[iSi+1][4] = d25;
	    LLdCoeffsInSplines[iSi+1][5] = d26;
	    LLdCoeffsInSplines[iSi+1][6] = d27;
	    LLdCoeffsInSplines[iSi+1][7] = d28;			
	  }
	  
	  // I think we are ready to create them now...
	  
	  for (iS = 0; iS < iNumSplines; iS++) {
	    BdryInterpolated2D *pNewS;
	    List<int> LiSplinePoints = LLiPtIdxInSplines[iS];
	    LdCoeffs = LLdCoeffsInSplines[iS];
	    
	    // Create the patch
	    pNewS = new BdryInterpolated2D(&bdry, iBCL, iBCR, iRegL,
					   iRegR, LiSplinePoints); 
	    
	    bdry.vAddPatchToList(pNewS);
	    
	    // Change the coefficients..
	    for (int iNewS = 0; iNewS < pNewS->iNumSplines(); iNewS++) {
	      int iBase = 8*iNewS;
	      pNewS->vSetSplineCoefficients(iNewS, LdCoeffs[iBase],
					    LdCoeffs[iBase+1],
					    LdCoeffs[iBase+2],
					    LdCoeffs[iBase+3],
					    LdCoeffs[iBase+4],
					    LdCoeffs[iBase+5],
					    LdCoeffs[iBase+6],
					    LdCoeffs[iBase+7]); 
	    }
			
	    pNewS->vRefresh();
	    double adB[2], adE[2];
	    pNewS->vGetBeginPoint(adB);
	    pNewS->vGetEndPoint(adE);
	    
	    for (int iRt = 0; iRt < pNewS->iNumOrderedRoots(); iRt++) {
	      double dRoot = pNewS->dGetOrderedRoot(iRt);
	      double adC[2];
	      pNewS->vPointAtArcLength(dRoot, adC);
	    }
			
	    // Make sure connectivity is stored too...
	    double adTan[2];
	    structVertConnect sVConnect;
	    
	    sVConnect.iPatch = bdry.iNumPatches()-1;
	    sVConnect.iVert  = LiSplinePoints[0];
	    bdry.vSetBdryPoint(LiSplinePoints[0], true);
	    // Get the tangent vector for iV0
	    pNewS->vTangentAtArcLengthRatio(0.0, adTan);
	    sVConnect.dAngle = atan2(adTan[1], adTan[0]);
	    // Add it to list
	    bdry.vAddToConnectivityList(sVConnect);
	    
	    // Do the same for iV1
	    sVConnect.iVert  = LiSplinePoints[LiSplinePoints.iLength()-1];
	    bdry.vSetBdryPoint(LiSplinePoints[LiSplinePoints.iLength()-1], true);
	    // Get the tangent vector for iV1
	    pNewS->vTangentAtArcLengthRatio(1.0, adTan);
	    sVConnect.dAngle = atan2(-adTan[1], -adTan[0]);
	    // Add it to list
	    bdry.vAddToConnectivityList(sVConnect);
	  }
	  
	  delete pBS;
}	  

void
Boundary2D::SetupPolyline(::Bdry2D &bdry, 
                          Boundary2D::Polyline const & polyline, 
                          int &iBdryPatch) const {

  int iV0, iV1;
  int iBCL, iBCR, iRegL, iRegR;
  
  polyline.SetupPatchMarkers(iBCL, iBCR, iRegL, iRegR); 
  typedef Boundary2D::Polyline::PointHandleIterator handle_iterator;
  
  handle_iterator p = polyline.FirstPointHandle();
  iV1 = *p;
  ++p; 
  for (/*empty*/ ; p != polyline.EndPointHandle(); ++p) {
    iV0 = iV1;
    iV1 = *p;

    BdrySeg *pBS = new ::BdrySeg (&bdry, iBCL, iBCR, iRegL, iRegR, iV0, iV1); 
    bdry.vAddPatchToList (pBS);					
    // Must still increment..
    iBdryPatch++;
    double adPt[2];
    double adVertPt[2];
    double adNorm;
    double dTanX, dTanY;
    ::structVertConnect sVConnect;
    
    // Might as well build the connectivity list as we go along, right?
    sVConnect.iPatch = bdry.iNumPatches()-1;
    sVConnect.iVert  = iV0;
    bdry.vSetBdryPoint(iV0, true);
    // Get the tangent vector for iV0
    pBS->vPointAtArcLengthRatio(0.01, adPt);
    // Get iV0...
    bdry.vGetPoint(iV0, adVertPt);
    dTanX = adPt[0] - adVertPt[0];
    dTanY = adPt[1] - adVertPt[1];
    adNorm = sqrt(dTanX*dTanX + dTanY*dTanY);
    dTanX /= adNorm;
    dTanY /= adNorm;
    sVConnect.dAngle = atan2(dTanY, dTanX);
    // Add it to list
    bdry.vAddToConnectivityList(sVConnect);
		
    // Do the same for iV1 too
    sVConnect.iPatch = iBdryPatch-1;
    sVConnect.iVert  = iV1;
    bdry.vSetBdryPoint(iV1, true);
    // Get the tangent vector for iV0
    pBS->vPointAtArcLengthRatio(0.99, adPt);
    bdry.vGetPoint(iV1, adVertPt);
    dTanX = adPt[0] - adVertPt[0];
    dTanY = adPt[1] - adVertPt[1];
    adNorm = sqrt(dTanX*dTanX + dTanY*dTanY);
    dTanX /= adNorm;
    dTanY /= adNorm;
    sVConnect.dAngle = atan2(dTanY, dTanX);
    // Add it to list
    bdry.vAddToConnectivityList(sVConnect);
  } // end of polyline vertices loop
}		

void
Boundary2D::BdryPatch2D::SetupPatchMarkers(
                  int &iBCL, int &iBCR, int& iRegL, int &iRegR) const {
  if ((_leftsidetype == BOUNDARY) || (_leftsidetype == BOUNDARY)) {
  	iBCL = _leftnumber;
	  iRegL = iInvalidRegion;
  } else {
	  assert (_leftsidetype == REGION || _leftsidetype == REGION);
	  iBCL = iInvalidBC;
	  iRegL = _leftnumber;
  }
  if ((_rightsidetype == BOUNDARY) || (_rightsidetype == BOUNDARY)) {
	  iBCR = _rightnumber;
	  iRegR = iInvalidRegion;
  } else {
	  assert (_rightsidetype == REGION || _rightsidetype == REGION);
	  iBCR = iInvalidBC;
	  iRegR = _rightnumber;
  }
} 

} // namespace grummp_generator

} // namespace GrAL
