#ifndef GRAL_GB_UTILITY_NOTIFIER_H
#define GRAL_GB_UTILITY_NOTIFIER_H

// $LICENSE

#include <vector>

/*! \defgroup notifyobserve Notifier and Observer

  \brief  A framework for automatically notifying observers of changes to their notifiers.

  An observer registers at a notifier via a call to <tt>connect(notifier const*)</tt>.
  On change, a notifier notifies all its observers by iterating over them and
  telling them about the specific change, for example, it may call notifier_assigned(this) on all of them.
  Normally, an ovserver will have only one notifier, but each notifier will usually have several observers.

  Typically, notifiers and observers will be used like this:
  \code
  class Master : public notifier {
  public:
    class observer_type : public observer {
    public:
      // this function is already used in observer and has a fixed interface
      virtual void notifier_assigned  (notifier const*);
      // this function is new and can have a custom interface
      virtual void notifier_item_added(Master   const*, int item);
    };
  
    void add_item(int item) {
      do_something(item);
      // notify all observers on the change
      for(observer_iterator ob = observers->begin(); ob != observers->end(); ++ob)
         dynamic_cast<observer_type *>(ob)->notifier_item_added(this, item);
    }
  }; // class Master

  // Objects of type  Dependent  depend on an object of type Master 
  class Dependent : public Master::observer_type {
  public:
     Dependent(Master const* m) { 
       connect(m); 
       // init(m); etc
     }
     ~Dependent() { } // calls disconnect on all notifiers
     virtual void notifier_assigned(notifier const* m); // probably re-initialization
     virtual void notifier_item_added(Master const* m, int item) {
       // do something with item

     }
     // ...
  };
  \endcode

 
  Often, the exact nature of the changed notifier is known or
  can be detected, and further queries about the nature of the
  change can be done.
*/




// opaque type for internal data structure
class notifier_table;

// see below
class notifier;
class observer;

/*! \brief Part of the Notify/Observer pattern

   \ingroup notifyobserve
 */
class observer {
private:
  friend class notifier;
  typedef std::vector<notifier const*> notifier_table;

  notifier_table notifiers;
public:
  observer();
  virtual ~observer();

  //! copying: connect to all notifiers from \c rhs 
  observer(const observer& rhs);
  //! disconnect from all notifiers, connect to all  notifiers from \c rhs
  observer& operator=(const observer& rhs);

  //! this is the main hook to be overloaded: Specify some action in case the notifier gets changed.
  virtual void notifier_assigned(const notifier* n) = 0;

  // derived classes may define more messages specific to particular changes of notifiers

  //!
  unsigned NumOfNotifiers() const { return notifiers.size();}

protected:
  //! Connect to a notifier. Will also add this object to \c n's observer list.
  void connect   (const notifier* n);
  //! Disconnect from a notifier. Will also remove this object from \c n's observer list.
  void disconnect(const notifier* n);
  //! Disconnect from all notifiers.
  void disconnect();
private:
  void add_notifier   (const notifier* n);
  void remove_notifier(const notifier* n);
  void clear();
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------


/*! \brief Part of the Notify/Observer pattern

   \ingroup notifyobserve
 */
class notifier {
private:
  friend class observer;
  typedef std::vector<observer *>   observer_table;
public:
  //! Iterator over sequence of registered observers
  typedef observer_table::iterator observer_iterator;
private:
  mutable observer_table  observers;
  int assignment_level;
public:
  notifier();
  notifier(notifier const& rs);

protected: 
  //! to be called before an assigment  in a derived class
  void before_assignment();
  //! to be called after an assignment in a derived class
  void after_assignment();
public:  
  notifier& operator=(const notifier& rs);
  virtual ~notifier();
  //!
  unsigned NumOfObservers() const { return observers.size();}
  //! First registered observer 
  observer_iterator begin_observer() { return observers.begin();}
  //! Past-the-end observer
  observer_iterator end_observer()   { return observers.end();}

private:
  void add_observer   (observer* obs) const;
  void remove_observer(observer* obs) const;
};

#endif
