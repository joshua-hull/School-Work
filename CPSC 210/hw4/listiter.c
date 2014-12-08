// Demonstrate iterators. 
#include "list.h"

int main() {
      // create a linked list object
      list_t *ll = newList();
      // add elements to the list 
      l_add(ll, "C"); 
      l_add(ll, "A"); 
      l_add(ll, "E"); 
      l_add(ll, "B"); 
      l_add(ll, "D"); 
      l_add(ll, "F"); 
      // use iterator to display contents of ll 
      printf("Contents of ll: "); 
      iterator_t *itr = newIterator(ll); 
      while(l_hasnext(itr)) {

          char *element = l_next(itr);
          printf("%s ", element);

      } 
      printf("\n");
      exit(0);
}

