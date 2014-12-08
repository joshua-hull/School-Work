// Cpsc 210: Lab 14: mylist.cpp (c) 2012 Joshua Hull


/** 
    Doubly linked list implementation of a deque.
**/
#include <iostream>
#include "mylist.h"

using namespace std;

/** mylist Constructors **/
mylist:: mylist() {
   head = new mynode;  // Create dummy header node
   tail = new mynode;  // Create dummy tail node
   head->setnext(tail);
   tail->setprev(head);
   listsize = 0;
}

/** Copy constructor -- create totally separate copy of list **/
mylist:: mylist(mylist & list) {
   mynode *ptr = list.head;
   mynode *prev;
   mynode *nodecopy;
   head = new mynode;               // dummy header node

   // Walk through original list creating nodes of new list
   prev = head;
   while ((ptr = ptr->getnext()) != NULL) {
      nodecopy = new mynode(*ptr);  // create new node
      prev->setnext(nodecopy);
      nodecopy->setprev(prev);
      prev = nodecopy;
      tail = nodecopy;
   }
   listsize = list.listsize;
}

/** mylist Destructor -- free nodes before destroying list **/
mylist:: ~mylist() {
   mynode *ptr = head;
   mynode *next;
   while (ptr != NULL) {
      next = ptr->getnext();
      delete(ptr);
      ptr = next;
   }
}

/** mylist Methods **/
void mylist:: addFront(string word) {
   mynode *newnode = new mynode(word);
   mynode *next = head->getnext();

   newnode->setnext(next);  // point new node forward to old first
   next->setprev(newnode);  // point old first back to new node
   newnode->setprev(head);  // point back to dummy header node
   head->setnext(newnode);  // point dummy to new node
   listsize++;
}

void mylist:: addEnd(string word) {
   mynode *newnode = new mynode(word);
   mynode *prev = tail->getprev();

   newnode->setprev(prev);  // point new node back to old last
   prev->setnext(newnode);  // point old last forward to new node
   newnode->setnext(tail);  // point new forward to dummy tail
   tail->setprev(newnode);  // and tail back to new node
   listsize++;
}

string mylist:: removeFront() {
   string result = "";
   mynode *firstword = head->getnext();
   mynode *next = firstword->getnext();
   if (listsize > 0) {
      result = firstword->getword();
      head->setnext(next);
      next->setprev(head);
      listsize--;
   }
   // Stop memory leak
   delete(firstword);
   return result;
}

string mylist:: removeEnd() {
   string result = "";
   mynode *lastword = tail->getprev();
   mynode *prev = lastword->getprev();
   if (listsize > 0) {
      result = lastword->getword();
      tail->setprev(prev);
      prev->setnext(tail);
      listsize--;
   }
   // Stop memory leak
   delete(lastword);
   return(result);
}

int mylist:: size() {
   return listsize;
}

bool mylist:: isempty() {
   return(listsize == 0);
}

void mylist:: print() {
   mynode *ptr = head;
   cout << "Words:\n";
   while ((ptr=ptr->getnext()) != tail) {
      cout << "   " << ptr->getword() << endl;
   }
}
