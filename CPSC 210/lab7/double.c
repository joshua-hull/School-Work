// CP SC 210: Lab 7: double.c (c) 2012 Joshua Hull

#include "dlist.h"

/** newDlist **/
dlist_t *newDlist() {
   dlist_t *list;
   dnode_t *dummyhead, *dummytail;

   /* Create doubly linked list object */
   list = malloc(sizeof(dlist_t));

   /* Create dummy head and tail nodes */
   dummyhead = malloc(sizeof(dnode_t));
   dummyhead->count = -1;
   dummyhead->word = strdup("");
   dummytail = malloc(sizeof(dnode_t));
   dummytail->count = -1;
   dummytail->word = strdup("");
   
   /* And link them into the list */
   dummyhead->next = dummytail;
   dummytail->prev = dummyhead;
   list->head = dummyhead;
   list->tail = dummytail;

   return(list);
}

/** addword **/
void addword(dlist_t *list, char *word) {
   dnode_t *pos;
   dnode_t *new;

   /* Find position in list */
   pos = list->head->next;  /* Skip past dummy header node */
   while ((pos->count != -1) && (strcmp(pos->word, word) < 0)) {
      pos = pos->next;
   }

   /* Was it a match? */
   if (strcmp(pos->word, word) == 0) {
      pos->count++;
      return;
   }

   /* No -- create new node and insert into list */
   new = malloc(sizeof(dnode_t));
   new->next = pos;              /* Point new node's next to successor   */
   new->prev = pos->prev;/* Point new node's prev to 
                                    predecessor */
   pos->prev->next = new;    /* Point predecessor node to new node   */
   pos->prev = new;          /* Point successor node to new node     */
   new->count = 1;               /* Initialize count                     */
   new->word = strdup(word);     /* And point to copy of new word        */
   return;
}

/** deleteword **/
int deleteword(dlist_t *list, char *word) {
    dnode_t *ptr;
    // Start at the head of the list.
    ptr = list->head;
    // While we haven't jumped off the end of the list and haven't found our
    // word yet...
    while(ptr != NULL && strcmp(word,ptr->word) != 0){
        // Move to the next node in the list.
        ptr = ptr->next;
    }
    if(ptr == NULL) {
        // If we are at the end of the list then we didn't find the word.
        return(0);
    } else {
        // So we found it. Rearrange the appropriate pointers, free up some
        // space, and return.
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        free(ptr);
        return(1);
    }
}
   
/** print **/
void print(dlist_t *list) {
   dnode_t *pos;
   pos = list->head->next;     /* Skip over dummy header node          */
   while (pos->count != -1) {
      printf("%s: %d\n", pos->word, pos->count);
      pos = pos->next;
   }
}

/** rprint **/
void rprint(dlist_t *list) {
    dnode_t *ptr;
    // Start at the end of the list.
    ptr = list->tail->prev;
    // While we haven't found the dummy node...
    while (ptr->count != -1) {
        // Print out the node and move to the next one.
        printf("%s: %d\n",ptr->word,ptr->count);
        ptr = ptr->prev;
    }
}
