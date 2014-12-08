// Cpsc 210: Lab 14: stack.h (c) 2012 Joshua Hull

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "mylist.h"

using namespace std;

class stack: public mylist {
   public:
      void push(string s);
      string pop();
};

#endif
