#ifndef GRAL_GB_CONTAINER_BACK_PUSHER_H
#define GRAL_GB_CONTAINER_BACK_PUSHER_H

#include <iostream>


template<class CONTAINER>
class back_pusher {
private:
  CONTAINER* cont;
public:
  back_pusher() : cont(0) {}
  back_pusher(CONTAINER& c) : cont(&c) {}

  void read(std::istream& in)
    { 
      typename CONTAINER::value_type tmp;  
      in >> tmp;
      cont->push_back(tmp);
    }
};


template<class CONTAINER>
inline
std::istream& operator>>(std::istream& in, 
			 back_pusher<CONTAINER> & b)
{
  b.read(in);
  return in;
}

template<class CONTAINER>
inline
std::ostream& operator<<(std::ostream& out, 
			 back_pusher<CONTAINER> & b)
{
  return out;
}

#endif
