#ifndef NMWR_GB_MULTI_ISTREAM_CONTROL_DEVICE_H
#define NMWR_GB_MULTI_ISTREAM_CONTROL_DEVICE_H

#include "IO/istream-control-device.h"

class multi_istream_control_device 
  : public istream_control_device_impl {
  
  typedef istream_control_device_impl base;
public:
  multi_istream_control_device(std::istream* i1, std::istream* i2,
                               const std::string& nm,
                               const std::string& ind = "")
    : base(i1,nm,ind), in2(i2)  {}

  virtual void update() {
    base::update();  
    MV.ReadValues(*in2);
  }

private:
 std::istream* in2;
};

#endif
