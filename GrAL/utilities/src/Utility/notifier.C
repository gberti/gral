#include "Utility/notifier.h"

#include <algorithm>



//----------------- observer ------------------------

observer::observer()  { }
observer::~observer() { clear(); }


void observer::clear() 
{
  notifier_table::iterator it;
  for(it = notifiers.begin(); it != notifiers.end(); ++it)
    (*it)->remove_observer(this);
}

void observer::connect(const notifier* nt)
{
  if( nt != 0) {
    add_notifier(nt);
    nt->add_observer(this);
  }
}

void observer::disconnect(const notifier* nt)
{
  if(nt != 0) {
    remove_notifier(nt);
    nt->remove_observer(this);
  }
}

void observer::disconnect()
{
  for(notifier_table::iterator it = notifiers.begin(); it != notifiers.end(); ++it) 
    (*it)->remove_observer(this);
  notifiers.clear();
}

void observer::add_notifier(const notifier* nt)
{
  // could check for duplicates
  if(nt != 0)
    notifiers.push_back(nt);
}

void observer::remove_notifier(const notifier* nt)
{
  if(nt != 0) {
    notifier_table::iterator it = std::find(notifiers.begin(),notifiers.end(),nt);
    if (it != notifiers.end())
      notifiers.erase(it);
  }
}


// Here we do a copy of notifiers: copied observers depend on the same
// physical objects as the right-hand sides.
observer::observer(const observer& rhs) { 
  notifiers = rhs.notifiers;
  notifier_table::iterator nf;
  for(nf = notifiers.begin(); nf != notifiers.end(); ++nf)
    (*nf)->add_observer(this);
}

observer& observer::operator=(const observer& rhs) { 
  if(this != & rhs) {
    clear();
    notifiers = rhs.notifiers;
    notifier_table::iterator nf;
    for(nf = notifiers.begin(); nf != notifiers.end(); ++nf)
      (*nf)->add_observer(this);
  }
  return (*this);
}




//---------------- notifier ----------------------------

notifier::notifier() : assignment_level(0) { }

notifier::~notifier() { 
  for(observer_table::iterator it = observers.begin(); it != observers.end(); ++it)
    (*it)->remove_notifier(this);    
}

notifier& notifier::operator=(const notifier& rs)
{
  if(this != &rs) {
    // this is largely superflous, but gives a pattern how to implement
    // op= in derived classes. This ensures notify_all() is called only
    // at the most derived level
    before_assignment();
    after_assignment();
  }
  return (*this);
}

// Here we do NOT copy observers, because they are tied to a physical object,
// that is, to a reference and not to a value!
notifier::notifier(const notifier&) { }

void notifier::before_assignment() 
{ 
  assignment_level++;
}

void notifier::after_assignment() 
{ 
  // make sure that observers are notified only for most derived class
  assignment_level--;
  if (assignment_level == 0)
    for(observer_table::iterator it = observers.begin(); it != observers.end(); ++it)
      (*it)->notifier_assigned(this);
}



void notifier::add_observer(observer* obs) const { 
  observers.push_back(obs);
}

void notifier::remove_observer(observer* obs) const
{
  observer_table::iterator it = find(observers.begin(),observers.end(),obs);
  if (it != observers.end())
    observers.erase(it);
}





