// Cpsc 210: Lab 14: queue.cpp (c) 2012 Joshua Hull
#include "queue.h"

using namespace std;

void queue:: enqueue(string s) {
   addEnd(s);
}

string queue:: dequeue() {
   return removeEnd();
}

