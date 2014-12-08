// Cpsc 210: Lab 13: deque.cpp (c) 2012 Joshua Hull

#include <iostream>

using namespace std;

template <class T>
class Deque {
  public:
     Deque();
     void addFront(T item);
     void addEnd(T item);
     T removeFront();   // Both this and removeEnd return the object that was 
     T removeEnd();     // removed
     int size();
     void print();

  private:
     int tail;       // "tail" points to the last filled index in the array
     int capacity;   // maximum capicity of array used to implement deque
     T array[20];  // array used to implement deque
};

/** Constructor **/
template <class T>
Deque<T>::Deque()
{
   tail = -1;
   capacity = 20;
}

/** Methods **/
template <class T>
void Deque<T>::addFront(T i)
{
  // don't bother adding if we are already at capacity
  if(tail < capacity-1) {
    // move everything in the array up one index
    int j = tail+1;
    for( ; j > 0; j--) {
      array[j] = array[j-1];
    }
    // add the new element at index 0
    array[0] = i;
    // update tail since we just
    tail++;
  }
}
template <class T>
void Deque<T>::addEnd(T i)
{
  /* Add element i to the position in the array
     specified by "tail" */
  if(tail < capacity-1) {
     tail++;
     array[tail] = i;
  }
}
template <class T>
T Deque<T>::removeFront()
{
  /* Remove the element at index 0 in the array;
     don't forget to move the remaining elements at indices 1..n
     down and to update the value of tail */
  
  T result;
  if (tail > -1) {
     result = array[0];
     for (int j=1; j<=tail; j++) {
       array[j-1] = array[j];
     }
     tail--;
  }
  return result;
}
template <class T>
T Deque<T>::removeEnd()
{
  /* Remove the element before "tail" */
  T result;
  if (tail > -1) {
     result = array[tail];
     tail--;
  }
  return result;
}
template <class T>
int Deque<T>::size() {
  /* Return the size of the dequeue (there's a really
     easy way to do this using the instance variables!) */
  return tail + 1;
}
template <class T>
void Deque<T>::print() {
  /* Print out everything in the array from index 0..n */
  for (int i=0; i <= tail; i++) {
      cout << array[i] << " ";
  }
  cout << endl;
}

/** Main Program **/
int main() {
  Deque <int>deq1;

  deq1.addFront(3);
  deq1.addFront(2);
  deq1.addFront(1);
  deq1.addEnd(4);
  deq1.addEnd(5);
  deq1.addEnd(6);
  cout << "Should print: 1 2 3 4 5 6" << endl;
  deq1.print();
  cout << "Size: " << deq1.size() << endl;
  deq1.removeFront();
  deq1.removeEnd();
  deq1.removeFront();
  deq1.removeEnd();
  cout << "Should print: 3 4" << endl;
  deq1.print();
  cout << "Size: " << deq1.size() << endl;

  /** Add code to test string dequeue here **/

    Deque <string> deq2;
    deq2.addEnd("cat");
    deq2.addEnd("dog");
    deq2.addEnd("pig");
    deq2.addEnd("ape");

    deq2.print();

    cout << "Size: " << deq2.size() <<endl;

}
