#include "BST.h"
#include <iostream>

using namespace std;
int main(){
BST *t = new BST();

if (t->insert(5)) 
   cout << "5 was inserted" << endl;        
else
   cout << "5 was not inserted" << endl;

if (t->insert(4)) 
   cout << "4 was inserted" << endl;       
else
   cout << "4 was not inserted" << endl;

if (t->insert(9)) 
   cout << "9 was inserted" << endl;     
else
   cout << "9 was not inserted" << endl;

if (t->insert(5))                            
   cout << "5 was inserted" << endl;     
else
   cout << "5 was not inserted" << endl;

if (t->find(9))
   cout << "9 was found" << endl;
else
   cout << "9 was not found" << endl;       
if (t->insert(12)) 
   cout << "12 was inserted" << endl;        
else
   cout << "12 was not inserted" << endl;

if (t->insert(12)) 
   cout << "12 was inserted" << endl;        
else
   cout << "12 was not inserted" << endl;

if (t->insert(6)) 
   cout << "6 was inserted" << endl;        
else
   cout << "6 was not inserted" << endl;

if (t->insert(3)) 
   cout << "3 was inserted" << endl;        
else
   cout << "3 was not inserted" << endl;



t->preorder();  
t->postorder();
t->inorder();

if (t->remove(5))
   cout << "5 was deleted" << endl;
else
   cout << "5 was not deleted" << endl;

t->preorder();  
t->postorder();
t->inorder();

cout << "All done!" << endl;

return 0;
}
