/** CpSc 210:  Test driver for tree class in lab 12 **/

#include <iostream>
#include "tree.h"

using namespace std;

int main() {
   tree *root = NULL;
   int vals[] = {20, 3, 44, 99, 200, 4, 3, 88, 77, 22, 9, 66, 432, 500,
                 200, 44, 44, 33, 486, 200, 3, 99, 101, 5, 200, 99, 136,
                 444, 189, 89, 100, 200, 300, 400, 150, 250, 350, 450,
                 25, 125, 225, 325, 425, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
                 200, 44, 44, 33, 486, 200, 3, 99, 101, 5, 200, 99, 136,
                 444, 189, 89, 100, 200, 300, 400, 150, 250, 350, 450,
                 200, 500, 201, 499, 202, 498, 203, 497, 204, 496, 205, 
                 -1};
   int ndx=1;

   /** Use first value as root of tree **/
   root = new tree(vals[0]);

   /** Add rest of values to tree **/
   while (vals[ndx] >= 0) {
      root->processValue(vals[ndx]);
      ndx++;
   }

   /** Print tree in ascending order by node value **/
   cout << "Sorted list of numbers (ascending order):\n";
   root->printup();
   cout << endl;

   /** Print tree in descending order by node value **/
   cout << "Sorted list of numbers (descending order):\n";
   root->printdown();
   cout << endl;

   /** Free the tree **/
   cout << "Deleting Tree:\n";
   delete(root);

}
