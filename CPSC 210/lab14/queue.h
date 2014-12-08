// Cpsc 210: Lab 14: queue.h (c) 2012 Joshua Hull

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "mylist.h"

using namespace std;

class queue: public mylist {
   public:
      void enqueue(string s);
      string dequeue();
};

#endif
