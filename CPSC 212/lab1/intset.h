// CP SC 212 Lab 1
// Joshua Hull


#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

class Node {
        public:
        Node *next;
        int value;
        Node() {
                next = NULL;
                value = 0;
        }
};

class Intset {

 private: 
  int size, allocated;
  int *A;
  Node *head;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
