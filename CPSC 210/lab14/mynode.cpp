// Cpsc 210: Lab 14: mynode.cpp (c) 2012 Joshua Hull

#include <iostream>
#include "mynode.h"

using namespace std;

/** mynode Constructors **/
// Default constructor
mynode:: mynode() {
   nodeword = "";
   next = NULL;
}

mynode:: mynode(const string & word) {
   nodeword = word;
   next = NULL;
}

/** mynode Destructor **/
mynode:: ~mynode() {
  // do nothing
}

/** mynode Methods **/
string mynode:: getword() {
   return nodeword;
}

mynode * mynode:: getnext() {
   return next;
}

void mynode:: setnext(mynode * ptr) {
   next = ptr;
}

mynode * mynode:: getprev() {
   return prev;
}

void mynode:: setprev(mynode * ptr) {
   prev = ptr;
}
