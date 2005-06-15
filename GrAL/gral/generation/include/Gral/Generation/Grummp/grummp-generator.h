#ifndef  GRAL_MG_GRUMMP_GENERATOR_H
#define  GRAL_MG_GRUMMP_GENERATOR_H

#include "Gral/Generation/Grummp/grummp-adapter.h"
#include "Gral/Generation/Grummp/grummp-input.h"

namespace GrAL {

namespace grummp_generator {

using grummp_adapter::Mesh2DAdapter;

class TriGenerator {
  public:
    inline TriGenerator();
    inline ~TriGenerator();
    void triangulate(std::string const &filename, 
                     std::string const &options,
                     Mesh2DAdapter &output);
    void triangulate(Boundary2D const &boundary, 
                     std::string const &options,
                     Mesh2DAdapter &output);
    void setScale(double s) { dScale = s;}
    double getScale() const {return dScale;}

    void setGrading(double g) { dGrading=g;}
    double getGrading() const {return dGrading;}
  private:
    double dScale;
    double dGrading;
    bool   qCircleEncroaching;
    int iQualMeasure; 
    int iPreviousPasses;
    bool qAllowBdryChanges;
    bool qAnisoCoarsening;
    Mesh2D *pM2D;

}; 

inline 
TriGenerator::TriGenerator() : dScale(1.), 
                               dGrading(1.), 
                               qCircleEncroaching(false), 
                               iQualMeasure(2),
                               iPreviousPasses(0),
                               qAllowBdryChanges(true),
                               qAnisoCoarsening(false),
                               pM2D(0) {
  // empty
}

inline 
TriGenerator::~TriGenerator() {
  // empty
}

} // namespace grummp_generator

} // namespace GrAL

#endif
