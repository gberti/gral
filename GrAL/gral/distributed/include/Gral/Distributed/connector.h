#ifndef NMWR_GB_DISTR_CONNECTOR_H
#define NMWR_GB_DISTR_CONNECTOR_H


// $LICENSE

#include <vector>    // STL

#include "Config/compiler-config.h"


#include "Utility/copy-traits.h"
#include "Utility/reference-count.h"


//----------------------------------------------------------------
//  
/*!  \defgroup connectors Connectors 
     \brief Abstractions for copying data between iterator ranges.

  \contents:
  - completely abstract: class connector_impl; 
  - partially abstract:
    \c template<class SenderIt, class ReceiverIt> <BR> 
    \c class range_connector;
  - concrete:
     - \c template<class SenderIt, class ReceiverIt> <BR>
       \c class copy_range_connector;
     - \c template<class SenderIt, class ReceiverIt> <BR>
       \c class copy_range_connector;

  \description:
  The classes in this module allow distributed data structures (dds),
  such as distributed grid functions,
  to ignore the data transport mechanism used
  (message passing or just simple in-core copy).
  On the other hand, the data transport
  handler can ignore details of data storage in the dds.


*/
//----------------------------------------------------------------



//----------------------------------------------------------------
/*! \brief abstract base for data connectors (letter class)
   \ingroup connectors
 */
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

/*! \brief specialization of copy_traits for connector_impl
 */
template<>
struct copy_traits<connector_impl> : public copy_traits_base<connector_impl> {
  static connector_impl* clone(connector_impl const& c) { return c.clone();}
};

/*! \brief Envelope class for connector_impl
   \ingroup connectors
 */
class Connector {
private:
   copy_on_write_ptr<connector_impl> impl; 
public: 
   typedef Connector self;
  Connector(connector_impl* im = 0)  :impl(im) {}
  void SendData() { impl->send_data();}
  void RecvData() { impl->recv_data();}
  void SendDataBegin() { impl->send_data_begin();}
  void SendDataEnd()   { impl->send_data_end();}
  void RecvDataBegin() { impl->recv_data_begin();}
  void RecvDataEnd()   { impl->recv_data_end();}
};

/*! \defgroup connectorfunctions  Connector function objects
    \brief Function objects for parameterization of
     foreach(begin,end,function) - type algorithms

   \ingroup connectors
*/

/*! \brief send data associated to connector
  \ingroup connectorfunctions

  If [b,e) is a range of Connector s, then
  \code
  foreach(b,e,ConnectorSendData);
  \endcode
  is semantically equivalent to
  \code
  foreach(b,e,ConnectorSendBegin);
  foreach(b,e,ConnectorSendEnd);
  \endcode
  The latter may be used to let communication and computation overlap
  (compute-and-send-ahead).

 */
class ConnectorSendData {
 public:
  void operator()(Connector& C) const { C.SendData();}
  void operator()(connector_impl* C) const { C->send_data();}
};

/*! \brief begin to send data associated to connector
  \ingroup connectorfunctions
  \see ConnectorSendData
 */
class ConnectorSendBegin {
 public:
  void operator()(Connector& C) const { C.SendDataBegin();}
  void operator()(connector_impl* C) const { C->send_data_begin();}
};

/*! \brief  finish to send data associated to connector
  \ingroup connectorfunctions
  \see ConnectorSendData
 */
class ConnectorSendEnd {
 public:
  void operator()(Connector& C) const { C.SendDataEnd();}
  void operator()(connector_impl* C) const { C->send_data_end();}
};

/*! \brief receive data associated to connector
  \ingroup connectorfunctions
  \see ConnectorSendData
 */
class ConnectorRecvData {
 public:
  void operator()(Connector& C)      const { C.RecvData();}
  void operator()(connector_impl* C) const { C->recv_data();}
};

/*! \brief  begin to receive data associated to connector
  \ingroup connectorfunctions
  \see ConnectorSendData, ConnectorRecvData
 */
class ConnectorRecvBegin {
 public:
  void operator()(Connector& C) const { C.RecvDataBegin();}
  void operator()(connector_impl* C) const { C->recv_data_begin();}
};

/*! \brief finish to receive data associated to connector
  \ingroup connectorfunctions
  \see ConnectorSendData, ConnectorRecvData
 */
class ConnectorRecvEnd {
 public:
  void operator()(Connector& C) const { C.RecvDataEnd();}
  void operator()(connector_impl* C) const { C->recv_data_end();}
};


//----------------------------------------------------------------
/*! \brief  connector that operates on a range
     \ingroup connectors

 */
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


/*!  \brief connector that copies the associated send range to the destination range
     \ingroup connectors
 */
template<class SenderIt, class ReceiverIt>
class copy_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
  using conn_base::src_b;
  using conn_base::src_e;
  using conn_base::dest_b;
  using conn_base::dest_e;
public:
  copy_range_connector() {}
  copy_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re) {}
  virtual connector_impl* clone() const { return new copy_range_connector(*this);}

  virtual void send_data();  
  virtual void recv_data()  {}
};


/*!  \brief connector that adds the associated range to the destination range
     \ingroup connectors
 */
template<class SenderIt, class ReceiverIt>
class add_to_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
  using conn_base::src_b;
  using conn_base::src_e;
  using conn_base::dest_b;
  using conn_base::dest_e;
public:
  add_to_range_connector() {}
  add_to_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re) {}
  virtual connector_impl* clone() const { return new add_to_range_connector(*this);}

  virtual void send_data();//   { my_add_to(src_b,src_e,dest_b);}
  virtual void recv_data()   {}
};


/*! \brief connector that copies send range to destination range, using
  and intermediate buffer
  \ingroup connectors

   Buffering may be avoided if we know that [dest_b,dest_e) already is
   a buffer, as may be the case with MPI.
*/
template<class SenderIt, class ReceiverIt>
class buffered_copy_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
private:
  // should use iterator_traits here, but work only with
  // partial specialization.
  typedef typename SenderIt::value_type sender_value_type;
  std::vector<sender_value_type>  buffer;

  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
  using conn_base::src_b;
  using conn_base::src_e;
  using conn_base::dest_b;
  using conn_base::dest_e;
public:
  buffered_copy_range_connector() {}
  buffered_copy_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re), buffer(/* size not known! */ ) {}
  // buffer(se -sb) possible?
  virtual connector_impl* clone() const { return new buffered_copy_range_connector(*this);}

  virtual void send_data();
  virtual void recv_data();
};



/*! \brief connector that adds send range to destination range, using
  and intermediate buffer
  \ingroup connectors

*/
template<class SenderIt, class ReceiverIt>
class buffered_add_to_range_connector : public range_connector<SenderIt,ReceiverIt> 
{
private:
  // should use iterator_traits here, but work only with
  // partial specialization.
  typedef typename SenderIt::value_type sender_value_type;
  std::vector<sender_value_type>  buffer;

  typedef range_connector<SenderIt,ReceiverIt>  conn_base;
  using conn_base::src_b;
  using conn_base::src_e;
  using conn_base::dest_b;
  using conn_base::dest_e;
public:
  buffered_add_to_range_connector() {}
  buffered_add_to_range_connector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
    : conn_base(sb,se,rb,re) {}
  virtual connector_impl* clone() const { return new buffered_add_to_range_connector(*this);}

  virtual void send_data(); 
  virtual void recv_data(); 
};


/*! Creator function for copy_range_connector
  \ingroup connectors
  \relates copy_range_connector, Connector
 */
template<class SenderIt, class ReceiverIt>
inline Connector
CopyConnector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt rs)
{ 
  return Connector
    (new copy_range_connector<SenderIt,ReceiverIt>(sb,se,rb,rs));
}

/*! Creator function for copy_range_connector
  \ingroup connectors
  \relates copy_range_connector, Connector
 */
template<class SenderRge, class ReceiverRge>
inline Connector
CopyConnector(SenderRge s, ReceiverRge r)
{ 
  typedef typename SenderRge::iterator   SenderIt;
  typedef typename ReceiverRge::iterator ReceiverIt;
  return Connector
    (new copy_range_connector<SenderIt,ReceiverIt>(s.begin(),s.end(),
						   r.begin(),r.end()));
}

/*! Creator function for add_to_range_connector
  \ingroup connectors
  \relates add_to_range_connector, Connector
 */
template<class SenderIt, class ReceiverIt>
inline Connector
AddingConnector(SenderIt sb, SenderIt se, ReceiverIt rb, ReceiverIt re)
{ 
  return Connector
    (new add_to_range_connector<SenderIt,ReceiverIt>(sb,se,rb,re));
}


/*! Creator function for add_to_range_connector
  \ingroup connectors
  \relates add_to_range_connector, Connector
 */
template<class SenderRge, class ReceiverRge>
inline Connector
AddingConnector(SenderRge s, ReceiverRge r)
{ 
  typedef typename SenderRge::iterator   SenderIt;
  typedef typename ReceiverRge::iterator ReceiverIt;
  return Connector
    (new add_to_range_connector<SenderIt,ReceiverIt>(s.begin(),s.end(),
						     r.begin(),r.end()));
}


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/range-connector.C"
#endif

#endif
