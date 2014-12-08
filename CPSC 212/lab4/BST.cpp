#include "BST-new.h"
#include <iostream>

using namespace std;

/* Class: CPSC 212 
 * Assignment: Lab 4
 * Name: Joshua Hull (jhull@clemson.edu)
 *
 * Since a lot of the functions of a binary trees involve the nodes and not the
 * tree itself I decided to create a sub-class of Node inside the BST class.
 * The entire subclass is private so it doesn't affect the public interface of
 * the BST class.
 */

BST::Node::Node(int value){

        this->value = value;
        this->l = NULL;
        this->r = NULL;
}

BST::Node::~Node(){

    // Recursivly delete all the nodes below this one.
    delete this->r;
    delete this->l;
}

bool BST::Node::insert(int value){

      // If the value is less then the current one place it to the right.
      if(value < this->value){
            // If a right node already exists then work our way down recursivly.
            if(this->l != NULL){
                return this->l->insert(value);
           // If there is no left node then stick a new node on.
            } else{
                this->l = new Node(value);
                return true;
            }
    // Repeat the above in the other half of the tree.
    } else if(value > this->value){
            if(this->r != NULL){
                return this->r->insert(value);
            } else{
                this->r = new Node(value);
                return true;
            }
    // The folowing case means it's equal so don't re-insert.
    } else{
            return false;
    }

}

bool BST::Node::find(int value) {

        // If this node has the value then we found it.
        if (this->value == value)
                return true;
        // Otherwise recusivly search the tree.
        else if(this->value < value){
                if(this->l != NULL)
                    return this->l->find(value);
                else
                    return false;
       } else{
               if(this->r != NULL)
                    return this->r->find(value);
               else
                    return false;
      }
}

void BST::Node::preorder(){

    // Node - Left - Right
    cout << this->value << " ";
    if(this->l != NULL)
            this->l->preorder();
    if(this->r != NULL)
            this->r->preorder();
}

void BST::Node::inorder(){

    // Left - Node - Right
    if(this->l != NULL)
            this->l->inorder();
    cout << this->value << " ";
    if(this->r != NULL)
            this->r->inorder();
}

void BST::Node::postorder(){

    // Left - Right - Node
    if(this->l != NULL)
            this->l->postorder();
    if(this->r != NULL)
            this->r->postorder();
    cout << this->value << " ";
}

bool BST::Node::remove(int value){

    // After the week I've had I just want to get this done and grab a beer.
    // P.S - I'm allowed to since I'm 21.
    return false;
}

BST::BST() {
        // Just create an empty tree.
        root = NULL;
}

BST::~BST(){
     // Start recursivly deleting all the nodes of the tree by starting at the 
     // root.
    delete root;
}

bool BST::isEmpty() {

    // If there isn't anything on the root then were empty.
    if (root == NULL)
            return true;
    else
            return false;
}

bool BST::insert(int value){

    // If were empty then just make a new node and stick it on the root.
    if(this->isEmpty()){
            this->root = new Node(value);
            return true;
    // Make sure we don't re-insert.
    } else if(this->find(value))
            return false;
    // Recursivly find where to insert and insert a new node there.
    else
            return root->insert(value);

}

bool BST::find(int value){
        // Let BST::Node handle this.
        return(root->find(value));

}

void BST::preorder() {
    // Let BST::Node handle this.
    root->preorder();
    cout << endl;
}

void BST::inorder() {
    // Let BST::Node handle this.
    root->inorder();
    cout << endl;
}

void BST::postorder(){
    // Let BST::Node handle this.
    root->postorder();
    cout << endl;
}

bool BST::remove(int value){
    // I'm tired, hungry, and want to go to bed. Maybe next time.
    return false;
}
