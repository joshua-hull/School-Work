// Cpsc 210: Lab 14: mylist.h (c) 2012 Joshua Hull

#ifndef MYLIST_H
#define MYLIST_H

#include "mynode.h"

using namespace std;

class mylist {
   public:

      // Methods associated with mylist object
      int    size();
      bool   isempty();
      void print();
      
      // Prevent others from accessing the class, should be inhereted only.
   protected:
      void addFront(string word);
      void addEnd(string word);
      string removeFront();
      string removeEnd();

      // Constructors
      mylist();
      mylist(mylist &list);   // Copy constructor

      // Destructor
      ~mylist();

   private:
      // Data associated with mylist object
      mynode *head;        // pointer to dummy header node
      mynode *tail;        // pointer to dummy tail node
      int    listsize;     // list size
};

#endif
