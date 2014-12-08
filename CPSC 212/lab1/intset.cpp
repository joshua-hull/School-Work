// CP SC 212 Lab 1
// Joshua Hull

#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{
  head = new Node();
}

Intset::~Intset()
{
  // Simply walk down the list from the front, deleting along the way.
  Node *tmp;
  while(head != NULL) {
    tmp = head->next;
    delete head;
    head = tmp;
  }        
}

/* Return true if key is in the set */
bool Intset::find(int key)
{
  Node *tmp = head;
  // Since this is a singly linked list, binary search won't work.
  // is the only real option.
  while (tmp != NULL) {
    if (tmp->value == key){
      return true;
    }
    tmp = tmp-> next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
  assert (!find(key));
  Node *tmp = new Node;
  Node *ptr = head;

  // Case 1: We are inserting the first node. Simply overwrite dummy node.
  if (head->next == NULL) {
    head->value = key;
    head->next = tmp;
  }
  
  // Case 2: We are inserting a value less then the head. Append the new head
  // to the begining. 
  else if(head->value > key){
    tmp->value = key;
    tmp->next = head;
    head = tmp;
  }
    
  // Default Case: We are inserting after the first node. Walk through the list
  // to find the right spot and insert when we get there. 
  else {
    while (ptr->next->next != NULL && key > ptr->next->value) {
      ptr = ptr->next;
    }
    tmp->value = key;
    tmp->next = ptr->next;
    ptr->next = tmp;
    }
}
/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{

  assert (find(key));

  Node *tmp = head;
  Node *toBeDeleted;

  // If we are deleting the head then simply point head to the next node and
  // delete the old head.
  if(head->value == key) {
    tmp = tmp->next;
    delete head;
    head = tmp;
  } 
  // Otherwise walk throught the list and delete the approppriate node when we
  // find it, fixing the links before hand.
  else {
      while(tmp->next->value != key) {
        tmp = tmp->next;
    }
    toBeDeleted = tmp->next;
    tmp->next = tmp->next->next;
    delete toBeDeleted;
  }
}

void Intset::print(void)
{

  // Walk throught the list and print along the way.
  Node *tmp = head;
  while (tmp->next != NULL) {
    cout << tmp->value  << "\n";
    tmp = tmp->next;
  }
}
