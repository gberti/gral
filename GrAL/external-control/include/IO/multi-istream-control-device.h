#ifndef NMWR_GB_MULTI_ISTREAM_CONTROL_DEVICE_H
#define NMWR_GB_MULTI_ISTREAM_CONTROL_DEVICE_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



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
