#ifndef NMWR_GB_RANGE_CONNECTOR_C
#define NMWR_GB_RANGE_CONNECTOR_C

#include "Gral/Distributed/connector.h"

#include <algorithm> // STL


// $LICENSE



// STL copy requires iterator_category and such.
// here are simpler versions.

template<class I1, class I2>
I2 my_copy(I1 b1, I1 e1, I2 b2)
{
  while(b1 != e1) {
    *b2 = *b1; // op=(*b2,*b1)
    ++b1; ++b2;
  }
  return b2;
}

template<class I1, class I2, class AssignOp>
I2 my_copy_op(I1 b1, I1 e1, I2 b2, AssignOp op)
{
  while(b1 != e1) {
    op(*b2,*b1); 
    ++b1; ++b2;
  }
  return b2;
}


template<class I1, class I2>
I2 my_add_to(I1 b1, I1 e1, I2 b2)
{
  while(b1 != e1) {
    *b2 += *b1; // op+=(*b2,*b1)
    ++b1; ++b2;
  }
  return b2;
}


template<class SenderIt, class ReceiverIt>
range_connector<SenderIt,ReceiverIt>::~range_connector() {}

template<class SenderIt, class ReceiverIt>
void copy_range_connector<SenderIt,ReceiverIt>::send_data()
 { my_copy(src_b,src_e,dest_b);}

template<class SenderIt, class ReceiverIt>
void add_to_range_connector<SenderIt,ReceiverIt>::send_data() 
{ my_add_to(src_b,src_e,dest_b);}

template<class SenderIt, class ReceiverIt>
void buffered_copy_range_connector<SenderIt,ReceiverIt>::send_data()
 { my_copy(src_b,src_e,back_inserter(buffer));}

template<class SenderIt, class ReceiverIt>
void buffered_copy_range_connector<SenderIt,ReceiverIt>::recv_data()
{ my_copy(buffer.begin(), buffer.end(), dest_b);}

template<class SenderIt, class ReceiverIt>
void buffered_add_to_range_connector<SenderIt,ReceiverIt>::send_data()
 { my_add_to(src_b,src_e,back_inserter(buffer));}

template<class SenderIt, class ReceiverIt>
void buffered_add_to_range_connector<SenderIt,ReceiverIt>::recv_data()
{ my_add_to(buffer.begin(), buffer.end(), dest_b);}


#endif
