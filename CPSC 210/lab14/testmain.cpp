// Cpsc 210: Lab 14: testmain.cpp (c) 2012 Joshua Hull

/**
**/
#include <iostream>
#include "stack.h"
#include "queue.h"

using namespace std;

int main() {
   // Stack tests
   stack s1;
   string word;
   string status;

   status = s1.isempty() ? "empty" : "not empty";
   cout << "Stack is " << status << endl;
   s1.push("cat");
   s1.push("dog");
   s1.push("pig");
   status = s1.isempty() ? "empty" : "not empty";
   cout << "Stack is " << status << endl;
   cout << "Stack size is " << s1.size() << endl;
   s1.print();

   cout << "Popped word is \"" << s1.pop() << "\"" << endl;
   cout << "Popped word is \"" << s1.pop() << "\"" << endl;
   cout << "Popped word is \"" << s1.pop() << "\"" << endl;
   status = s1.isempty() ? "empty" : "not empty";
   cout << "Stack is " << status << endl;
   cout << "Stack size is " << s1.size() << endl;

   // Queue test
   queue q1;

   status = q1.isempty() ? "empty" : "not empty";
   cout << "queue is " << status << endl;
   q1.enqueue("cat");
   q1.enqueue("dog");
   q1.enqueue("pig");
   status = q1.isempty() ? "empty" : "not empty";
   cout << "queue is " << status << endl;
   cout << "queue size is " << q1.size() << endl;
   q1.print();
   cout << "QUEUE PRINT USING MYLIST print()\n";
   q1.mylist::print();

   cout << "Dequeued word is \"" << q1.dequeue() << "\"" << endl;
   cout << "Dequeued word is \"" << q1.dequeue() << "\"" << endl;
   cout << "Dequeued word is \"" << q1.dequeue() << "\"" << endl;
   status = q1.isempty() ? "empty" : "not empty";
   cout << "queue is " << status << endl;
   cout << "queue size is " << q1.size() << endl;

}

