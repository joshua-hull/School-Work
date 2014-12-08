// Cpsc 210: Lab 12: tree.cpp (c) Joshua Hull

#include <iostream>
#include "tree.h"

using namespace std;

/** tree constructor **/
tree:: tree(int initialValue) {
    value = initialValue;
    count = 1;
    left = NULL;
    right = NULL;
}

/** tree destructor **/
tree:: ~tree() {
   /** Leave following print in your submission **/
   cout << "Deleting " << value << ": count " << count << endl;
    
    // If there is a left node, delete it.
    if (left != NULL) {
        delete left;
    }
    // If there is a right node, delete it.
    if (right != NULL) {
        delete right;
    }
}

/** methods **/
void tree:: processValue(int searchvalue) {
    // If this node has it, incriment the count.
    if (value == searchvalue) {
        count ++;
    // If we are less then current node's value
    } else if (searchvalue < value) {
        // but there are no more nodes on the nleft, make a new one
        if (left == NULL) {
            left = new tree(searchvalue);
        // otherwise keep searching
        } else {
            left->processValue(searchvalue);
        }
    // If we are greater then the current node do the same but on the right.
    } else {
        if (right == NULL) {
            right = new tree(searchvalue);    
        } else {
            right ->processValue(searchvalue);
        }      
    }
}

void tree::printup() {
    // If there is a lower number to our right, print it first.
     if(left != NULL) {
        left->printup();
     }
    // Then print ourselves.
     cout << value << ":" << count << endl;
    // then any larger values to the rigth.
    if (right != NULL) {
        right->printup();
    }
}

void tree::printdown() {
    // If there is a greater value to the right, print it.
    if (right != NULL) {
        right->printdown();
    }
    // Print ourselves
    cout << value << ":" << count << endl;
    // Print any lower values to our left.
    if (left != NULL) {
        left->printdown();
    }
}
