// Cpsc 210: Lab 14: stack.cpp (c) 2012 Joshua Hull
#include "stack.h"

using namespace std;

void stack:: push(string s) {
   addFront(s);
}

string stack:: pop() {
   return removeFront();
}
