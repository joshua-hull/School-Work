#ifndef TREE_H
#define TREE_H

using namespace std;

class tree {
   public:
      // Constructors
      tree(int initialValue); // Construct an instance of a tree
                              //   setting value field to initialValue

      // Destructor
      ~tree();                // Destroy the tree and its subtrees

      // Methods associated with tree object
      void processValue(int searchvalue);
      void printup();
      void printdown();

   private:
      //  Data associated with a tree object
      int value;          // Node value
      int count;          // Occurence count of value
      tree *left;         // Pointer to left subtree
      tree *right;        // Pointer to right subtree
};

#endif
