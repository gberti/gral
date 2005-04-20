#ifndef NMWR_GB_MULTI_ISTREAM_CONTROL_DEVICE_H
#define NMWR_GB_MULTI_ISTREAM_CONTROL_DEVICE_H

// $LICENSE


#include "IO/istream-control-device.h"

namespace GrAL {


class multi_istream_control_device 
  : public istream_control_device_impl {
  
  typedef istream_control_device_impl base;
public:
  multi_istream_control_device(boost::shared_ptr<std::istream> i1, 
			       boost::shared_ptr<std::istream> i2,
                               const std::string& nm,
                               const std::string& ind = "")
    : base(i1,nm,ind), in2(i2)  {}

  virtual void update() {
    base::update();  
    MV.ReadValues(*in2);
  }

private:
  boost::shared_ptr<std::istream> in2;
};

} // namespace GrAL 

#endif
