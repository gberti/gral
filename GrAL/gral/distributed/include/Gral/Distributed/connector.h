#ifndef NMWR_GB_DISTR_CONNECTOR_H
#define NMWR_GB_DISTR_CONNECTOR_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "compiler-config.h"

#include "vector.h"    // STL

#include "Container/copy-traits.h"
#include "Utility/reference-count.h"


//----------------------------------------------------------------
//  
//  Connectors -- abstractions for moving data between ranges.
//
//  Contents:
//  ---------
//  completely abstract:
//   [1] class connector; 
//  partially abstract:
//   [2] template<class SenderIt, class ReceiverIt> 
//       class range_connector;
//  concrete:
//   [3] template<class SenderIt, class ReceiverIt> 
//       class copy_range_connector;
//   [4] template<class SenderIt, class ReceiverIt> 
//       class copy_range_connector;
//
//  Purpose:
//  -------
//  This allows distributed data structures (such as distributed
//  grid functions) to ignore the data transport mechanism used
//  (message passing or just in-memory copy), and the data transport
//  handler to ignore details of data storage in the dds.
//
//  State:
//  -----
//  Under Development.
//
//----------------------------------------------------------------



//----------------------------------------------------------------
//                 [1] connector_impl / Connector                   
//----------------------------------------------------------------

class connector_impl {
public:
  virtual void send_data() = 0;
  virtual void send_data_begin() { send_data();}
  virtual void send_data_end()   {}
  virtual void recv_data() = 0;
  virtual void recv_data_begin() { recv_data();}
  virtual void recv_data_end()   {}

  virtual connector_impl* clone() const = 0;
  virtual ~connector_impl();
};

struct copy_traits<connector_impl> {
  static connector_impl* clone(connector_impl const& c) { return c.clone();}
};


class Connector {
private:
   RCIPtr<connector_impl> impl; // needs non-abstract type
  // connector_impl* impl;
public: 
   typedef Connector self;
  MAKE_DEFAULT_OPS_SELF
  Connector(connector_impl* im = 0)  :impl(im) {}
  void SendData() { impl->send_data();}
  void RecvData() { impl->recv_data();}
  void SendDataBegin() { impl->send_data_begin();}
  void SendDataEnd()   { impl->send_data_end();}
  void RecvDataBegin() { impl->recv_data_begin();}
  void RecvDataEnd()   { impl->recv_data_end();}
};

// function objects for parameterization of
// foreach(begin,end,function) - type algorithms
class ConnectorSendData {
 public:
  void operator()(Connector& C) const { C.SendData();}
  void operator()(connector_impl* C) const { C->send_data();}
};

class ConnectorSendBegin {
 public:
  void operator()(Connector& C) const { C.SendDataBegin();}
  void operator()(connector_impl* C) const { C->send_data_begin();}
};

class ConnectorSendEnd {
 public:
  void operator()(Connector& C) const { C.SendDataEnd();}
  void operator()(connector_impl* C) const { C->send_data_end();}
};

class ConnectorRecvData {
 public:
  void operator()(Connector& C)      const { C.RecvData();}
  void operator()(connector_impl* C) const { C->recv_data();}
};

class ConnectorRecvBegin {
 public:
  void operator()(Connector& C) const { C.RecvDataBegin();}
  void operator()(connector_impl* C) const { C->recv_data_begin();}
};

class ConnectorRecvEnd {
 public:
  void operator()(Connector& C) const { C.RecvDataEnd();}
  void operator()(connector_impl* C) const { C->recv_data_end();}
};


//----------------------------------------------------------------
//                   [2] range_connector / RangeConnector         
//----------------------------------------------------------------

template<class SenderIt, class ReceiverIt>
class range_connector : public connector_impl {
protected:
  SenderIt    src_b,  src_e;
  ReceiverIt  dest_b, dest_e;
public:
  range_connector() {}
  ~range_connector();
  range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : src_b(sb), src_e(se), dest_b(rb), dest_e(re) {}

  virtual  void set_src (SenderIt b, SenderIt e)
    { src_b  = b; src_e = e;}
  virtual  void set_dest(ReceiverIt b, ReceiverIt e)
    { dest_b = b; dest_e = e;}
};

/*
template<class SenderIt, class ReceiverIt>
class RangeConnector {
private:
  //  RCIPtr<range_connector<SenderIt,ReceiverIt> > impl; // needs non-abstract type
  range_connector<SenderIt,ReceiverIt>*         impl;
public: 
  RangeConnector(range_connector<SenderIt,ReceiverIt>* im = 0) : impl(im) {}
  void SetSrc (SenderIt   b, SenderIt   e) { impl->set_src(b,e);}
  void SetDest(ReceiverIt b, ReceiverIt e) { impl->set-dest(b,e);}
};

// in principle, we could derive RangeConnector<>  from Connector
// but this would make some castin necessary.
template<class SenderIt, class ReceiverIt>
inline 
Connector ToConnector(RangeConnector<SenderIt,ReceiverIt>& RC)
{ 
  // here we must get access to the base pointer, because
  // assignment of RCIPtr<> of different base types can only
  // implemented with member templates.
  return Connector(new range_connector<SenderIt,ReceiverIt>( &(*RC.get_impl())));
}
*/

//----------------------------------------------------------------
//        [3], [4] copy_range_connector, add_range_connector
//----------------------------------------------------------------





template<class SenderIt, class ReceiverIt>
class copy_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
public:
  copy_range_connector() {}
  copy_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re) {}
  virtual connector_impl* clone() const { return new copy_range_connector(*this);}

  virtual void send_data();  
  virtual void recv_data()  {}
};

template<class SenderIt, class ReceiverIt>
class add_to_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
public:
  add_to_range_connector() {}
  add_to_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re) {}
  virtual connector_impl* clone() const { return new add_to_range_connector(*this);}

  virtual void send_data();//   { my_add_to(src_b,src_e,dest_b);}
  virtual void recv_data()   {}
};


// buffering may be avoided if we know that [dest_b,dest_e) already is
// a buffer, as may be the case with MPI.

template<class SenderIt, class ReceiverIt>
class buffered_copy_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
private:
  // should use iterator_traits here, but work only with
  // partial specialization.
  typedef typename SenderIt::value_type sender_value_type;
  vector<sender_value_type>  buffer;

  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
public:
  buffered_copy_range_connector() {}
  buffered_copy_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re), buffer(/* size not known! */ ) {}
  // buffer(se -sb) possible?
  virtual connector_impl* clone() const { return new buffered_copy_range_connector(*this);}

  virtual void send_data();//       { my_copy(src_b,src_e,back_inserter(buffer));}
  virtual void recv_data();//       { my_copy(buffer.begin(), buffer.end(), dest_b);}
};


template<class SenderIt, class ReceiverIt>
class buffered_add_to_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
private:
  // should use iterator_traits here, but work only with
  // partial specialization.
  typedef typename SenderIt::value_type sender_value_type;
  vector<sender_value_type>  buffer;

  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
public:
  buffered_add_to_range_connector() {}
  buffered_add_to_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt rs)
    : conn_base(sb,se,rb,re) {}
  virtual connector_impl* clone() const { return new buffered_add_to_range_connector(*this);}

  virtual void send_data(); // { copy(src_b,src_e,buffer.begin());}
  virtual void recv_data(); // { add_to(buffer.begin(), buffer.end(), dest_b);}
};



template<class SenderIt, class ReceiverIt>
inline
//RangeConnector<SenderIt,ReceiverIt> 
Connector
CopyConnector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt rs)
{ 
  //  return RangeConnector<SenderIt,ReceiverIt>
  return Connector
    (new copy_range_connector<SenderIt,ReceiverIt>(sb,se,rb,re));
}

template<class SenderRge, class ReceiverRge>
inline
//RangeConnector<SenderIt,ReceiverIt> 
Connector
CopyConnector(SenderRge s, ReceiverRge r)
{ 
  typedef typename SenderRge::iterator   SenderIt;
  typedef typename ReceiverRge::iterator ReceiverIt;
  //  return RangeConnector<SenderIt,ReceiverIt>
  return Connector
    (new copy_range_connector<SenderIt,ReceiverIt>(s.begin(),s.end(),
						   r.begin(),r.end()));
}

template<class SenderIt, class ReceiverIt>
inline
//RangeConnector<SenderIt,ReceiverIt> 
Connector
AddingConnector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt rs)
{ 
  //  return RangeConnector<SenderIt,ReceiverIt>
  return Connector
    (new add_to_range_connector<SenderIt,ReceiverIt>(sb,se,rb,re));
}


template<class SenderRge, class ReceiverRge>
inline
//RangeConnector<SenderIt,ReceiverIt> 
Connector
AddingConnector(SenderRge s, ReceiverRge r)
{ 
  typedef typename SenderRge::iterator   SenderIt;
  typedef typename ReceiverRge::iterator ReceiverIt;
  //  return RangeConnector<SenderIt,ReceiverIt>
  return Connector
    (new add_to_range_connector<SenderIt,ReceiverIt>(s.begin(),s.end(),
						     r.begin(),r.end()));
}


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/range-connector.C"
#endif

#endif
