#ifndef GRAL_MG_TRIANGLE_GENERATOR_H
#define GRAL_MG_TRIANGLE_GENERATOR_H

#include "Gral/Generation/Triangle/triangle-input.h"
#include "Gral/Generation/Triangle/triangle-adapter.h"
#include "Gral/Generation/Triangle/TriangleOptionsParser.h"
#include <stdio.h>

namespace GrAL {

namespace triangle_generator {

using triangle_adapter::TriangleAdapter;
using triangle_adapter::free_input_triangulateio;
using triangle_adapter::free_output_triangulateio;


class NotInvertibleError : public std::exception {
  public:
    NotInvertibleError() : det(0.0) {};
    NotInvertibleError(double d) : det(d) {}
    virtual ~NotInvertibleError() throw() {}

    virtual const char* what() const throw () {
       snprintf(msg, 63,"Transformation not invertible, det=%18.14e", det);
       return msg;  
    }
  private:
    mutable char msg[64];
    double det;
};

class TriangleGenerator /* : TriangleInput_types*/ {
  public:

     inline TriangleGenerator(); 
     virtual inline ~TriangleGenerator();
     void triangulate(TriangleInput & input, const char *flags, 
                      TriangleAdapter &output);

     void triangulate(TriangleInput & input, 
                      TriangleAdapter &output);

     const char* options(); 
     void options(const char *str) { _options.parse(str);}

     std::string info(const char* flags) { TriangleOptionsParser p;
                                           return p.info(flags); }
  
     bool getOption(const std::string name) { return _options.getOption(name);}
     void setOption(const std::string name, bool v) {
       _options.setOption(name,v);
     }
  
     double getMinAngle() const { return _options.getMinAngle();}
     void setMinAngle(const double angle) { _options.setMinAngle(angle);}
  
     double getMaxArea() const {return _options.getMaxArea();}
     void setMaxArea(const double area) { _options.setMaxArea(area);}

     virtual int triunsuitable(const double *pa, const double *pb, const 
                               double *pc, const double area) const {
         return 0; // do not impose user constraints
     }

     virtual void  incircle_transform(const double *apa, const double *apb, 
                 const double *apc, const double *apd, 
                 double *pa, double *pb, double *pc, double *pd,
                 double *inverse) const {
          /* identity transformation */
          pa[0] = apa[0];  pa[1] = apa[1]; 
          pb[0] = apb[0];  pb[1] = apb[1]; 
          pc[0] = apc[0];  pc[1] = apc[1]; 
          pd[0] = apd[0];  pd[1] = apd[1]; 
          if (inverse) {
            inverse[0]=1.0;
            inverse[1]=0.0;
            inverse[2]=0.0;
            inverse[3]=1.0;
          }
     }

  private:
     typedef TriangleInput::vertex_iterator vertex_iterator;
     typedef TriangleInput::segment_iterator segment_iterator;
     typedef TriangleInput::hole_iterator hole_iterator;
     typedef TriangleInput::region_iterator region_iterator;

     TriangleOptionsParser _options;
     char options_string[100];

     bool use_vertex_markers, use_vertex_params;

     void setup_vertex_coords(TriangleInput &input,
                              struct ::triangulateio *t);
     void setup_vertex_params(TriangleInput &input,
                              struct ::triangulateio *t);
     void setup_vertex_markers(TriangleInput & input,
                              struct ::triangulateio *t);
     void setup_segments(TriangleInput & input,
                              struct ::triangulateio *in);
     void setup_holes(TriangleInput & input,
                              struct ::triangulateio *in);
     void setup_regions(TriangleInput & input,
                              struct ::triangulateio *in);
     void setup_input(TriangleInput & input,
                              struct ::triangulateio *t);

     void init_triangulateio(::triangulateio *t);
 
     void triangulate_impl(TriangleInput & input, 
                      TriangleAdapter &output);
};

inline
TriangleGenerator::TriangleGenerator() : use_vertex_markers(true), 
                                         use_vertex_params(true) {
  //empty
}

inline
TriangleGenerator::~TriangleGenerator() {
  // empty
}

template <class TestFun, class IncircleMap>
class TriangleGeneratorPro : public TriangleGenerator {
   public : 
   TriangleGeneratorPro() {}
   TriangleGeneratorPro(TestFun const& fun, IncircleMap const &map) 
     : tritestFunObj(fun), incircleMap(map) {}

   void setTriangleTestFunction(TestFun const& fun) {
       tritestFunObj = fun;
   }

   void setIncircleMap(IncircleMap const & map) {
       incircleMap = map;
   }
   TestFun const & getTriangleTestFunction() const {return tritestFunObj; }

   IncircleMap const &getIncircleMap() const {return incircleMap;}

   int triunsuitable(const double *pa, const double *pb, const 
                               double *pc, const double area) const {
      return tritestFunObj(pa, pb, pc, area); 
   }

   void  incircle_transform(const double *apa, const double *apb, 
                 const double *apc, const double *apd, 
                 double *pa, double *pb, double *pc, double *pd,
                 double *inverse) const {
      incircleMap(apa, apb, apc, apd, pa, pb, pc, pd, inverse); 
   }
   private:
      TestFun tritestFunObj;
      IncircleMap incircleMap;
};

class AcceptAllTestFun {
  public:
    AcceptAllTestFun() {}
   ~AcceptAllTestFun() {};
    int  operator() (const double *pa, const double *pb, const double *pc,
                     const double area) const {
      return 0;
    }
};

class IdentityMapper {
  public:
    IdentityMapper() {}
   ~IdentityMapper() {};

    void operator() (const double *apa, const double *apb, const double *apc,
                     const double *apd, double *pa, double *pb, double *pc,
                     double *pd, double* inverse=NULL) const {
      pa[0] = apa[0];  pa[1] = apa[1]; 
      pb[0] = apb[0];  pb[1] = apb[1]; 
      pc[0] = apc[0];  pc[1] = apc[1]; 
      pd[0] = apd[0];  pd[1] = apd[1]; 
      if (inverse) {
        inverse[0] = 1.0;
        inverse[1] = 0.0;
        inverse[2] = 0.0;
        inverse[3] = 1.0;
      }
    }
};

class ConstEllipseMapper {
  public:
    ConstEllipseMapper() {
      setTransformation(1.0, 1.0, 1.0, 0.0); 
    }
    ConstEllipseMapper(const double *lambda, const double *e) {
      setTransformation(lambda, e); 
    }
    ConstEllipseMapper(double lambda1, double lambda2, 
                            double e1, double e2) {
      setTransformation(lambda1, lambda2, e1, e2);  
    }
    // + default copy constructor
    // + default assignment operator
    
    void setTransformation(const double *lambda, const double *e);
    
    void setTransformation(double lambda1, double lambda2, 
                           double e1, double e2);

    const double*  getEigenValues() const {return lambda;}
    const double*  getEigenDir() const {return e;}
    
   ~ConstEllipseMapper() {};

    void operator() (const double *apa, const double *apb, const double *apc,
                     const double *apd, double *pa, double *pb, double *pc,
                     double *pd, double *inverse) const {
       transform(apa, pa);
       transform(apb, pb);
       transform(apc, pc);
       transform(apd, pd); 
       if (inverse) {
         inverse[0] = m_inv[0];
         inverse[1] = m_inv[1];
         inverse[2] = m_inv[2];
         inverse[3] = m_inv[3];
         //memmove(inverse, m_inv, 4*sizeof(double));
       }
    }
  private:
    void transform(const double *x, double *y) const {
       y[0] = m[0]*x[0] + m[1]*x[1];
       y[1] = m[2]*x[0] + m[3]*x[1];
    }
    void set_transformation(); 
      
    double lambda[2];
    double e[2];
    double m[4];
    double m_inv[4];
};

} // namespace triangle_generator
} // namespace GrAL

#endif /* ifndef GRAL_MG_TRIANGLE_GENERATOR_H */
